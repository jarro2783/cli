// file      : cli/traversal/elements.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <traversal/elements.hxx>

namespace traversal
{
  void names::
  traverse (type& e)
  {
    dispatch (e.named ());
  }
}
