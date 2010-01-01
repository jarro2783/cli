// file      : cli/runtime-inline.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009-2010 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "runtime-inline.hxx"

using namespace std;

void
generate_runtime_inline (context& ctx)
{
  ostream& os (ctx.os);
  string const& inl (ctx.inl);

  os << "namespace cli"
     << "{";

  // unknown_mode
  //
  os << "// unknown_mode" << endl
     << "//" << endl

     << inl << "unknown_mode::" << endl
     << "unknown_mode (value v)" << endl
     << ": v_ (v)"
     << "{"
     << "}";

  // exception
  //
  os << "// exception" << endl
     << "//" << endl

     << inl << "std::ostream&" << endl
     << "operator<< (std::ostream& os, const exception& e)"
     << "{"
     << "e.print (os);"
     << "return os;"
     << "}";

  // unknown_option
  //
  os << "// unknown_option" << endl
     << "//" << endl

     << inl << "unknown_option::" << endl
     << "unknown_option (const std::string& option)" << endl
     << ": option_ (option)"
     << "{"
     << "}"

     << inl << "const std::string& unknown_option::" << endl
     << "option () const"
     << "{"
     << "return option_;"
     << "}";

  // unknown_argument
  //
  os << "// unknown_argument" << endl
     << "//" << endl

     << inl << "unknown_argument::" << endl
     << "unknown_argument (const std::string& argument)" << endl
     << ": argument_ (argument)"
     << "{"
     << "}"

     << inl << "const std::string& unknown_argument::" << endl
     << "argument () const"
     << "{"
     << "return argument_;"
     << "}";

  // missing_value
  //
  os << "// missing_value" << endl
     << "//" << endl

     << inl << "missing_value::" << endl
     << "missing_value (const std::string& option)" << endl
     << ": option_ (option)"
     << "{"
     << "}"

     << inl << "const std::string& missing_value::" << endl
     << "option () const"
     << "{"
     << "return option_;"
     << "}";

  // invalid_value
  //
  os << "// invalid_value" << endl
     << "//" << endl

     << inl << "invalid_value::" << endl
     << "invalid_value (const std::string& option," << endl
     << "const std::string& value)" << endl
     << ": option_ (option),"
     << "  value_ (value)"
     << "{"
     << "}"

     << inl << "const std::string& invalid_value::" << endl
     << "option () const"
     << "{"
     << "return option_;"
     << "}"

     << inl << "const std::string& invalid_value::" << endl
     << "value () const"
     << "{"
     << "return value_;"
     << "}";

  // file_io_failure
  //
  if (ctx.options.generate_file_scanner ())
  {
    os << "// file_io_failure" << endl
       << "//" << endl

       << inl << "file_io_failure::" << endl
       << "file_io_failure (const std::string& file)" << endl
       << ": file_ (file)"
       << "{"
       << "}"

       << inl << "const std::string& file_io_failure::" << endl
       << "file () const"
       << "{"
       << "return file_;"
       << "}";
  }

  // argv_scanner
  //
  os << "// argv_scanner" << endl
     << "//" << endl;

  os << inl << "argv_scanner::" << endl
     << "argv_scanner (int& argc, char** argv, bool erase)" << endl
     << ": i_ (1), argc_ (argc), argv_ (argv), erase_ (erase)"
     << "{"
     << "}";

  os << inl << "argv_scanner::" << endl
     << "argv_scanner (int start, int& argc, char** argv, bool erase)" << endl
     << ": i_ (start), argc_ (argc), argv_ (argv), erase_ (erase)"
     << "{"
     << "}";

  os << inl << "int argv_scanner::" << endl
     << "end () const"
     << "{"
     << "return i_;"
     << "}";

  // argv_file_scanner
  //
  if (ctx.options.generate_file_scanner ())
  {
    bool sep (!ctx.opt_sep.empty ());

    os << "// argv_file_scanner" << endl
       << "//" << endl;

    os << inl << "argv_file_scanner::" << endl
       << "argv_file_scanner (int& argc," << endl
       << "char** argv," << endl
       << "const std::string& option," << endl
       << "bool erase)" << endl
       << ": argv_scanner (argc, argv, erase)," << endl
       << "  option_ (option)";
    if (sep)
      os << "," << endl
         << "  skip_ (false)";
    os << "{"
       << "}";

    os << inl << "argv_file_scanner::" << endl
       << "argv_file_scanner (int start," << endl
       << "int& argc," << endl
       << "char** argv," << endl
       << "const std::string& option," << endl
       << "bool erase)" << endl
       << ": argv_scanner (start, argc, argv, erase)," << endl
       << "  option_ (option)";
    if (sep)
      os << "," << endl
         << "  skip_ (false)";
    os << "{"
       << "}";
  }

  os << "}"; // namespace cli
}
