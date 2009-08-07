// file      : cli/token.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_TOKEN_HXX
#define CLI_TOKEN_HXX

#include <string>
#include <cstddef>

class Token
{
public:
  enum Type
  {
    t_eos,
    t_keyword,
    t_identifier,
    t_punctuation,
    t_bracket_lit,
    t_string_lit,
    t_bool_lit,
    t_int_lit,
    t_float_lit
  };

  Type
  type () const
  {
    return type_;
  }

  size_t
  line () const
  {
    return l_;
  }

  size_t
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
    k_bool
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
  Token (size_t l, size_t c)
      : l_ (l), c_ (c), type_ (t_eos)
  {
  }

  Token (Keyword k, size_t l, size_t c)
      : l_ (l), c_ (c), type_ (t_keyword), keyword_ (k)
  {
  }

  Token (Punctuation p, size_t l, size_t c)
      : l_ (l), c_ (c), type_ (t_punctuation), punctuation_ (p)
  {
  }

  // Identifier & literals.
  //
  Token (Type t, std::string const& s, size_t l, size_t c)
      : l_ (l), c_ (c), type_ (t), str_ (s)
  {
  }

private:
  size_t l_;
  size_t c_;

  Type type_;

  Keyword keyword_;
  Punctuation punctuation_;
  std::string str_;
};

#endif // CLI_TOKEN_HXX
