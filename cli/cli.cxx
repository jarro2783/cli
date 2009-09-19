// file      : cli/cli.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <memory>   // std::auto_ptr
#include <fstream>
#include <iostream>

#include "parser.hxx"
#include "generator.hxx"

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
    semantics::path path (argv[1]);

    ifstream ifs (path.string ().c_str ());
    if (!ifs.is_open ())
    {
      cerr << path << ": error: unable to open in read mode" << endl;
      return 1;
    }

    ifs.exceptions (ifstream::failbit | ifstream::badbit);

    parser p;
    auto_ptr<semantics::cli_unit> unit (p.parse (ifs, path));

    generator g;
    g.generate (*unit, path);
  }
  catch (semantics::invalid_path const&)
  {
    cerr << "error: '" << argv[1] << "' is not a valid filesystem path"
         << endl;
    return 1;
  }
  catch (std::ios_base::failure const&)
  {
    cerr << argv[1] << ": error: read failure" << endl;
    return 1;
  }
  catch (parser::invalid_input const&)
  {
    // Diagnostics has already been issued by the parser.
    //
    return 1;
  }
  catch (generator::failed const&)
  {
    // Diagnostics has already been issued by the generator.
    //
    return 1;
  }
}
