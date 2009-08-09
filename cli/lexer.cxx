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
      include_ (false)
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
get ()
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

Lexer::Char Lexer::
peek ()
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

          break;
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
          return Token (Token::p_colon, c.line (), c.column ());
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
          return Token (Token::p_lparen, c.line (), c.column ());
        }
      case ')':
        {
          return Token (Token::p_rparen, c.line (), c.column ());
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
            throw invalid_input ();
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
    }
    catch (invalid_input const&)
    {
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
  for (Char c (peek ()); !is_eos (c) && is_space (c); c = peek ())
    get ();
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
      throw invalid_input ();
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
      throw invalid_input ();
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
      throw invalid_input ();
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
      throw invalid_input ();
    }

    lexeme += c;

    if (c == end)
      break;
  }

  return Token (Token::t_path_lit, lexeme, ln, cl);
}
