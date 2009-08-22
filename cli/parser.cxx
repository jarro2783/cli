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
operator<< (std::ostream& os, Token const& t)
{
  switch (t.type ())
  {
  case Token::t_eos:
    {
      os << "end-of-stream";
      break;
    }
  case Token::t_keyword:
    {
      os << "keyword '" << keywords[t.keyword ()] << "'";
      break;
    }
  case Token::t_identifier:
    {
      os << "identifier '" << t.identifier () << "'";
      break;
    }
  case Token::t_punctuation:
    {
      os << "'" << punctuation[t.punctuation ()] << "'";
      break;
    }
  case Token::t_path_lit:
    {
      os << "path literal";
      break;
    }
  case Token::t_string_lit:
    {
      os << "string literal";
      break;
    }
  case Token::t_char_lit:
    {
      os << "char literal";
      break;
    }
  case Token::t_bool_lit:
    {
      os << "bool literal";
      break;
    }
  case Token::t_int_lit:
    {
      os << "integer literal";
      break;
    }
  case Token::t_float_lit:
    {
      os << "floating point literal";
      break;
    }
  case Token::t_call_expr:
    {
      os << "call expression";
      break;
    }
  case Token::t_template_expr:
    {
      os << "template expression";
      break;
    }
  }

  return os;
}

void Parser::
recover (Token& t)
{
  // Recover by skipping past next ';'.
  //
  for (;; t = lexer_->next ())
  {
    if (t.type () == Token::t_eos)
      break;

    if (t.punctuation () == Token::p_semi)
    {
      t = lexer_->next ();
      break;
    }
  }
}

void Parser::
parse (std::istream& is, std::string const& id)
{
  Lexer lexer (is, id);
  lexer_ = &lexer;
  id_ = &id;
  valid_ = true;
  def_unit ();

  if (!valid_ || !lexer.valid ())
    throw InvalidInput ();
}

void Parser::
def_unit ()
{
  Token t (lexer_->next ());

  // include-decl-seq
  //
  while (t.keyword () == Token::k_include)
  {
    try
    {
      include_decl ();
      t = lexer_->next ();
    }
    catch (Error const&)
    {
      valid_ = false;
      recover (t);
    }
  }

  // decl-seq
  //
  while (t.type () != Token::t_eos)
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
      throw Error ();
    }
    catch (Error const&)
    {
      valid_ = false;
      break; // Non-recoverable error.
    }
  }
}

void Parser::
include_decl ()
{
  Token t (lexer_->next ());

  if (t.type () != Token::t_path_lit)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected path literal instead of " << t << endl;
    throw Error ();
  }

  t = lexer_->next ();

  if (t.punctuation () != Token::p_semi)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected ';' instead of " << t << endl;
    throw Error ();
  }
}

bool Parser::
decl (Token& t)
{
  if (t.type () == Token::t_keyword)
  {
    switch (t.keyword ())
    {
    case Token::k_namespace:
      {
        namespace_def ();
        return true;
      }
    case Token::k_class:
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

void Parser::
namespace_def ()
{
  Token t (lexer_->next ());

  if (t.type () != Token::t_identifier)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected identifier instead of " << t << endl;
    throw Error ();
  }

  t = lexer_->next ();

  if (t.punctuation () != Token::p_lcbrace)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected '{' instead of " << t << endl;
    throw Error ();
  }

  // decl-seq
  //
  t = lexer_->next ();

  while (decl (t))
    t = lexer_->next ();

  if (t.punctuation () != Token::p_rcbrace)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected namespace declaration, class declaration, or '}' "
         << "instead of " << t << endl;
    throw Error ();
  }
}

void Parser::
class_def ()
{
  Token t (lexer_->next ());

  if (t.type () != Token::t_identifier)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected identifier instead of " << t << endl;
    throw Error ();
  }

  t = lexer_->next ();

  if (t.punctuation () != Token::p_lcbrace)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected '{' instead of " << t << endl;
    throw Error ();
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
    catch (Error const&)
    {
      valid_ = false;
      recover (t);
    }
  }

  if (t.punctuation () != Token::p_rcbrace)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected option declaration or '}' instead of " << t << endl;
    throw Error ();
  }

  t = lexer_->next ();

  if (t.punctuation () != Token::p_semi)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected ';' instead of " << t << endl;
    throw Error ();
  }
}

bool Parser::
option_def (Token& t)
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
    case Token::t_identifier:
    case Token::t_string_lit:
      {
        break;
      }
    default:
      {
        cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
             << "option name expected instead of " << t << endl;
        throw Error ();
      }
    }

    t = lexer_->next ();

    if (t.punctuation () == Token::p_or)
      t = lexer_->next ();
    else
      break;
  }

  // initializer
  //
  if (t.punctuation () == Token::p_eq)
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
      case Token::t_string_lit:
      case Token::t_char_lit:
      case Token::t_bool_lit:
      case Token::t_int_lit:
      case Token::t_float_lit:
      case Token::t_call_expr:
        {
          t = lexer_->next ();
          break;
        }
      default:
        {
          cerr << *id_ << ':' << t.line () << ':' << t.column ()
               << ": error: expected intializer instead of " << t << endl;
          throw Error ();
        }
      }
    }
  }
  else if (t.type () == Token::t_call_expr)
  {
    // c-tor initializer
    //
    t = lexer_->next ();
  }

  if (t.punctuation () != Token::p_semi)
  {
    cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected ';' instead of " << t << endl;
    throw Error ();
  }

  return true;
}

bool Parser::
qualified_name (Token& t)
{
  if (t.type () != Token::t_identifier && t.punctuation () != Token::p_dcolon)
    return false;

  if (t.punctuation () == Token::p_dcolon)
    t = lexer_->next ();

  while (true)
  {
    if (t.type () != Token::t_identifier)
    {
      cerr << *id_ << ':' << t.line () << ':' << t.column () << ": error: "
           << "expected identifier after '::'" << endl;
      throw Error ();
    }

    t = lexer_->next ();

    if (t.type () == Token::t_template_expr)
    {
      // Template-id.
      //
      t = lexer_->next ();
    }

    if (t.punctuation () == Token::p_dcolon)
      t = lexer_->next ();
    else
      break;
  }

  return true;
}

bool Parser::
fundamental_type (Token& t)
{
  switch (t.keyword ())
  {
  case Token::k_signed:
  case Token::k_unsigned:
    {
      switch ((t = lexer_->next ()).keyword ())
      {
      case Token::k_short:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case Token::k_int:
            {
              t = lexer_->next ();
            }
          default:
            break;
          }
          break;
        }
      case Token::k_long:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case Token::k_int:
            {
              t = lexer_->next ();
              break;
            }
          case Token::k_long:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case Token::k_int:
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
      case Token::k_int:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case Token::k_short:
            {
              t = lexer_->next ();
              break;
            }
          case Token::k_long:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case Token::k_long:
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
      case Token::k_char:
        {
          t = lexer_->next ();
          break;
        }
      default:
        break;
      }
      break;
    }
  case Token::k_short:
  case Token::k_long:
    {
      bool l (t.keyword () == Token::k_long);

      switch ((t = lexer_->next ()).keyword ())
      {
      case Token::k_signed:
      case Token::k_unsigned:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case Token::k_int:
            {
              t = lexer_->next ();
            }
          default:
            break;
          }
          break;
        }
      case Token::k_long:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case Token::k_signed:
          case Token::k_unsigned:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case Token::k_int:
                {
                  t = lexer_->next ();
                }
              default:
                break;
              }
              break;
            }
          case Token::k_int:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case Token::k_signed:
              case Token::k_unsigned:
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
      case Token::k_int:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case Token::k_signed:
          case Token::k_unsigned:
            {
              t = lexer_->next ();
            }
          default:
            break;
          }
          break;
        }
      case Token::k_double:
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
  case Token::k_int:
    {
      switch ((t = lexer_->next ()).keyword ())
      {
      case Token::k_signed:
      case Token::k_unsigned:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case Token::k_short:
            {
              t = lexer_->next ();
              break;
            }
          case Token::k_long:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case Token::k_long:
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
      case Token::k_short:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case Token::k_signed:
          case Token::k_unsigned:
            {
              t = lexer_->next ();
            }
          default:
            break;
          }
          break;
        }
      case Token::k_long:
        {
          switch ((t = lexer_->next ()).keyword ())
          {
          case Token::k_signed:
          case Token::k_unsigned:
            {
              t = lexer_->next ();
              break;
            }
          case Token::k_long:
            {
              switch ((t = lexer_->next ()).keyword ())
              {
              case Token::k_signed:
              case Token::k_unsigned:
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
  case Token::k_char:
    {
      switch ((t = lexer_->next ()).keyword ())
      {
      case Token::k_signed:
      case Token::k_unsigned:
        {
          t = lexer_->next ();
        }
      default:
        break;
      }
      break;
    }
  case Token::k_bool:
  case Token::k_wchar:
  case Token::k_float:
    {
      t = lexer_->next ();
      break;
    }
  case Token::k_double:
    {
      switch ((t = lexer_->next ()).keyword ())
      {
      case Token::k_long:
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
