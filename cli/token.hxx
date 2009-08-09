// file      : cli/token.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_TOKEN_HXX
#define CLI_TOKEN_HXX

#include <string>
#include <cstddef> // std::size_t

class Token
{
public:
  enum Type
  {
    t_eos,
    t_keyword,
    t_identifier,
    t_punctuation,
    t_path_lit,
    t_string_lit,
    t_char_lit,
    t_bool_lit,
    t_int_lit,
    t_float_lit
  };

  Type
  type () const
  {
    return type_;
  }

  std::size_t
  line () const
  {
    return l_;
  }

  std::size_t
  column () const
  {
    return c_;
  }

  // Keyword
  //
public:
  enum Keyword
  {
    k_include,
    k_namespace,
    k_class,
    k_signed,
    k_unsigned,
    k_bool,
    k_char,
    k_wchar,
    k_short,
    k_int,
    k_long,
    k_float,
    k_double
  };

  Keyword
  keyword () const
  {
    return keyword_;
  }

  // Identifier
  //
public:
  std::string const&
  identifier () const
  {
    return str_;
  }

  // Punctuation
  //
public:
  enum Punctuation
  {
    p_semi,
    p_comma,
    p_colon,
    p_lcbrace,
    p_rcbrace,
    p_lparen,
    p_rparen,
    p_eq,
    p_or
  };

  Punctuation
  punctuation () const
  {
    return punctuation_;
  }

  // Literals.
  //
public:
  std::string const&
  literal () const
  {
    return str_;
  }

  // C-tors.
  //
public:
  // EOS.
  //
  Token (std::size_t l, std::size_t c)
      : l_ (l), c_ (c), type_ (t_eos)
  {
  }

  Token (Keyword k, std::size_t l, std::size_t c)
      : l_ (l), c_ (c), type_ (t_keyword), keyword_ (k)
  {
  }

  Token (Punctuation p, std::size_t l, std::size_t c)
      : l_ (l), c_ (c), type_ (t_punctuation), punctuation_ (p)
  {
  }

  // Identifier & literals.
  //
  Token (Type t, std::string const& s, std::size_t l, std::size_t c)
      : l_ (l), c_ (c), type_ (t), str_ (s)
  {
  }

private:
  std::size_t l_;
  std::size_t c_;

  Type type_;

  Keyword keyword_;
  Punctuation punctuation_;
  std::string str_;
};

#endif // CLI_TOKEN_HXX
