// file      : cli/lexer.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <iostream>

#include "lexer.hxx"

using namespace std;

Lexer::
Lexer (istream& is, string const& id)
    : loc_ ("C"),
      is_ (is),
      id_ (id),
      l_ (1),
      c_(1),
      eos_ (false),
      include_ (false),
      valid_ (true),
      buf_ (0, 0, 0),
      unget_ (false)
{
  keyword_map_["include"]   = Token::k_include;
  keyword_map_["namespace"] = Token::k_namespace;
  keyword_map_["class"]     = Token::k_class;
  keyword_map_["signed"]    = Token::k_signed;
  keyword_map_["unsigned"]  = Token::k_unsigned;
  keyword_map_["bool"]      = Token::k_bool;
  keyword_map_["char"]      = Token::k_char;
  keyword_map_["wchar_t"]   = Token::k_wchar;
  keyword_map_["short"]     = Token::k_short;
  keyword_map_["int"]       = Token::k_int;
  keyword_map_["long"]      = Token::k_long;
  keyword_map_["float"]     = Token::k_float;
  keyword_map_["double"]    = Token::k_double;
}

Lexer::Char Lexer::
peek ()
{
  if (unget_)
    return buf_;
  else
  {
    if (eos_)
      return Char (Char::Traits::eof (), l_, c_);
    else
    {
      Char::IntType i (is_.peek ());

      if (i == Char::Traits::eof ())
        eos_ = true;

      return Char (i, l_, c_);
    }
  }
}

Lexer::Char Lexer::
get ()
{
  if (unget_)
  {
    unget_ = false;
    return buf_;
  }
  else
  {
    // When is_.get () returns eof, the failbit is also set (stupid,
    // isn't?) which may trigger an exception. To work around this
    // we will call peek() first and only call get() if it is not
    // eof. But we can only call peek() on eof once; any subsequent
    // calls will spoil the failbit (even more stupid).
    //
    Char c (peek ());

    if (!is_eos (c))
    {
      is_.get ();

      if (c == '\n')
      {
        l_++;
        c_ = 1;
      }
      else
        c_++;
    }

    return c;
  }
}

void Lexer::
unget (Char c)
{
  // Because iostream::unget cannot work once eos is reached,
  // we have to provide our own implementation.
  //
  buf_ = c;
  unget_ = true;
}

Token Lexer::
next ()
{
  while (true) // Recovery loop.
  {
    bool include (include_);
    include_ = false;

    skip_spaces ();

    Char c (get ());

    if (is_eos (c))
      return Token (c.line (), c.column ());

    try
    {
      switch (c)
      {
      case '\'':
        {
          return char_literal (c);
        }
      case '\"':
        {
          if (include)
            return path_literal (c);
          else
            return string_literal (c);
        }
      case '<':
        {
          if (include)
            return path_literal (c);
          else
            return template_expression (c);
        }
      case ';':
        {
          return Token (Token::p_semi, c.line (), c.column ());
        }
      case ',':
        {
          return Token (Token::p_comma, c.line (), c.column ());
        }
      case ':':
        {
          if (peek () == ':')
          {
            get ();
            return Token (Token::p_dcolon, c.line (), c.column ());
          }
          break;
        }
      case '{':
        {
          return Token (Token::p_lcbrace, c.line (), c.column ());
        }
      case '}':
        {
          return Token (Token::p_rcbrace, c.line (), c.column ());
        }
      case '(':
        {
          return call_expression (c);
        }
      case '=':
        {
          return Token (Token::p_eq, c.line (), c.column ());
        }
      case '|':
        {
          return Token (Token::p_or, c.line (), c.column ());
        }
      case '-':
        {
          // This can be a beginning of an identifier or a an integer
          // literal. Figure out which one it is.
          //
          Char p (peek ());

          if (is_dec_digit (p))
            return int_literal (get (), true, c.line (), c.column ());
          else if (is_space (p))
          {
            skip_spaces ();
            p = peek ();

            if (is_dec_digit (p))
              return int_literal (get (), true, c.line (), c.column ());

            // Stray '-'.
            //
            cerr << id_ << ':' << c.line () << ':' << c.column ()
                 << ": error: unexpected character '-'" << endl;
            throw InvalidInput ();
          }

          break;
        }
      }

      if (is_alpha (c) || c == '_' || c == '-' || c == '/')
      {
        return identifier (c);
      }

      if (is_dec_digit (c))
      {
        return int_literal (c);
      }

      cerr << id_ << ':' << c.line () << ':' << c.column ()
           << ": error: unexpected character '" << c << "'" << endl;
      throw InvalidInput ();
    }
    catch (InvalidInput const&)
    {
      valid_ = false;
    }

    // Try to recover.
    //
    do
    {
      c = get ();

      if (is_eos (c))
        return Token (c.line (), c.column ());
    } while (c != ';');
  }
}

void Lexer::
skip_spaces ()
{
  for (Char c (peek ());; c = peek ())
  {
    if (is_eos (c))
      break;

    if (c == '/')
    {
      c = get ();
      Char p (peek ());

      if (p == '/')
      {
        get ();

        // C++ comment. Read until newline or eos.
        //
        for (c = get (); !is_eos (c) && c != '\n'; c = get ()) ;
        continue;
      }
      else if (p == '*')
      {
        get ();

        // C comment.
        //
        for (c = get ();; c = get ())
        {
          if (is_eos (c))
          {
            cerr << id_ << ':' << c.line () << ':' << c.column ()
                 << ": error: end of stream reached while reading "
                 << "C-style comment" << endl;
            throw InvalidInput ();
          }

          if (c == '*')
          {
            c = peek ();
            if (c == '/')
            {
              get ();
              break;
            }
          }
        }
        continue;
      }
      else
      {
        unget (c);
        break;
      }
    }

    if (!is_space (c))
      break;

    get ();
  }
}

Token Lexer::
identifier (Char c)
{
  size_t ln (c.line ()), cl (c.column ());
  string lexeme;
  lexeme += c;

  bool check (c == '-' || c == '/');

  for (c = peek ();
       !is_eos (c) && (is_alnum (c) || c == '_' || c == '-');
       c = peek ())
  {
    get ();
    lexeme += c;
  }

  // Check for invalid identifiers.
  //
  if (check)
  {
    size_t i (1);

    for (; i < lexeme.size (); ++i)
      if (is_alnum (lexeme[i]) || lexeme[i] == '_')
        break;

    if (i == lexeme.size ())
    {
      cerr << id_ << ':' << c.line () << ':' << c.column () << ": error: "
           << "invalid character sequence '" << lexeme << "'" << endl;
      throw InvalidInput ();
    }
  }

  KeywordMap::const_iterator i (keyword_map_.find (lexeme));

  if (i != keyword_map_.end ())
  {
    if (i->second == Token::k_include)
      include_ = true;

    return Token (i->second, ln, cl);
  }

  if (lexeme == "true" || lexeme == "false")
    return Token (Token::t_bool_lit, lexeme, ln, cl);

  return Token (Token::t_identifier, lexeme, ln, cl);
}

Token Lexer::
int_literal (Char c, bool neg, size_t ml, size_t mc)
{
  size_t ln (neg ? ml : c.line ()), cl (neg ? mc : c.column ());
  string lexeme;

  if (neg)
    lexeme += '-';

  lexeme += c;

  for (c = peek (); !is_eos (c) && is_dec_digit (c); c = peek ())
  {
    get ();
    lexeme += c;
  }

  return Token (Token::t_int_lit, lexeme, ln, cl);
}

Token Lexer::
char_literal (Char c)
{
  size_t ln (c.line ()), cl (c.column ());
  string lexeme;
  lexeme += c;

  char p (c);

  while (true)
  {
    c = get ();

    if (is_eos (c))
    {
      cerr << id_ << ':' << c.line () << ':' << c.column () << ": error: "
           << "end of stream reached while reading character literal" << endl;
      throw InvalidInput ();
    }

    lexeme += c;

    if (c == '\'' && p != '\\')
      break;

    // We need to keep track of \\ escapings so we don't confuse
    // them with \', as in '\\'.
    //
    if (c == '\\' && p == '\\')
      p = '.';
    else
      p = c;
  }

  return Token (Token::t_char_lit, lexeme, ln, cl);
}

Token Lexer::
string_literal (Char c)
{
  size_t ln (c.line ()), cl (c.column ());
  string lexeme;
  lexeme += c;

  while (true)
  {
    lexeme += string_literal_trailer ();

    // Check if there are more strings.
    //
    skip_spaces ();

    c = peek ();

    if (is_eos (c) || c != '"')
      break;

    get ();
    lexeme += " \"";
  }

  return Token (Token::t_char_lit, lexeme, ln, cl);
}

string Lexer::
string_literal_trailer ()
{
  string r;
  char p ('"');

  while (true)
  {
    Char c = get ();

    if (is_eos (c))
    {
      cerr << id_ << ':' << c.line () << ':' << c.column () << ": error: "
           << "end of stream reached while reading string literal" << endl;
      throw InvalidInput ();
    }

    r += c;

    if (c == '"' && p != '\\')
      break;

    // We need to keep track of \\ escapings so we don't confuse
    // them with \', as in '\\'.
    //
    if (c == '\\' && p == '\\')
      p = '.';
    else
      p = c;
  }

  return r;
}

Token Lexer::
path_literal (Char c)
{
  size_t ln (c.line ()), cl (c.column ());
  string lexeme;
  lexeme += c;

  char end (c == '<' ? '>' : '"');

  while (true)
  {
    c = get ();

    if (is_eos (c))
    {
      cerr << id_ << ':' << c.line () << ':' << c.column () << ": error: "
           << "end of stream reached while reading path literal" << endl;
      throw InvalidInput ();
    }

    lexeme += c;

    if (c == end)
      break;
  }

  return Token (Token::t_path_lit, lexeme, ln, cl);
}

Token Lexer::
call_expression (Char c)
{
  size_t ln (c.line ()), cl (c.column ());
  string lexeme;
  lexeme += c;
  size_t balance (1);

  while (balance != 0)
  {
    c = get ();

    if (is_eos (c))
    {
      cerr << id_ << ':' << c.line () << ':' << c.column () << ": error: "
           << "end of stream reached while reading call expression" << endl;
      throw InvalidInput ();
    }

    lexeme += c;

    switch (c)
    {
    case '(':
      {
        balance++;
        break;
      }
    case ')':
      {
        balance--;
        break;
      }
    }
  }

  return Token (Token::t_call_expr, lexeme, ln, cl);
}

Token Lexer::
template_expression (Char c)
{
  size_t ln (c.line ()), cl (c.column ());
  string lexeme;
  lexeme += c;
  size_t balance (1);

  while (balance != 0)
  {
    c = get ();

    if (is_eos (c))
    {
      cerr << id_ << ':' << c.line () << ':' << c.column () << ": error: "
           << "end of stream reached while reading template expression"
           << endl;
      throw InvalidInput ();
    }

    lexeme += c;

    switch (c)
    {
    case '<':
      {
        balance++;
        break;
      }
    case '>':
      {
        balance--;
        break;
      }
    }
  }

  return Token (Token::t_template_expr, lexeme, ln, cl);
}
