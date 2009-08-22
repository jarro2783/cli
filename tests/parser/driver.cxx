// file      : tests/parser/driver.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <fstream>
#include <iostream>

#include <cli/parser.hxx>

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

    Parser parser;
    parser.parse (ifs, argv[1]);
  }
  catch (Parser::InvalidInput const&)
  {
    return 1;
  }
}
