// file      : cli/parser.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <iostream>

#include "token.hxx"
#include "lexer.hxx"
#include "parser.hxx"

using namespace std;

const char* keywords[] =
{
  "include",
  "namespace",
  "class",
  "signed",
  "unsigned",
  "bool",
  "char",
  "wchar_t",
  "short",
  "int",
  "long",
  "float",
  "double"
};

const char* punctuation[] = {";", ",", "::", "{", "}", /*"(", ")",*/ "=", "|"};

// Output the token type and value in a format suitable for diagnostics.
//
std::ostream&
operator<< (std::ostream& os, token const& t)
{
  switch (t.type ())
  {
  case token::t_eos:
    {
      os << "end-of-stream";
      break;
    }
  case token::t_keyword:
    {
      os << "keyword '" << keywords[t.keyword ()] << "'";
      break;
    }
  case token::t_identifier:
    {
      os << "identifier '" << t.identifier () << "'";
      break;
    }
  case token::t_punctuation:
    {
      os << "'" << punctuation[t.punctuation ()] << "'";
      break;
    }
  case token::t_path_lit:
    {
      os << "path literal";
      break;
    }
  case token::t_string_lit:
    {
      os << "string literal";
      break;
    }
  case token::t_char_lit:
    {
      os << "char literal";
      break;
    }
  case token::t_bool_lit:
    {
      os << "bool literal";
      break;
    }
  case token::t_int_lit:
    {
      os << "integer literal";
      break;
    }
  case token::t_float_lit:
    {
      os << "floating point literal";
      break;
    }
  case token::t_call_expr:
    {
      os << "call expression";
      break;
    }
  case token::t_template_expr:
    {
      os << "template expression";
      break;
    }
  }

  return os;
}

void parser::
recover (token& t)
{
  // Recover by skipping past next ';'.
  //
  for (;; t = lexer_->next ())
  {
    if (t.type () == token::t_eos)
      break;

    if (t.punctuation () == token::p_semi)
    {
      t = lexer_->next ();
      break;
    }
  }
}

void parser::
parse (std::istream& is, std::string const& id)
{
  lexer l (is, id);
  lexer_ = &l;
  id_ = &id;
  valid_ = true;
  def_unit ();

  if (!valid_ || !l.valid ())
    throw invalid_input ();
}

void parser::
def_unit ()
{
  token t (lexer_->next ());

  // include-decl-seq
  //
  while (t.keyword () == token::k_include)
  {
    try
    {
      include_decl ();
      t = lexer_->next ();
    }
    catch (error const&)
    {
      valid_ = false;
      recover (t);
    }
  }

  // decl-seq
  //
  while (t.type () != token::t_eos)
  {
    try
    {
      if (decl (t))
      {
        t = lexer_->next ();
        continue;
      }

      cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
           << "expected namespace or class declaration instead of " << t
           << endl;
      throw error ();
    }
    catch (error const&)
    {
      valid_ = false;
      break; // Non-recoverable error.
    }
  }
}

void parser::
include_decl ()
{
  token t (lexer_->next ());

  if (t.type () != token::t_path_lit)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected path literal instead of " << t << endl;
    throw error ();
  }

  t = lexer_->next ();

  if (t.punctuation () != token::p_semi)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected ';' instead of " << t << endl;
    throw error ();
  }
}

bool parser::
decl (token& t)
{
  if (t.type () == token::t_keyword)
  {
    switch (t.keyword ())
    {
    case token::k_namespace:
      {
        namespace_def ();
        return true;
      }
    case token::k_class:
      {
        class_def ();
        return true;
      }
    default:
      break;
    }
  }
  return false;
}

void parser::
namespace_def ()
{
  token t (lexer_->next ());

  if (t.type () != token::t_identifier)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected identifier instead of " << t << endl;
    throw error ();
  }

  t = lexer_->next ();

  if (t.punctuation () != token::p_lcbrace)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected '{' instead of " << t << endl;
    throw error ();
  }

  // decl-seq
  //
  t = lexer_->next ();

  while (decl (t))
    t = lexer_->next ();

  if (t.punctuation () != token::p_rcbrace)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected namespace declaration, class declaration, or '}' "
         << "instead of " << t << endl;
    throw error ();
  }
}

void parser::
class_def ()
{
  token t (lexer_->next ());

  if (t.type () != token::t_identifier)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected identifier instead of " << t << endl;
    throw error ();
  }

  t = lexer_->next ();

  if (t.punctuation () != token::p_lcbrace)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected '{' instead of " << t << endl;
    throw error ();
  }

  // decl-seq
  //
  t = lexer_->next ();

  while (true)
  {
    try
    {
      if (!option_def (t))
        break;

      t = lexer_->next ();
    }
    catch (error const&)
    {
      valid_ = false;
      recover (t);
    }
  }

  if (t.punctuation () != token::p_rcbrace)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected option declaration or '}' instead of " << t << endl;
    throw error ();
  }

  t = lexer_->next ();

  if (t.punctuation () != token::p_semi)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected ';' instead of " << t << endl;
    throw error ();
  }
}

bool parser::
option_def (token& t)
{
  // type-spec
  //
  // These two functions set t to the next token if they return
  // true.
  //
  if (!qualified_name (t) && !fundamental_type (t))
    return false;

  // option-name-seq
  //
  while (true)
  {
    switch (t.type ())
    {
    case token::t_identifier:
    case token::t_string_lit:
      {
        break;
      }
    default:
      {
        cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
             << "option name expected instead of " << t << endl;
        throw error ();
      }
    }

    t = lexer_->next ();

    if (t.punctuation () == token::p_or)
      t = lexer_->next ();
    else
      break;
  }

  // initializer
  //
  if (t.punctuation () == token::p_eq)
  {
    t = lexer_->next ();

    // assignment initiaizer
    //
    if (qualified_name (t))
    {
    }
    else
    {
      switch (t.type ())
      {
      case token::t_string_lit:
      case token::t_char_lit:
      case token::t_bool_lit:
      case token::t_int_lit:
      case token::t_float_lit:
      case token::t_call_expr:
        {
          t = lexer_->next ();
          break;
        }
      default:
        {
          cerr << *id_ << ':' << t.line () << ':' << t.column ()
               << ": error: expected intializer instead of " << t << endl;
          throw error ();
        }
      }
    }
  }
  else if (t.type () == token::t_call_expr)
  {
    // c-tor initializer
    //
    t = lexer_->next ();
  }

  if (t.punctuation () != token::p_semi)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected ';' instead of " << t << endl;
    throw error ();
  }

  return true;
}

bool parser::
qualified_name (token& t)
{
  if (t.type () != token::t_identifier && t.punctuation () != token::p_dcolon)
    return false;

  if (t.punctuation () == token::p_dcolon)
    t = lexer_->next ();

  while (true)
  {
    if (t.type () != token::t_identifier)
    {
      cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
           << "expected identifier after '::'" << endl;
      throw error ();
    }

    t = lexer_->next ();

    if (t.type () == token::t_template_expr)
    {
      // Template-id.
      //
      t = lexer_->next ();
    }

    if (t.punctuation () == token::p_dcolon)
      t = lexer_->next ();
    else
      break;
  }

  return true;
}

bool parser::
fundamental_type (token& t)
{
  switch (t.keyword ())
  {
  case token::k_signed:
  case token::k_unsigned:
    {
      switch ((t = lexer_->next ()).keyword ())
      {
      case token::k_short:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_int:
            {
              t = lexer_->next ();
            }
          default:
            break;
          }
          break;
        }
      case token::k_long:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_int:
            {
              t = lexer_->next ();
              break;
            }
          case token::k_long:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_int:
                {
                  t = lexer_->next ();
                }
              default:
                break;
              }
              break;
            }
          default:
            break;
          }
          break;
        }
      case token::k_int:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_short:
            {
              t = lexer_->next ();
              break;
            }
          case token::k_long:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_long:
                {
                  t = lexer_->next ();
                }
              default:
                break;
              }
              break;
            }
          default:
            break;
          }
          break;
        }
      case token::k_char:
        {
          t = lexer_->next ();
          break;
        }
      default:
        break;
      }
      break;
    }
  case token::k_short:
  case token::k_long:
    {
      bool l (t.keyword () == token::k_long);

      switch ((t = lexer_->next ()).keyword ())
      {
      case token::k_signed:
      case token::k_unsigned:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_int:
            {
              t = lexer_->next ();
            }
          default:
            break;
          }
          break;
        }
      case token::k_long:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_signed:
          case token::k_unsigned:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_int:
                {
                  t = lexer_->next ();
                }
              default:
                break;
              }
              break;
            }
          case token::k_int:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_signed:
              case token::k_unsigned:
                {
                  t = lexer_->next ();
                }
              default:
                break;
              }
              break;
            }
          default:
            break;
          }
          break;
        }
      case token::k_int:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_signed:
          case token::k_unsigned:
            {
              t = lexer_->next ();
            }
          default:
            break;
          }
          break;
        }
      case token::k_double:
        {
          if (l)
            t = lexer_->next ();

          break;
        }
      default:
        break;
      }
      break;
    }
  case token::k_int:
    {
      switch ((t = lexer_->next ()).keyword ())
      {
      case token::k_signed:
      case token::k_unsigned:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_short:
            {
              t = lexer_->next ();
              break;
            }
          case token::k_long:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_long:
                {
                  t = lexer_->next ();
                }
              default:
                break;
              }
            }
          default:
            break;
          }
          break;
        }
      case token::k_short:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_signed:
          case token::k_unsigned:
            {
              t = lexer_->next ();
            }
          default:
            break;
          }
          break;
        }
      case token::k_long:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_signed:
          case token::k_unsigned:
            {
              t = lexer_->next ();
              break;
            }
          case token::k_long:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_signed:
              case token::k_unsigned:
                {
                  t = lexer_->next ();
                }
              default:
                break;
              }
              break;
            }
          default:
            break;
          }
          break;
        }
      default:
        break;
      }
      break;
    }
  case token::k_char:
    {
      switch ((t = lexer_->next ()).keyword ())
      {
      case token::k_signed:
      case token::k_unsigned:
        {
          t = lexer_->next ();
        }
      default:
        break;
      }
      break;
    }
  case token::k_bool:
  case token::k_wchar:
  case token::k_float:
    {
      t = lexer_->next ();
      break;
    }
  case token::k_double:
    {
      switch ((t = lexer_->next ()).keyword ())
      {
      case token::k_long:
        {
          t = lexer_->next ();
        }
      default:
        break;
      }
      break;
    }
  default:
    return false;
  }

  return true;
}
