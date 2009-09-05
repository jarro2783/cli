// file      : tests/parser/driver.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <fstream>
#include <iostream>

#include <parser.hxx>
#include <semantics.hxx>
#include <traversal.hxx>

using namespace std;

int main (int argc, char* argv[])
{
  if (argc != 2)
  {
    cerr << "usage: " << argv[0] << " file.cli" << endl;
    return 1;
  }

  try
  {
    ifstream ifs;
    ifs.exceptions (ifstream::failbit | ifstream::badbit);
    ifs.open (argv[1]);

    parser p;
    p.parse (ifs, argv[1]);
  }
  catch (parser::invalid_input const&)
  {
    return 1;
  }
}
