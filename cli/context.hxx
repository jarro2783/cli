// file      : cli/context.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_CONTEXT_HXX
#define CLI_CONTEXT_HXX

#include <ostream>

#include <cutl/shared-ptr.hxx>

#include "semantics.hxx"
#include "traversal.hxx"

using std::endl;

class context
{
private:
  struct data
  {
  };

  cutl::shared_ptr<data> data_;

public:
  std::ostream& os;

public:
  context (std::ostream& os_);
  context (context& c);

private:
  context&
  operator= (context const&);
};

#endif // CLI_CONTEXT_HXX
