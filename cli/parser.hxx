// file      : cli/parser.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2011 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_PARSER_HXX
#define CLI_PARSER_HXX

#include <string>
#include <memory> // std::auto_ptr
#include <istream>

#include "semantics/elements.hxx"
#include "semantics/unit.hxx"

class token;
class lexer;

class parser
{
public:
  struct invalid_input {};

  std::auto_ptr<semantics::cli_unit>
  parse (std::istream& is, semantics::path const& path);

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
  qualified_name (token&, std::string& name);

  bool
  fundamental_type (token&, std::string& name);

private:
  void
  recover (token& t);

private:
  bool valid_;
  semantics::path const* path_;

  lexer* lexer_;

  semantics::cli_unit* unit_;
  semantics::scope* scope_;
};

#endif // CLI_PARSER_HXX
