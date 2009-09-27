// file      : examples/hello/driver.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <iostream>

#include "hello.hxx"

using namespace std;

void
usage ()
{
  cerr << "usage: driver <options> <names>" << endl
       << "  [--help]" << endl
       << "  [--greeting|-g <string>]" << endl
       << "  [--exclamations|-e <integer>]" << endl;
}

int
main (int argc, char* argv[])
{
  try
  {
    int end; // End of options.
    options o (argc, argv, end);

    if (o.help ())
    {
      usage ();
      return 0;
    }

    if (end == argc)
    {
      cerr << "no names provided" << endl;
      usage ();
      return 1;
    }

    for (int i = end; i < argc; i++)
    {
      cout << o.greeting () << ", " << argv[i];

      for (unsigned int j = 0; j < o.exclamations (); j++)
        cout << '!';

      cout << endl;
    }
  }
  catch (const cli::exception& e)
  {
    cerr << e << endl;
    usage ();
    return 1;
  }
}
