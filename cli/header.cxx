// file      : cli/header.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "header.hxx"

namespace
{
  //
  //
  struct option: traversal::option, context
  {
    option (context& c) : context (c) {}

    virtual void
    traverse (type& o)
    {
      string name (ename (o));
      string type (o.type ().name ());

      os << "const " << type << "&" << endl
         << name << " () const;"
         << endl;

      if (modifier)
        os << "void" << endl
           << name << " (const " << type << "&);"
           << endl;
    }
  };

  //
  //
  struct option_data: traversal::option, context
  {
    option_data (context& c) : context (c) {}

    virtual void
    traverse (type& o)
    {
      string member (emember (o));
      string type (o.type ().name ());

      os << type << " " << member << ";";
    }
  };

  //
  //
  struct class_: traversal::class_, context
  {
    class_ (context& c)
        : context (c),
          option_ (c),
          option_data_ (c)
    {
      names_option_ >> option_;
      names_option_data_ >> option_data_;
    }

    virtual void
    traverse (type& c)
    {
      string name (escape (c.name ()));

      os << "class " << name
         << "{"
         << "public:" << endl
         << endl;

      // c-tors
      //
      string um ("::cli::unknown_mode");

      os << name << " (int argc," << endl
         << "char** argv," << endl
         << um << " option = " << um << "::fail," << endl
         << um << " argument = " << um << "::stop);"
         << endl;

      os << name << " (int start," << endl
         << "int argc," << endl
         << "char** argv," << endl
         << um << " option = " << um << "::fail," << endl
         << um << " argument = " << um << "::stop);"
         << endl;

      os << name << " (int argc," << endl
         << "char** argv," << endl
         << "int& end," << endl
         << um << " option = " << um << "::fail," << endl
         << um << " argument = " << um << "::stop);"
         << endl;

      os << name << " (int start," << endl
         << "int argc," << endl
         << "char** argv," << endl
         << "int& end," << endl
         << um << " option = " << um << "::fail," << endl
         << um << " argument = " << um << "::stop);"
         << endl;

      //
      //
      os << "// Option accessors" << (modifier ? " and modifiers." : ".") << endl
         << "//" << endl
         << endl;

      names (c, names_option_);

      // usage
      //
      if (usage)
      {
        os << "// Print usage information." << endl
           << "//" << endl
           << "static void" << endl
           << "print_usage (::std::ostream&);"
           << endl;
      }

      // _parse()
      //
      os << "private:" << endl
         << "void" << endl
         << "_parse (::cli::scanner&," << endl
         << um << " option," << endl
         << um << " argument);"
         << endl;

      // Data members.
      //
      os << "public:" << endl; //@@ tmp

      names (c, names_option_data_);

      os << "};";
    }

  private:
    option option_;
    traversal::names names_option_;

    option_data option_data_;
    traversal::names names_option_data_;
  };

  //
  //
  struct includes: traversal::cxx_includes, context
  {
    includes (context& c) : context (c) {}

    virtual void
    traverse (semantics::cxx_includes& i)
    {
      os << "#include " << i.file () << endl
         << endl;
    }
  };
}

void
generate_header (context& ctx)
{
  traversal::cli_unit unit;
  includes includes (ctx);
  traversal::names unit_names;
  namespace_ ns (ctx);
  class_ cl (ctx);

  unit >> includes;
  unit >> unit_names >> ns;
  unit_names >> cl;

  traversal::names ns_names;

  ns >> ns_names >> ns;
  ns_names >> cl;

  unit.dispatch (ctx.unit);
}
