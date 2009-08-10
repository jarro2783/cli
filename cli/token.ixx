// file      : cli/token.ixx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

inline Token::Type Token::
type () const
{
  return type_;
}

inline std::size_t Token::
line () const
{
  return l_;
}

inline std::size_t Token::
column () const
{
  return c_;
}

inline Token::Keyword Token::
keyword () const
{
  return keyword_;
}

inline std::string const& Token::
identifier () const
{
  return str_;
}

inline Token::Punctuation Token::
punctuation () const
{
  return punctuation_;
}

inline std::string const& Token::
literal () const
{
  return str_;
}

inline Token::
Token (std::size_t l, std::size_t c)
    : l_ (l), c_ (c), type_ (t_eos)
{
}

inline Token::
Token (Keyword k, std::size_t l, std::size_t c)
    : l_ (l), c_ (c), type_ (t_keyword), keyword_ (k)
{
}

inline Token::
Token (Punctuation p, std::size_t l, std::size_t c)
    : l_ (l), c_ (c), type_ (t_punctuation), punctuation_ (p)
{
}

inline Token::
Token (Type t, std::string const& s, std::size_t l, std::size_t c)
    : l_ (l), c_ (c), type_ (t), str_ (s)
{
}
