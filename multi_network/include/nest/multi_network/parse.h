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

#ifndef MN_PARSE_H
#define MN_PARSE_H

#include <string>
#include <vector>
#include <istream>

namespace nest_mn {
  
  class Parser {
    //a: private by default
    std::istream* in;
    void parseString (std::ostringstream& arg, char delim);
  public:
    Parser (std::string s);
    ~Parser();
    bool eof () { return in->eof (); }
    void ignoreWhitespace ();
    std::string nextArg ();
  };

  //a: not a part of the Parser class because we call it without creating an instance
  char ** parseArgs (std::string cmd,
		     std::string args,
		     int* argc);
}

#endif
