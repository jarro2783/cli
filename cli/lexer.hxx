// file      : cli/lexer.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_LEXER_HXX
#define CLI_LEXER_HXX

#include <map>
#include <string>
#include <locale>
#include <cstddef> // std::size_t
#include <istream>

#include "token.hxx"

class Lexer
{
public:
  Lexer (std::istream& is, std::string const& id);

  Token
  next ();

protected:
  class Char
  {
  public:
    typedef std::char_traits<char> Traits;
    typedef Traits::int_type IntType;
    typedef Traits::char_type CharType;

    Char (IntType v, std::size_t l, std::size_t c);

    operator CharType () const;

    IntType
    value () const;

    std::size_t
    line () const;

    std::size_t
    column () const;

  private:
    IntType v_;
    std::size_t l_;
    std::size_t c_;
  };

  Char
  get ();

  Char
  peek ();

protected:
  class invalid_input {};

  void
  skip_spaces ();

  Token
  identifier (Char);

  Token
  int_literal (Char,
               bool neg = false,
               std::size_t ml = 0,
               std::size_t mc = 0);

  Token
  char_literal (Char);

  Token
  string_literal (Char);

  std::string
  string_literal_trailer ();

  Token
  path_literal (Char);

protected:
  bool
  is_alpha (char c) const;

  bool
  is_oct_digit (char c) const;

  bool
  is_dec_digit (char c) const;

  bool
  is_hex_digit (char c) const;

  bool
  is_alnum (char c) const;

  bool
  is_space (char c) const;

  bool
  is_eos (Char const& c) const;

  char
  to_upper (char c) const;

private:
  typedef std::map<std::string, Token::Keyword> KeywordMap;

  std::locale loc_;
  std::istream& is_;
  std::string id_;
  std::size_t l_;
  std::size_t c_;

  KeywordMap keyword_map_;

  bool eos_;
  bool include_;
};

#include "lexer.ixx"

#endif // CLI_LEXER_HXX
