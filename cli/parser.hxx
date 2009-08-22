// file      : cli/parser.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_PARSER_HXX
#define CLI_PARSER_HXX

#include <string>
#include <istream>

class Token;
class Lexer;

class Parser
{
public:
  struct InvalidInput {};

  void
  parse (std::istream& is, std::string const& id);

private:
  struct Error {};

  void
  def_unit ();

  void
  include_decl ();

  bool
  decl (Token&);

  void
  namespace_def ();

  void
  class_def ();

  bool
  option_def (Token&);

  bool
  qualified_name (Token&);

  bool
  fundamental_type (Token&);

private:
  void
  recover (Token& t);

private:
  bool valid_;
  Lexer* lexer_;
  std::string const* id_;
};

#endif // CLI_PARSER_HXX
