/*
 *  ioutils.h
 *
 *  This file is part of NEST.
 *
 *  Copyright (C) 2004 The NEST Initiative
 *
 *  NEST is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  NEST is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with NEST.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef IOUTILS_H
#define IOUTILS_H


// C++ includes:
#include <sstream>

// Includes from libnestutil:



// Includes from nestkernel:


// Includes from sli:



namespace nest
{

  namespace IOUtils 
  {
    void write (std::ostringstream& out, std::string s);
    std::string read (std::istringstream& in, int delim);
    std::string read (std::istringstream& in);
  }


} // namespace


#endif /* IOUTILS_H */