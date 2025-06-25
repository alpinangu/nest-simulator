/*
 *  This file is part of MUSIC.
 *  Copyright (C) 2007, 2008, 2009 INCF
 *
 *  MUSIC is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  MUSIC is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "nest/multi_network/parse.h"

#include <sstream>
#include <vector>
#include <cstdio>

namespace nest {
  
  Parser::Parser (std::string s)
    : in (new std::istringstream (s))
  {
  
  }
  Parser::~Parser ()
  {
	  delete in;
  }
  
  //a: removes only the leading whitespace
  void
  Parser::ignoreWhitespace ()
  {
    while (isspace (in->peek ()))
      in->ignore ();
  }

  
  void
  Parser::parseString (std::ostringstream& arg, char delim)
  {
    while (true)
      {
	int c;
	switch (c = in->get ())
	  {
	  case '\'':
	  case '"':
	    if (c == delim)
	      break;
	  default:
	    arg << (char) c;
	    continue;
	  case '\\':
	    arg << (char) in->get ();
	    continue;
	  case EOF:
	    // NOTE: generate error message
	    break;
	  }
	break;
      }  
  }

  //a: returns the next space-separated argument, supporting quotes and escapes
  std::string
  Parser::nextArg ()
  {
    std::ostringstream arg;
    while (true)
      {
	int c;
	switch (c = in->get ())
	  {
	  default:
	    arg << (char) c;
	    continue;
	  case '\\':
	    arg << (char) in->get ();
	    continue;
	  case '\'':
	  case '"':
	    parseString (arg, c);
	    continue;
	  case ' ':
	  case '\t':
	  case EOF:
	    break;
	  }
	break;
      }
    return arg.str ();
  }

  //a: not a part of the Parser class, but in the same header file
  //a: because we call that function without creating an object
  //a: builds an artificial argv array from the input strings and updates *argc to the number of arguments parsed
  char **
  parseArgs (std::string cmd,
	     std::string argstring,
	     int* argc)
  {
    //creates a parsser object
    Parser in (argstring);
    //a: a vector of strings
    std::vector<std::string> args;
    args.push_back (cmd);

    //a: removes the leading white space from the argument string
    in.ignoreWhitespace ();

    while (! in.eof ())
      args.push_back (in.nextArg ());

    //a: this mimics the argv
    //a: result points to the first char pointer
    int nArgs = args.size ();
    char** result = new char*[nArgs + 1];

    for (int i = 0; i < nArgs; ++i)
      {
	int len = args[i].length ();
  //a: setting the pointer at position i to point to a fresh memory block.
	result[i] = new char[len + 1];
	args[i].copy (result[i], len);
  //a: manually null-terminates the string
	result[i][len] = '\0';
      }
    result[nArgs] = 0;
    *argc = nArgs;
    return result;
  }

}
