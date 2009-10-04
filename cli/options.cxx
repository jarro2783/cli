// This code was generated by CLI, a command line interface
// compiler for C++.
//

#include "options.hxx"

#include <string>
#include <vector>
#include <ostream>
#include <sstream>

namespace cli
{
  // unknown_option
  //
  unknown_option::
  ~unknown_option () throw ()
  {
  }

  void unknown_option::
  print (std::ostream& os) const
  {
    os << "unknown option '" << option () << "'";
  }

  const char* unknown_option::
  what () const throw ()
  {
    return "unknown option";
  }

  // unknown_argument
  //
  unknown_argument::
  ~unknown_argument () throw ()
  {
  }

  void unknown_argument::
  print (std::ostream& os) const
  {
    os << "unknown argument '" << argument () << "'";
  }

  const char* unknown_argument::
  what () const throw ()
  {
    return "unknown argument";
  }

  // missing_value
  //
  missing_value::
  ~missing_value () throw ()
  {
  }

  void missing_value::
  print (std::ostream& os) const
  {
    os << "missing value for option '" << option () << "'";
  }

  const char* missing_value::
  what () const throw ()
  {
    return "missing option value";
  }

  // invalid_value
  //
  invalid_value::
  ~invalid_value () throw ()
  {
  }

  void invalid_value::
  print (std::ostream& os) const
  {
    os << "invalid value '" << value () << "' for option '"
       << option () << "'";
  }

  const char* invalid_value::
  what () const throw ()
  {
    return "invalid option value";
  }

  template <typename X>
  struct parser
  {
    static int
    parse (X& x, char** argv, int n)
    {
      if (n > 1)
      {
        std::istringstream is (argv[1]);
        if (!(is >> x && is.eof ()))
          throw invalid_value (argv[0], argv[1]);
        return 2;
      }
      else
        throw missing_value (argv[0]);
    }
  };

  template <>
  struct parser<bool>
  {
    static int
    parse (bool& x, char**, int)
    {
      x = true;
      return 1;
    }
  };

  template <>
  struct parser<std::string>
  {
    static int
    parse (std::string& x, char** argv, int n)
    {
      if (n > 1)
      {
        x = argv[1];
        return 2;
      }
      else
        throw missing_value (argv[0]);
    }
  };

  template <typename X>
  struct parser<std::vector<X> >
  {
    static int
    parse (std::vector<X>& v, char** argv, int n)
    {
      X x;
      int i (parser<X>::parse (x, argv, n));
      v.push_back (x);
      return i;
    }
  };

  template <typename X, typename T, T X::*P>
  int
  thunk (X& x, char** argv, int n)
  {
    return parser<T>::parse (x.*P, argv, n);
  }
}

#include <map>
#include <cstring>

// options
//

options::
options (int argc,
         char** argv,
         ::cli::unknown_mode opt,
         ::cli::unknown_mode arg)
: help_ (),
  version_ (),
  suppress_inline_ (),
  output_dir_ (),
  hxx_suffix_ (".hxx"),
  ixx_suffix_ (".ixx"),
  cxx_suffix_ (".cxx")
{
  _parse (1, argc, argv, opt, arg);
}

options::
options (int start,
         int argc,
         char** argv,
         ::cli::unknown_mode opt,
         ::cli::unknown_mode arg)
: help_ (),
  version_ (),
  suppress_inline_ (),
  output_dir_ (),
  hxx_suffix_ (".hxx"),
  ixx_suffix_ (".ixx"),
  cxx_suffix_ (".cxx")
{
  _parse (start, argc, argv, opt, arg);
}

options::
options (int argc,
         char** argv,
         int& end,
         ::cli::unknown_mode opt,
         ::cli::unknown_mode arg)
: help_ (),
  version_ (),
  suppress_inline_ (),
  output_dir_ (),
  hxx_suffix_ (".hxx"),
  ixx_suffix_ (".ixx"),
  cxx_suffix_ (".cxx")
{
  end = _parse (1, argc, argv, opt, arg);
}

options::
options (int start,
         int argc,
         char** argv,
         int& end,
         ::cli::unknown_mode opt,
         ::cli::unknown_mode arg)
: help_ (),
  version_ (),
  suppress_inline_ (),
  output_dir_ (),
  hxx_suffix_ (".hxx"),
  ixx_suffix_ (".ixx"),
  cxx_suffix_ (".cxx")
{
  end = _parse (start, argc, argv, opt, arg);
}

typedef
std::map<std::string, int (*) (options&, char**, int)>
_cli_options_map;

static _cli_options_map _cli_options_map_;

struct _cli_options_map_init
{
  _cli_options_map_init ()
  {
    _cli_options_map_["--help"] = 
    &::cli::thunk<options, bool, &options::help_>;
    _cli_options_map_["--version"] = 
    &::cli::thunk<options, bool, &options::version_>;
    _cli_options_map_["--suppress-inline"] = 
    &::cli::thunk<options, bool, &options::suppress_inline_>;
    _cli_options_map_["--output-dir"] = 
    &::cli::thunk<options, std::string, &options::output_dir_>;
    _cli_options_map_["--hxx-suffix"] = 
    &::cli::thunk<options, std::string, &options::hxx_suffix_>;
    _cli_options_map_["--ixx-suffix"] = 
    &::cli::thunk<options, std::string, &options::ixx_suffix_>;
    _cli_options_map_["--cxx-suffix"] = 
    &::cli::thunk<options, std::string, &options::cxx_suffix_>;
  }
} _cli_options_map_init_;

int options::
_parse (int start,
        int argc,
        char** argv,
        ::cli::unknown_mode opt_mode,
        ::cli::unknown_mode arg_mode)
{
  bool opt (true);

  for (; start < argc;)
  {
    const char* s (argv[start]);

    if (std::strcmp (s, "--") == 0)
    {
      start++;
      opt = false;
      continue;
    }

    _cli_options_map::const_iterator i (
      opt ? _cli_options_map_.find (s) : _cli_options_map_.end ());

    if (i != _cli_options_map_.end ())
    {
      start += (*(i->second)) (*this, argv + start, argc - start);
    }
    else if (opt && s[0] == '-' && s[1] != '\0')
    {
      switch (opt_mode)
      {
        case ::cli::unknown_mode::skip:
        {
          start++;
          continue;
        }
        case ::cli::unknown_mode::stop:
        {
          break;
        }
        case ::cli::unknown_mode::fail:
        {
          throw ::cli::unknown_option (s);
        }
      }

      break;
    }
    else
    {
      switch (arg_mode)
      {
        case ::cli::unknown_mode::skip:
        {
          start++;
          continue;
        }
        case ::cli::unknown_mode::stop:
        {
          break;
        }
        case ::cli::unknown_mode::fail:
        {
          throw ::cli::unknown_argument (s);
        }
      }

      break;
    }
  }

  return start;
}

