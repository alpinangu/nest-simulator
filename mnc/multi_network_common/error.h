/*
 *  error.h
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

#ifndef ERROR_H
#define ERROR_H


#include <sstream>
#include <string>

namespace nest_mn {

  void error ();
  void hang ();
  void error (std::string msg);
  void error (std::ostringstream& ostr);
  void error0 (std::string msg);
  void errorRank (std::string msg);
  void checkOnce (bool& flag, std::string msg);
  void checkInstantiatedOnce (bool& flag, std::string className);
  void checkCalledOnce (bool& isCalled,
			std::string funcName,
			std::string suffix);
  
} // namespace

#endif /* ERROR_H */
