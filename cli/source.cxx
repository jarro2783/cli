// file      : cli/source.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include "source.hxx"

namespace
{
  //
  //
  struct option_init: traversal::option, context
  {
    option_init (context& c) : context (c), comma_ (false) {}

    virtual void
    traverse (type& o)
    {
      if (comma_)
        os << "," << endl
           << "  ";
      else
        comma_ = true;

      os << emember (o);

      if (o.initialized_p ())
      {
        using semantics::expression;
        expression const& i (o.initializer ());

        switch (i.type ())
        {
        case expression::string_lit:
        case expression::char_lit:
        case expression::bool_lit:
        case expression::int_lit:
        case expression::float_lit:
        case expression::identifier:
            {
            os << " (" << i.value () << ")";
            break;
          }
        case expression::call_expr:
          {
            os << " " << i.value ();
            break;
          }
        }
      }
      else
        os << " ()";
    }

  private:
    bool comma_;
  };

  //
  //
  struct option_map: traversal::option, context
  {
    option_map (context& c) : context (c) {}

    virtual void
    traverse (type& o)
    {
      using semantics::names;

      string member (emember (o));
      string type (o.type ().name ());
      string scope (escape (o.scope ().name ()));
      string map ("_cli_" + scope + "_map_");

      names& n (o.named ());

      for (names::name_iterator i (n.name_begin ()); i != n.name_end (); ++i)
      {
        os << "_cli_" << scope << "_map_[\"" << *i << "\"] = " << endl
           << "&::cli::thunk< " << scope << ", " << type << ", " <<
          "&" << scope << "::" << member << " >;";
      }
    }
  };

  //
  //
  struct class_: traversal::class_, context
  {
    class_ (context& c)
        : context (c), option_map_ (c)
    {
      names_option_map_ >> option_map_;
    }

    virtual void
    traverse (type& c)
    {
      string name (escape (c.name ()));

      os << "// " << name << endl
         << "//" << endl
         << endl;

      // c-tors
      //
      string um ("::cli::unknown_mode");

      os << name << "::" << endl
         << name << " (int argc," << endl
         << "char** argv," << endl
         << um << " opt," << endl
         << um << " arg)" << endl
         << ": ";
      {
        option_init init (*this);
        traversal::names names_init (init);
        names (c, names_init);
      }
      os << "{"
         << "_parse (1, argc, argv, opt, arg);"
         << "}";

      os << name << "::" << endl
         << name << " (int start," << endl
         << "int argc," << endl
         << "char** argv," << endl
         << um << " opt," << endl
         << um << " arg)" << endl
         << ": ";
      {
        option_init init (*this);
        traversal::names names_init (init);
        names (c, names_init);
      }
      os << "{"
         << "_parse (start, argc, argv, opt, arg);"
         << "}";

      os << name << "::" << endl
         << name << " (int argc," << endl
         << "char** argv," << endl
         << "int& end," << endl
         << um << " opt," << endl
         << um << " arg)" << endl
         << ": ";
      {
        option_init init (*this);
        traversal::names names_init (init);
        names (c, names_init);
      }
      os << "{"
         << "end = _parse (1, argc, argv, opt, arg);"
         << "}";

      os << name << "::" << endl
         << name << " (int start," << endl
         << "int argc," << endl
         << "char** argv," << endl
         << "int& end," << endl
         << um << " opt," << endl
         << um << " arg)" << endl
         << ": ";
      {
        option_init init (*this);
        traversal::names names_init (init);
        names (c, names_init);
      }
      os << "{"
         << "end = _parse (start, argc, argv, opt, arg);"
         << "}";

      // _parse()
      //
      string map ("_cli_" + name + "_map");

      os << "typedef" << endl
         << "std::map<std::string, int (*) (" <<
        name << "&, char**, int)>" << endl
         << map << ";"
         << endl
         << "static " << map << " " << map << "_;"
         << endl;

      os << "struct " << map << "_init"
         << "{"
         << map << "_init ()"
         << "{";

      names (c, names_option_map_);

      os << "}"
         << "} " << map << "_init_;"
         << endl;

      bool pfx (!opt_prefix.empty ());
      bool sep (!opt_sep.empty ());

      os << "int " << name << "::" << endl
         << "_parse (int start," << endl
         << "int argc," << endl
         << "char** argv," << endl
         << um << " opt_mode," << endl
         << um << " arg_mode)"
         << "{";

      if (sep)
        os << "bool opt = true;" // Still recognizing options.
           << endl;

      os << "for (; start < argc;)"
         << "{"
         << "const char* s = argv[start];";

      if (sep)
        os << endl
           << "if (std::strcmp (s, \"" << opt_sep << "\") == 0)"
           << "{"
           << "start++;"
           << "opt = false;"
           << "continue;"
           << "}"
           << map << "::const_iterator i (" << endl
           << "opt ? " << map << "_.find (s) : " << map << "_.end ());";
      else
        os << map << "::const_iterator i (" << map << "_.find (s));";

      os << endl
         << "if (i != " << map << "_.end ())"
         << "{"
         << "start += (*(i->second)) (*this, argv + start, argc - start);"
         << "}";

      // Unknown option.
      //
      if (pfx)
      {
        size_t n (opt_prefix.size ());

        os << "else if (";

        if (sep)
          os << "opt && ";

        os << "std::strncmp (s, \"" << opt_prefix << "\", " <<
          n << ") == 0 && s[" << n << "] != '\\0')"
           << "{"
           << "switch (opt_mode)"
           << "{"
           << "case ::cli::unknown_mode::skip:" << endl
           << "{"
           << "start++;"
           << "continue;"
           << "}"
           << "case ::cli::unknown_mode::stop:" << endl
           << "{"
           << "break;"
           << "}"
           << "case ::cli::unknown_mode::fail:" << endl
           << "{"
           << "throw ::cli::unknown_option (s);"
           << "}"
           << "}" // switch
           << "break;" // The stop case.
           << "}";
      }

      // Unknown argument.
      //
      os << "else"
         << "{"
         << "switch (arg_mode)"
         << "{"
         << "case ::cli::unknown_mode::skip:" << endl
         << "{"
         << "start++;"
         << "continue;"
         << "}"
         << "case ::cli::unknown_mode::stop:" << endl
         << "{"
         << "break;"
         << "}"
         << "case ::cli::unknown_mode::fail:" << endl
         << "{"
         << "throw ::cli::unknown_argument (s);"
         << "}"
         << "}" // switch
         << "break;" // The stop case.
         << "}"

         << "}" // for
         << "return start;"
         << "}";
    }

  private:
    option_map option_map_;
    traversal::names names_option_map_;
  };
}

void
generate_source (context& ctx)
{
  ctx.os << "#include <map>" << endl
         << "#include <cstring>" << endl
         << endl;

  traversal::cli_unit unit;
  traversal::names unit_names;
  namespace_ ns (ctx);
  class_ cl (ctx);

  unit >> unit_names >> ns;
  unit_names >> cl;

  traversal::names ns_names;

  ns >> ns_names >> ns;
  ns_names >> cl;

  unit.dispatch (ctx.unit);
}
