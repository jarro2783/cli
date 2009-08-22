// file      : cli/parser.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_PARSER_HXX
#define CLI_PARSER_HXX

#include <string>
#include <istream>

class token;
class lexer;

class parser
{
public:
  struct invalid_input {};

  void
  parse (std::istream& is, std::string const& id);

private:
  struct error {};

  void
  def_unit ();

  void
  include_decl ();

  bool
  decl (token&);

  void
  namespace_def ();

  void
  class_def ();

  bool
  option_def (token&);

  bool
  qualified_name (token&);

  bool
  fundamental_type (token&);

private:
  void
  recover (token& t);

private:
  bool valid_;
  lexer* lexer_;
  std::string const* id_;
};

#endif // CLI_PARSER_HXX
