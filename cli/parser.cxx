// file      : cli/parser.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <iostream>

#include "token.hxx"
#include "lexer.hxx"
#include "parser.hxx"

#include "semantics.hxx"

using namespace std;
using namespace semantics;

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

auto_ptr<cli_unit> parser::
parse (std::istream& is, path const& p)
{
  auto_ptr<cli_unit> unit (new cli_unit (p));
  unit_ = unit.get ();

  lexer l (is, p.string ());
  lexer_ = &l;

  path_ = &p;
  valid_ = true;

  def_unit ();

  if (!valid_ || !l.valid ())
    throw invalid_input ();

  return unit;
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

  scope* old (scope_);
  scope_ = unit_;

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

      cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
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

  scope_ = old;
}

void parser::
include_decl ()
{
  token t (lexer_->next ());

  if (t.type () != token::t_path_lit)
  {
    cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected path literal instead of " << t << endl;
    throw error ();
  }

  if (valid_)
  {
    cxx_unit& n (unit_->new_node<cxx_unit> (*path_, t.line (), t.column ()));
    unit_->new_edge<cxx_includes> (*unit_, n, t.literal ());
  }

  t = lexer_->next ();

  if (t.punctuation () != token::p_semi)
  {
    cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
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
    cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected identifier instead of " << t << endl;
    throw error ();
  }

  scope* old (scope_);

  if (valid_)
  {
    namespace_& n (
      unit_->new_node<namespace_> (*path_, t.line (), t.column ()));
    unit_->new_edge<names> (*scope_, n, t.identifier ());
    scope_ = &n;
  }

  t = lexer_->next ();

  if (t.punctuation () != token::p_lcbrace)
  {
    cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected '{' instead of " << t << endl;
    throw error ();
  }

  // decl-seq
  //
  t = lexer_->next ();

  while (decl (t))
    t = lexer_->next ();

  scope_ = old;

  if (t.punctuation () != token::p_rcbrace)
  {
    cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
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
    cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected identifier instead of " << t << endl;
    throw error ();
  }

  scope* old (scope_);

  if (valid_)
  {
    class_& n (unit_->new_node<class_> (*path_, t.line (), t.column ()));
    unit_->new_edge<names> (*scope_, n, t.identifier ());
    scope_ = &n;
  }

  t = lexer_->next ();

  if (t.punctuation () != token::p_lcbrace)
  {
    cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
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

  scope_ = old;

  if (t.punctuation () != token::p_rcbrace)
  {
    cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected option declaration or '}' instead of " << t << endl;
    throw error ();
  }

  t = lexer_->next ();

  if (t.punctuation () != token::p_semi)
  {
    cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected ';' instead of " << t << endl;
    throw error ();
  }
}

bool parser::
option_def (token& t)
{
  size_t l (t.line ()), c (t.column ());

  // type-spec
  //
  // These two functions set t to the next token if they return
  // true.
  //
  string type_name;

  if (!qualified_name (t, type_name) && !fundamental_type (t, type_name))
    return false;

  option* o (0);

  if (valid_)
  {
    o = &unit_->new_node<option> (*path_, l, c);
    type& t (unit_->new_type (*path_, l, c, type_name));
    unit_->new_edge<belongs> (*o, t);
  }

  // option-name-seq
  //
  names::name_list nl;
  while (true)
  {
    switch (t.type ())
    {
    case token::t_identifier:
      {
        if (valid_)
          nl.push_back (t.identifier ());

        break;
      }
    case token::t_string_lit:
      {
        if (valid_)
        {
          // Get rid of '"'.
          //
          string r;
          string const& l (t.literal ());

          for (size_t i (0), n (l.size ()); i < n; ++i)
          {
            if (l[i] != '"' || (i != 0 && l[i - 1] == '\\'))
              r += l[i];
          }

          nl.push_back (r);
        }

        break;
      }
    default:
      {
        cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
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

  if (valid_)
    unit_->new_edge<names> (*scope_, *o, nl);

  // initializer
  //
  std::string ev;
  expression::expression_type et;

  if (t.punctuation () == token::p_eq)
  {
    // assignment initiaizer
    //
    t = lexer_->next ();

    l = t.line ();
    c = t.column ();

    if (qualified_name (t, ev))
    {
      et = expression::identifier;
    }
    else
    {
      switch (t.type ())
      {
      case token::t_string_lit:
        {
          ev = t.literal ();
          et = expression::string_lit;
          t = lexer_->next ();
          break;
        }
      case token::t_char_lit:
        {
          ev = t.literal ();
          et = expression::char_lit;
          t = lexer_->next ();
          break;
        }
      case token::t_bool_lit:
        {
          ev = t.literal ();
          et = expression::bool_lit;
          t = lexer_->next ();
          break;
        }
      case token::t_int_lit:
        {
          ev = t.literal ();
          et = expression::int_lit;
          t = lexer_->next ();
          break;
        }
      case token::t_float_lit:
        {
          ev = t.literal ();
          et = expression::float_lit;
          t = lexer_->next ();
          break;
        }
      case token::t_call_expr:
        {
          ev = t.expression ();
          et = expression::call_expr;
          t = lexer_->next ();
          break;
        }
      default:
        {
          cerr << *path_ << ':' << t.line () << ':' << t.column ()
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
    l = t.line ();
    c = t.column ();

    ev = t.expression ();
    et = expression::call_expr;
    t = lexer_->next ();
  }

  if (valid_ && !ev.empty ())
  {
    expression& e (unit_->new_node<expression> (*path_, l, c, et, ev));
    unit_->new_edge<initialized> (*o, e);
  }

  if (t.punctuation () == token::p_lcbrace)
  {
    // doc-string-seq
    //
    for (t = lexer_->next ();; t = lexer_->next ())
    {
      if (t.type () != token::t_string_lit)
      {
        cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
             << "expected documentation string instead of " << t << endl;
        throw error ();
      }

      if (valid_)
      {
        // Get rid of '"'.
        //
        string r;
        string const& l (t.literal ());

        for (size_t i (0), n (l.size ()); i < n; ++i)
        {
          if (l[i] != '"' || (i != 0 && l[i - 1] == '\\'))
            r += l[i];
        }

        o->doc ().push_back (r);
      }

      t = lexer_->next ();

      if (t.punctuation () != token::p_comma)
        break;
    }

    if (t.punctuation () != token::p_rcbrace)
    {
      cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
           << "expected '}' instead of " << t << endl;
      throw error ();
    }

    t = lexer_->next ();
  }

  if (t.punctuation () != token::p_semi)
  {
    cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
         << "expected ';' instead of " << t << endl;
    throw error ();
  }

  return true;
}

bool parser::
qualified_name (token& t, string& r)
{
  if (t.type () != token::t_identifier && t.punctuation () != token::p_dcolon)
    return false;

  r.clear ();

  if (t.punctuation () == token::p_dcolon)
  {
    r += "::";
    t = lexer_->next ();
  }

  while (true)
  {
    if (t.type () != token::t_identifier)
    {
      cerr << *path_ << ':' << t.line () << ':' << t.column () << ": error: "
           << "expected identifier after '::'" << endl;
      throw error ();
    }

    r += t.identifier ();
    t = lexer_->next ();

    if (t.type () == token::t_template_expr)
    {
      // Template-id.
      //
      r += t.expression ();
      t = lexer_->next ();
    }

    if (t.punctuation () == token::p_dcolon)
    {
      r += "::";
      t = lexer_->next ();
    }
    else
      break;
  }

  return true;
}

bool parser::
fundamental_type (token& t, string& r)
{
  r.clear ();

  switch (t.keyword ())
  {
  case token::k_signed:
  case token::k_unsigned:
    {
      r = t.keyword () == token::k_signed ? "signed" : "unsigned";
      switch ((t = lexer_->next ()).keyword ())
      {
      case token::k_short:
        {
          r += " short";
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_int:
            {
              r += " int";
              t = lexer_->next ();
            }
          default:
            break;
          }
          break;
        }
      case token::k_long:
        {
          r += " long";
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_int:
            {
              r += " int";
              t = lexer_->next ();
              break;
            }
          case token::k_long:
            {
              r += " long";
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_int:
                {
                  r += " int";
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
          r += " int";
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_short:
            {
              r += " short";
              t = lexer_->next ();
              break;
            }
          case token::k_long:
            {
              r += " long";
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_long:
                {
                  r += " long";
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
          r += " char";
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
      r = l ? "long" : "short";

      switch ((t = lexer_->next ()).keyword ())
      {
      case token::k_signed:
      case token::k_unsigned:
        {
          r += t.keyword () == token::k_signed ? " signed" : " unsigned";
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_int:
            {
              r += " int";
              t = lexer_->next ();
            }
          default:
            break;
          }
          break;
        }
      case token::k_long:
        {
          r += " long";
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_signed:
          case token::k_unsigned:
            {
              r += t.keyword () == token::k_signed ? " signed" : " unsigned";
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_int:
                {
                  r += " int";
                  t = lexer_->next ();
                }
              default:
                break;
              }
              break;
            }
          case token::k_int:
            {
              r += " int";
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_signed:
                {
                  r += " signed";
                  t = lexer_->next ();
                  break;
                }
              case token::k_unsigned:
                {
                  r += " unsigned";
                  t = lexer_->next ();
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
      case token::k_int:
        {
          r += " int";
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_signed:
            {
              r += " signed";
              t = lexer_->next ();
              break;
            }
          case token::k_unsigned:
            {
              r += " unsigned";
              t = lexer_->next ();
              break;
            }
          default:
            break;
          }
          break;
        }
      case token::k_double:
        {
          if (l)
          {
            r += " double";
            t = lexer_->next ();
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
      r = "int";
      switch ((t = lexer_->next ()).keyword ())
      {
      case token::k_signed:
      case token::k_unsigned:
        {
          r += t.keyword () == token::k_signed ? " signed" : " unsigned";
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_short:
            {
              r += " short";
              t = lexer_->next ();
              break;
            }
          case token::k_long:
            {
              r += " long";
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_long:
                {
                  r += " long";
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
          r += " short";
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_signed:
            {
              r += " signed";
              t = lexer_->next ();
              break;
            }
          case token::k_unsigned:
            {
              r += " unsigned";
              t = lexer_->next ();
              break;
            }
          default:
            break;
          }
          break;
        }
      case token::k_long:
        {
          r += " long";
          switch ((t = lexer_->next ()).keyword ())
          {
          case token::k_signed:
            {
              r += " signed";
              t = lexer_->next ();
              break;
            }
          case token::k_unsigned:
            {
              r += " unsigned";
              t = lexer_->next ();
              break;
            }
          case token::k_long:
            {
              r += " long";
              switch ((t = lexer_->next ()).keyword ())
              {
              case token::k_signed:
                {
                  r += " signed";
                  t = lexer_->next ();
                  break;
                }
              case token::k_unsigned:
                {
                  r += " unsigned";
                  t = lexer_->next ();
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
      default:
        break;
      }
      break;
    }
  case token::k_char:
    {
      r = "char";
      switch ((t = lexer_->next ()).keyword ())
      {
      case token::k_signed:
        {
          r += " signed";
          t = lexer_->next ();
          break;
        }
      case token::k_unsigned:
        {
          r += " unsigned";
          t = lexer_->next ();
          break;
        }
      default:
        break;
      }
      break;
    }
  case token::k_bool:
    {
      r = "bool";
      t = lexer_->next ();
      break;
    }
  case token::k_wchar:
    {
      r = "wchar_t";
      t = lexer_->next ();
      break;
    }
  case token::k_float:
    {
      r = "float";
      t = lexer_->next ();
      break;
    }
  case token::k_double:
    {
      r = "double";
      switch ((t = lexer_->next ()).keyword ())
      {
      case token::k_long:
        {
          r += " long";
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
