// file      : tests/lexer/driver.cxx
// author    : Boris Kolpackov <boris@codesynthesis.com>
// copyright : Copyright (c) 2009 Code Synthesis Tools CC
// license   : MIT; see accompanying LICENSE file

#include <fstream>
#include <iostream>

#include <cli/token.hxx>
#include <cli/lexer.hxx>

using namespace std;

const char* keywords[] =
{
  "include",
  "namespace",
  "class",
  "signed",
  "unsigned",
  "bool",
  "char",
  "wchar_t",
  "short",
  "int",
  "long",
  "float",
  "double"
};

const char* punctuation[] = {";", ",", "::", "{", "}", /*"(", ")",*/ "=", "|"};

int main (int argc, char* argv[])
{
  if (argc != 2)
  {
    cerr << "usage: " << argv[0] << " file.cli" << endl;
    return 1;
  }

  ifstream ifs;
  ifs.exceptions (ifstream::failbit | ifstream::badbit);
  ifs.open (argv[1]);

  Lexer l (ifs, argv[1]);

  while (true)
  {
    Token t (l.next ());

    switch (t.type ())
    {
    case Token::t_eos:
      {
        cout << "<EOS>" << endl;
        return 0;
      }
    case Token::t_keyword:
      {
        cout << "keyword: " << keywords[t.keyword ()] << endl;
        break;
      }
    case Token::t_identifier:
      {
        cout << "identifier: " << t.identifier () << endl;
        break;
      }
    case Token::t_punctuation:
      {
        cout << punctuation[t.punctuation ()] << endl;
        break;
      }
    case Token::t_path_lit:
      {
        cout << "path: " << t.literal () << endl;
        break;
      }
    case Token::t_string_lit:
      {
        cout << t.literal () << endl;
        break;
      }
    case Token::t_char_lit:
      {
        cout << t.literal () << endl;
        break;
      }
    case Token::t_bool_lit:
      {
        cout << t.literal () << endl;
        break;
      }
    case Token::t_int_lit:
      {
        cout << t.literal () << endl;
        break;
      }
    case Token::t_float_lit:
      {
        cout << t.literal () << endl;
        break;
      }
    case Token::t_call_expr:
      {
        cout << t.expression () << endl;
        break;
      }
    case Token::t_template_expr:
      {
        cout << t.expression () << endl;
        break;
      }
    }
  }
}
