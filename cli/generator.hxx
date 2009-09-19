// file      : cli/generator.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_GENERATOR_HXX
#define CLI_GENERATOR_HXX

#include <semantics/unit.hxx>

class generator
{
public:
  generator ();

  class failed {};

  void
  generate (semantics::cli_unit&, semantics::path const&);

private:
  generator (generator const&);

  generator&
  operator= (generator const&);
};

#endif // CLI_GENERATOR_HXX
