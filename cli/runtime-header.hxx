// file      : cli/runtime-header.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_RUNTIME_HEADER_HXX
#define CLI_RUNTIME_HEADER_HXX

#include "context.hxx"

void
generate_runtime_header_decl (context&);

void
generate_runtime_header_impl (context&);

#endif // CLI_RUNTIME_HEADER_HXX
