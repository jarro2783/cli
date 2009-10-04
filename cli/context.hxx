// file      : cli/context.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_CONTEXT_HXX
#define CLI_CONTEXT_HXX

#include <map>
#include <string>
#include <ostream>
#include <cstddef> // std::size_t

#include <cutl/shared-ptr.hxx>

#include "options.hxx"
#include "semantics.hxx"
#include "traversal.hxx"

using std::endl;

class context
{
public:
  typedef std::size_t size_t;
  typedef std::string string;
  typedef ::options options_type;

private:
  struct data;
  cutl::shared_ptr<data> data_;

public:
  std::ostream& os;
  semantics::cli_unit& unit;
  options_type const& options;

  string& inl;

  typedef std::map<string, string> reserved_name_map_type;
  reserved_name_map_type& reserved_name_map;

private:
  struct data
  {
    string inl_;

    reserved_name_map_type reserved_name_map_;
  };

public:
  // Escape C++ keywords, reserved names, and illegal characters.
  //
  string
  escape (string const&) const;

public:
  static string const&
  ename (semantics::nameable& n)
  {
    return n.context ().get<string> ("name");
  }

  static string const&
  emember (semantics::nameable& n)
  {
    return n.context ().get<string> ("member");
  }

public:
  context (std::ostream&, semantics::cli_unit&, options_type const&);

  context (context&);

private:
  context&
  operator= (context const&);
};

// Standard namespace traverser.
//
struct namespace_: traversal::namespace_, context
{
  namespace_ (context& c) : context (c) {}

  virtual void
  pre (type&);

  virtual void
  post (type&);
};

#endif // CLI_CONTEXT_HXX
