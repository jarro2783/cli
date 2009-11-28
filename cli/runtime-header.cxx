// file      : cli/runtime-header.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "runtime-header.hxx"

using namespace std;

void
generate_runtime_header (context& ctx)
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
     << "public:" << endl
     << "enum value"
     << "{"
     << "skip," << endl
     << "stop," << endl
     << "fail" << endl
     << "};"
     << "unknown_mode (value v);"
     << endl
     << "operator value () const {return v_;}" // Can't generate outside.
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

  os << "std::ostream&" << endl
     << "operator<< (std::ostream&, const exception&);"
     << endl;

  os << "class unknown_option: public exception"
     << "{"
     << "public:" << endl
     << "virtual" << endl
     << "~unknown_option () throw ();"
     << endl
     << "unknown_option (const std::string& option);"
     << endl
     << "const std::string&" << endl
     << "option () const;"
     << endl
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
     << "unknown_argument (const std::string& argument);"
     << endl
     << "const std::string&" << endl
     << "argument () const;"
     << endl
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
     << "missing_value (const std::string& option);"
     << endl
     << "const std::string&" << endl
     << "option () const;"
     << endl
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
     << "const std::string& value);"
     << endl
     << "const std::string&" << endl
     << "option () const;"
     << endl
     << "const std::string&" << endl
     << "value () const;"
     << endl
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

  os << "class eos_reached: public exception"
     << "{"
     << "public:" << endl
     << "virtual void" << endl
     << "print (std::ostream&) const;"
     << endl
     << "virtual const char*" << endl
     << "what () const throw ();"
     << "};";

  // scanner
  //
  os << "class scanner"
     << "{"
     << "public:" << endl
     << "virtual" << endl
     << "~scanner ();"
     << endl
     << "virtual bool" << endl
     << "more () = 0;"
     << endl
     << "virtual const char*" << endl
     << "peek () = 0;"
     << endl
     << "virtual const char*" << endl
     << "next () = 0;"
     << endl
     << "virtual void" << endl
     << "skip () = 0;"
     << "};";

  // argv_scanner
  //
  os << "class argv_scanner: public scanner"
     << "{"
     << "public:" << endl
     << "argv_scanner (int argc, char** argv);"
     << "argv_scanner (int start, int argc, char** argv);"
     << endl
     << "int" << endl
     << "end () const;"
     << endl
     << "virtual bool" << endl
     << "more ();"
     << endl
     << "virtual const char*" << endl
     << "peek ();"
     << endl
     << "virtual const char*" << endl
     << "next ();"
     << endl
     << "virtual void" << endl
     << "skip ();"
     << endl
     << "private:"
     << "int i_;"
     << "int argc_;"
     << "char** argv_;"
     << "};";

  os << "}"; // namespace cli
}
