// file      : cli/inline.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "inline.hxx"

namespace
{

}

void
generate_inline (context& ctx)
{
  traversal::cli_unit unit;
  traversal::names unit_names;
  namespace_ ns (ctx);
  traversal::names ns_names;

  unit >> unit_names >> ns >> ns_names >> ns;

  unit.dispatch (ctx.unit);
}
