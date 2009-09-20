// file      : cli/header.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "header.hxx"

namespace
{
  struct includes: traversal::cxx_includes, context
  {
    includes (context& c) : context (c) {}

    virtual void
    traverse (semantics::cxx_includes& i)
    {
      os << "#include " << i.file () << endl
         << endl;
    }
  };
}

void
generate_header (context& ctx)
{
  traversal::cli_unit unit;
  includes includes (ctx);
  traversal::names unit_names;
  namespace_ ns (ctx);
  traversal::names ns_names;

  unit >> includes;
  unit >> unit_names >> ns >> ns_names >> ns;

  unit.dispatch (ctx.unit);
}
