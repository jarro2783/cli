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

      os << type << " const&" << endl
         << name << " () const;"
         << endl;
    }
  };

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

      names (c, names_option_);

      os << "private:" << endl;

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
  traversal::names ns_names;
  class_ cl (ctx);

  unit >> includes;
  unit >> unit_names >> ns >> ns_names >> ns;
  ns_names >> cl;

  unit.dispatch (ctx.unit);
}
