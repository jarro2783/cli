// file      : cli/lexer.ixx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

// Lexer::Char
//
inline Lexer::Char::
Char (IntType v, std::size_t l, std::size_t c)
    : v_ (v), l_ (l), c_ (c)
{
}

inline Lexer::Char::
operator CharType () const
{
  return Traits::to_char_type (v_);
}

inline Lexer::Char::IntType Lexer::Char::
value () const
{
  return v_;
}

inline std::size_t Lexer::Char::
line () const
{
  return l_;
}

inline std::size_t Lexer::Char::
column () const
{
  return c_;
}

// Lexer
//
inline bool Lexer::
is_alpha (char c) const
{
  return std::isalpha (c, loc_);
}

inline bool Lexer::
is_oct_digit (char c) const
{
  return std::isdigit (c, loc_) && c != '8' && c != '9';
}

inline bool Lexer::
is_dec_digit (char c) const
{
  return std::isdigit (c, loc_);
}

inline bool Lexer::
is_hex_digit (char c) const
{
  return std::isxdigit (c, loc_);
}

inline bool Lexer::
is_alnum (char c) const
{
  return std::isalnum (c, loc_);
}

inline bool Lexer::
is_space (char c) const
{
  return std::isspace (c, loc_);
}

inline bool Lexer::
is_eos (Char const& c) const
{
  return  c.value () == Char::Traits::eof ();
}

inline char Lexer::
to_upper (char c) const
{
  return std::toupper (c, loc_);
}
