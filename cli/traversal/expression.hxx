// file      : cli/traversal/expression.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_TRAVERSAL_EXPRESSION_HXX
#define CLI_TRAVERSAL_EXPRESSION_HXX

#include <traversal/elements.hxx>
#include <semantics/expression.hxx>

namespace traversal
{
  struct expression: node<semantics::expression> {};
}

#endif // CLI_TRAVERSAL_EXPRESSION_HXX
