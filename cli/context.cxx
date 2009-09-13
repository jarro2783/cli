// file      : cli/context.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "context.hxx"

context::
context (std::ostream& os_)
    : data_ (new (shared) data),
      os (os_)
{
}

context::
context (context& c)
    : data_ (c.data_),
      os (c.os)
{
}
