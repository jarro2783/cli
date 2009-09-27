// file      : cli/runtime-header.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "runtime-header.hxx"

using namespace std;

void
generate_runtime_header_decl (context& ctx)
{
  ostream& os (ctx.os);

  os << "#include <iosfwd>" << endl
     << "#include <string>" << endl
     << "#include <exception>" << endl
     << endl;

  os << "namespace cli"
     << "{";

  // unknown_mode
  //
  os << "class unknown_mode"
     << "{"
     << "public:"
     << "enum value"
     << "{"
     << "skip," << endl
     << "stop," << endl
     << "fail" << endl
     << "};"
     << "unknown_mode (value v)" << endl
     << ": v_ (v) {}"
     << "operator value () const {return v_;}"
     << "private:" << endl
     << "value v_;"
     << "};";

  // Exceptions.
  //

  os << "// Exceptions." << endl
     << "//" << endl
     << endl;

  os << "class exception: public std::exception"
     << "{"
     << "public:" << endl
     << "virtual void" << endl
     << "print (std::ostream&) const = 0;"
     << "};";

  os << "inline std::ostream&" << endl
     << "operator<< (std::ostream& os, const exception& e)"
     << "{"
     << "e.print (os);"
     << "return os;"
     << "}";

  os << "class unknown_option: public exception"
     << "{"
     << "public:" << endl
     << "virtual" << endl
     << "~unknown_option () throw ();"
     << endl
     << "unknown_option (const std::string& option)" << endl
     << ": option_ (option)"
     << "{"
     << "}"
     << "const std::string&" << endl
     << "option () const"
     << "{"
     << "return option_;"
     << "}"
     << "virtual void" << endl
     << "print (std::ostream&) const;"
     << endl
     << "virtual const char*" << endl
     << "what () const throw ();"
     << endl
     << "private:" << endl
     << "std::string option_;"
     << "};";

  os << "class unknown_argument: public exception"
     << "{"
     << "public:" << endl
     << "virtual" << endl
     << "~unknown_argument () throw ();"
     << endl
     << "unknown_argument (const std::string& argument)" << endl
     << ": argument_ (argument)"
     << "{"
     << "}"
     << "const std::string&" << endl
     << "argument () const"
     << "{"
     << "return argument_;"
     << "}"
     << "virtual void" << endl
     << "print (std::ostream&) const;"
     << endl
     << "virtual const char*" << endl
     << "what () const throw ();"
     << endl
     << "private:" << endl
     << "std::string argument_;"
     << "};";

  os << "class missing_value: public exception"
     << "{"
     << "public:" << endl
     << "virtual" << endl
     << "~missing_value () throw ();"
     << endl
     << "missing_value (const std::string& option)" << endl
     << ": option_ (option)"
     << "{"
     << "}"
     << "const std::string&" << endl
     << "option () const"
     << "{"
     << "return option_;"
     << "}"
     << "virtual void" << endl
     << "print (std::ostream&) const;"
     << endl
     << "virtual const char*" << endl
     << "what () const throw ();"
     << endl
     << "private:" << endl
     << "std::string option_;"
     << "};";

  os << "class invalid_value: public exception"
     << "{"
     << "public:" << endl
     << "virtual" << endl
     << "~invalid_value () throw ();"
     << endl
     << "invalid_value (const std::string& option," << endl
     << "const std::string& value)" << endl
     << ": option_ (option),"
     << "  value_ (value)"
     << "{"
     << "}"
     << "const std::string&" << endl
     << "option () const"
     << "{"
     << "return option_;"
     << "}"
     << "const std::string&" << endl
     << "value () const"
     << "{"
     << "return value_;"
     << "}"
     << "virtual void" << endl
     << "print (std::ostream&) const;"
     << endl
     << "virtual const char*" << endl
     << "what () const throw ();"
     << endl
     << "private:" << endl
     << "std::string option_;"
     << "std::string value_;"
     << "};";

  os << "}"; // namespace cli
}

void
generate_runtime_header_impl (context& ctx)
{
  ostream& os (ctx.os);
}
