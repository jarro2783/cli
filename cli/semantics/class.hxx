// file      : cli/semantics/class.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_SEMANTICS_CLASS_HXX
#define CLI_SEMANTICS_CLASS_HXX

#include <semantics/elements.hxx>

namespace semantics
{
  class class_: public scope
  {
  protected:
    friend class graph<node, edge>;

    class_ (path const& file, size_t line, size_t column)
        : node (file, line, column)
    {
    }
  };
}

#endif // CLI_SEMANTICS_CLASS_HXX
