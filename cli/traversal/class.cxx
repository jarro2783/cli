// file      : cli/traversal/class.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <traversal/class.hxx>

namespace traversal
{
  void class_::
  traverse (type& c)
  {
    pre (c);
    names (c);
    post (c);
  }

  void class_::
  pre (type&)
  {
  }

  void class_::
  post (type&)
  {
  }
}
