// file      : cli/runtime-source.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "runtime-source.hxx"

using namespace std;

void
generate_runtime_source (context& ctx)
{
  ostream& os (ctx.os);

  os << "#include <map>" << endl
     << "#include <set>" << endl
     << "#include <string>" << endl
     << "#include <vector>" << endl
     << "#include <ostream>" << endl
     << "#include <sstream>" << endl
     << endl;

  os << "namespace cli"
     << "{";

  // unknown_option
  //
  os << "// unknown_option" << endl
     << "//" << endl
     << "unknown_option::" << endl
     << "~unknown_option () throw ()"
     << "{"
     << "}"
     << "void unknown_option::" << endl
     << "print (std::ostream& os) const"
     << "{"
     << "os << \"unknown option '\" << option () << \"'\";"
     << "}"
     << "const char* unknown_option::" << endl
     << "what () const throw ()"
     << "{"
     << "return \"unknown option\";"
     << "}";

  // unknown_argument
  //
  os << "// unknown_argument" << endl
     << "//" << endl
     << "unknown_argument::" << endl
     << "~unknown_argument () throw ()"
     << "{"
     << "}"
     << "void unknown_argument::" << endl
     << "print (std::ostream& os) const"
     << "{"
     << "os << \"unknown argument '\" << argument () << \"'\";"
     << "}"
     << "const char* unknown_argument::" << endl
     << "what () const throw ()"
     << "{"
     << "return \"unknown argument\";"
     << "}";

  // missing_value
  //
  os << "// missing_value" << endl
     << "//" << endl
     << "missing_value::" << endl
     << "~missing_value () throw ()"
     << "{"
     << "}"
     << "void missing_value::" << endl
     << "print (std::ostream& os) const"
     << "{"
     << "os << \"missing value for option '\" << option () << \"'\";"
     << "}"
     << "const char* missing_value::" << endl
     << "what () const throw ()"
     << "{"
     << "return \"missing option value\";"
     << "}";

  // invalid_value
  //
  os << "// invalid_value" << endl
     << "//" << endl
     << "invalid_value::" << endl
     << "~invalid_value () throw ()"
     << "{"
     << "}"
     << "void invalid_value::" << endl
     << "print (std::ostream& os) const"
     << "{"
     << "os << \"invalid value '\" << value () << \"' for option '\"" << endl
     << "   << option () << \"'\";"
     << "}"
     << "const char* invalid_value::" << endl
     << "what () const throw ()"
     << "{"
     << "return \"invalid option value\";"
     << "}";

  // eos_reached
  //
  os << "// eos_reached" << endl
     << "//" << endl
     << "void eos_reached::" << endl
     << "print (std::ostream& os) const"
     << "{"
     << "os << what ();"
     << "}"
     << "const char* eos_reached::" << endl
     << "what () const throw ()"
     << "{"
     << "return \"end of argument stream reached\";"
     << "}";

  // scanner
  //
  os << "// scanner" << endl
     << "//" << endl
     << "scanner::" << endl
     << "~scanner ()"
     << "{"
     << "}";

  // argv_scanner
  //
  os << "// argv_scanner" << endl
     << "//" << endl

     << "bool argv_scanner::" << endl
     << "more ()"
     << "{"
     << "return i_ < argc_;"
     << "}"

     << "const char* argv_scanner::" << endl
     << "peek ()"
     << "{"
     << "if (i_ < argc_)" << endl
     << "return argv_[i_];"
     << "else" << endl
     << "throw eos_reached ();"
     << "}"

     << "const char* argv_scanner::" << endl
     << "next ()"
     << "{"
     << "if (i_ < argc_)" << endl
     << "return argv_[i_++];"
     << "else" << endl
     << "throw eos_reached ();"
     << "}"

     << "void argv_scanner::" << endl
     << "skip ()"
     << "{"
     << "if (i_ < argc_)" << endl
     << "i_++;"
     << "else" << endl
     << "throw eos_reached ();"
     << "}";

  // parser class template & its specializations
  //
  os << "template <typename X>" << endl
     << "struct parser"
     << "{"
     << "static void" << endl
     << "parse (X& x, scanner& s)"
     << "{"
     << "const char* o (s.next ());"
     << endl
     << "if (s.more ())"
     << "{"
     << "const char* v (s.next ());"
     << "std::istringstream is (v);"
     << "if (!(is >> x && is.eof ()))" << endl
     << "throw invalid_value (o, v);"
     << "}"
     << "else" << endl
     << "throw missing_value (o);"
     << "}"
     << "};";

  // parser<bool>
  //
  os << "template <>" << endl
     << "struct parser<bool>"
     << "{"
     << "static void" << endl
     << "parse (bool& x, scanner& s)"
     << "{"
     << "s.next ();"
     << "x = true;"
     << "}"
     << "};";

  // parser<string>
  //
  os << "template <>" << endl
     << "struct parser<std::string>"
     << "{"
     << "static void" << endl
     << "parse (std::string& x, scanner& s)"
     << "{"
     << "const char* o (s.next ());"
     << endl
     << "if (s.more ())" << endl
     << "x = s.next ();"
     << "else" << endl
     << "throw missing_value (o);"
     << "}"
     << "};";

  // parser<std::vector<X>>
  //
  os << "template <typename X>" << endl
     << "struct parser<std::vector<X> >"
     << "{"
     << "static void" << endl
     << "parse (std::vector<X>& c, scanner& s)"
     << "{"
     << "X x;"
     << "parser<X>::parse (x, s);"
     << "c.push_back (x);"
     << "}"
     << "};";

  // parser<std::set<X>>
  //
  os << "template <typename X>" << endl
     << "struct parser<std::set<X> >"
     << "{"
     << "static void" << endl
     << "parse (std::set<X>& c, scanner& s)"
     << "{"
     << "X x;"
     << "parser<X>::parse (x, s);"
     << "c.insert (x);"
     << "}"
     << "};";

  // parser<std::map<K,V>>
  //
  os << "template <typename K, typename V>" << endl
     << "struct parser<std::map<K, V> >"
     << "{"
     << "static void" << endl
     << "parse (std::map<K, V>& m, scanner& s)"
     << "{"
     << "const char* o (s.next ());"
     << endl
     << "if (s.more ())"
     << "{"
     << "std::string ov (s.next ());"
     << "std::string::size_type p = ov.find ('=');"
     << endl
     << "if (p == std::string::npos)"
     << "{"
     << "K k = K ();"
     << endl
     << "if (!ov.empty ())"
     << "{"
     << "std::istringstream ks (ov);"
     << endl
     << "if (!(ks >> k && ks.eof ()))" << endl
     << "throw invalid_value (o, ov);"
     << "}"
     << "m[k] = V ();"
     << "}"
     << "else"
     << "{"
     << "K k = K ();"
     << "V v = V ();"
     << "std::string kstr (ov, 0, p);"
     << "std::string vstr (ov, p + 1);"
     << endl
     << "if (!kstr.empty ())"
     << "{"
     << "std::istringstream ks (kstr);"
     << endl
     << "if (!(ks >> k && ks.eof ()))" << endl
     << "throw invalid_value (o, ov);"
     << "}"
     << "if (!vstr.empty ())"
     << "{"
     << "std::istringstream vs (vstr);"
     << endl
     << "if (!(vs >> v && vs.eof ()))" << endl
     << "throw invalid_value (o, ov);"
     << "}"
     << "m[k] = v;"
     << "}"
     << "}"
     << "else" << endl
     << "throw missing_value (o);"
     << "}"
     << "};";

  // Parser thunk.
  //
  os << "template <typename X, typename T, T X::*P>" << endl
     << "void" << endl
     << "thunk (X& x, scanner& s)"
     << "{"
     << "parser<T>::parse (x.*P, s);"
     << "}";

  os << "}"; // namespace cli
}
