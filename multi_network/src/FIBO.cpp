/*
 *  FIBO.cpp
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
#include <cstring>
#include <iostream>
#include "nest/multi_network/FIBO.h"

namespace nest {

  FIBO::FIBO (int es)
  {
    configure (es);
  }

  
  void
  FIBO::configure (int es)
  {
    elementSize_ = es;
    size_ = elementSize_ * nInitial_;
    if (es > 0)
      buffer_.resize (size_);
    current_ = 0;
  }

  
  bool
  FIBO::isEmpty ()
  {
    return current_ == 0;
  }

  
  void*
  FIBO::insert ()
  {
    if (current_ == size_)
      grow (2 * size_);

    void* memory = static_cast<void*> (&buffer_[current_]);
    current_ += elementSize_;
    return memory;
  }
  

  void
  FIBO::insert (void* elements, int n_elements)
  {
    int blockSize = elementSize_ * n_elements;
    if (current_ + blockSize > size_)
      grow (3 * (current_ + blockSize) / 2);

    void* memory = static_cast<void*> (&buffer_[current_]);
    memcpy (memory, elements, blockSize);
    current_ += blockSize;
  }


  void
  FIBO::clear ()
  {
    current_ = 0;
  }
  

  void
  FIBO::nextBlockNoClear (void*& data, int& blockSize)
  {

    data = static_cast<void*> (&buffer_[0]);
    blockSize = current_;
  }


  void
  FIBO::nextBlock (void*& data, int& blockSize)
  {
    nextBlockNoClear (data, blockSize);
    clear ();
  }


  void
  FIBO::grow (int newSize)
  {
    size_ = newSize;
    buffer_.resize (size_);
  }
  
}
