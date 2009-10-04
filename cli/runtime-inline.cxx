// file      : cli/runtime-inline.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
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

  os << "// unknown_mode" << endl
     << "//" << endl;

  os << inl << "unknown_mode::" << endl
     << "unknown_mode (value v)" << endl
     << ": v_ (v)"
     << "{"
     << "}";

  os << "// exception" << endl
     << "//" << endl;

  os << inl << "std::ostream&" << endl
     << "operator<< (std::ostream& os, const exception& e)"
     << "{"
     << "e.print (os);"
     << "return os;"
     << "}";

  os << "// unknown_option" << endl
     << "//" << endl;

  os << inl << "unknown_option::" << endl
     << "unknown_option (const std::string& option)" << endl
     << ": option_ (option)"
     << "{"
     << "}";

  os << inl << "const std::string& unknown_option::" << endl
     << "option () const"
     << "{"
     << "return option_;"
     << "}";

  os << "// unknown_argument" << endl
     << "//" << endl;

  os << inl << "unknown_argument::" << endl
     << "unknown_argument (const std::string& argument)" << endl
     << ": argument_ (argument)"
     << "{"
     << "}";

  os << inl << "const std::string& unknown_argument::" << endl
     << "argument () const"
     << "{"
     << "return argument_;"
     << "}";

  os << "// missing_value" << endl
     << "//" << endl;

  os << inl << "missing_value::" << endl
     << "missing_value (const std::string& option)" << endl
     << ": option_ (option)"
     << "{"
     << "}";

  os << inl << "const std::string& missing_value::" << endl
     << "option () const"
     << "{"
     << "return option_;"
     << "}";

  os << "// invalid_value" << endl
     << "//" << endl;

  os << inl << "invalid_value::" << endl
     << "invalid_value (const std::string& option," << endl
     << "const std::string& value)" << endl
     << ": option_ (option),"
     << "  value_ (value)"
     << "{"
     << "}";

  os << inl << "const std::string& invalid_value::" << endl
     << "option () const"
     << "{"
     << "return option_;"
     << "}";

  os << inl << "const std::string& invalid_value::" << endl
     << "value () const"
     << "{"
     << "return value_;"
     << "}";

  os << "}"; // namespace cli
}
