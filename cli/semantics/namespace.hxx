// file      : cli/semantics/namespace.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_SEMANTICS_NAMESPACE_HXX
#define CLI_SEMANTICS_NAMESPACE_HXX

#include <semantics/elements.hxx>

namespace semantics
{
  class namespace_: public scope
  {
  protected:
    friend class graph<node, edge>;

    namespace_ (path const& file, size_t line, size_t column)
        : node (file, line, column)
    {
    }
  };
}

#endif // CLI_SEMANTICS_NAMESPACE_HXX
