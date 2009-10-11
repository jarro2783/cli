// file      : cli/usage.hxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#ifndef CLI_USAGE_HXX
#define CLI_USAGE_HXX

#include <cstddef> // std::size_t
#include <cstdlib> // std::abort

#include <cutl/compiler/code-stream.hxx>

template <typename C>
class usage_indenter: public cutl::compiler::code_stream<C>
{
public:
  usage_indenter (cutl::compiler::code_stream<C>& out)
      : out_ (out),
        option_length_ (0),
        construct_ (con_newline)
  {
  }

private:
  usage_indenter (usage_indenter const&);

  usage_indenter&
  operator= (usage_indenter const&);

public:
  virtual void
  put (C c)
  {
    switch (c)
    {
    case '\n':
      {
        switch (construct_)
        {
        case con_newline:
          {
            out_.put (c);
            break;
          }
        case con_option:
          {
            construct_ = con_newline;
            break;
          }
        case con_description:
          {
            out_.put (c);
            construct_ = con_newline;
            break;
          }
        default:
          {
            std::abort ();
          }
        }

        break;
      }
    case '-':
      {
        switch (construct_)
        {
        case con_newline:
          {
            construct_ = con_option;
            option_length_ = 0;
            output_indentation ();
            out_.put (c);
            ++option_length_;
            break;
          }
        case con_option:
          {
            ++option_length_;
            //fall through
          }
        case con_description:
          {
            out_.put (c);
            break;
          }
        default:
          {
            std::abort ();
          }
        }

        break;
      }
    default:
      {
        switch (construct_)
        {
        case con_newline:
          {
            construct_ = con_description;
            output_indentation ();
            out_.put (c);
            break;
          }
        case con_option:
          {
            ++option_length_;
            //fall through
          }
        default:
          {
            out_.put (c);
            break;
          }
        }

        break;
      }
    }
  }

  virtual void
  unbuffer ()
  {
  }

private:
  void
  output_indentation ()
  {
    std::size_t spaces;

    switch (construct_)
    {
    case con_option:
      {
        spaces = 2;
        option_length_ += 2;
        break;
      }
    case con_description:
      {
        spaces = 29;

        if (option_length_)
        {
          if (option_length_ > spaces)
            spaces = 1;
          else
            spaces -= option_length_;

          option_length_ = 0;
        }

        break;
      }
    default:
      {
        std::abort ();
      }
    }

    while (spaces--)
      out_.put (' ');
  }

private:
  cutl::compiler::code_stream<C>& out_;
  size_t option_length_;

  enum construct
  {
    con_newline,
    con_option,
    con_description
  } construct_;
};

#endif // CLI_USAGE_HXX
