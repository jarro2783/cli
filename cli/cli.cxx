// file      : cli/cli.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <fstream>
#include <iostream>

#include "parser.hxx"

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
    ifstream ifs (argv[1]);
    if (!ifs.is_open ())
    {
      wcerr << argv[1] << ": error: unable to open in read mode" << endl;
      return 1;
    }

    ifs.exceptions (ifstream::failbit | ifstream::badbit);

    parser p;
    p.parse (ifs, argv[1]);
  }
  catch (std::ios_base::failure const&)
  {
    wcerr << argv[1] << ": error: read failure" << endl;
    return 1;
  }
  catch (parser::invalid_input const&)
  {
    // Diagnostics has already been issued by the parser.
    //
    return 1;
  }
}
