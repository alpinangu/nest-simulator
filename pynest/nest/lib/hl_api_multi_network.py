# -*- coding: utf-8 -*-
#
# hl_api_network.py
#
# This file is part of NEST.
#
# Copyright (C) 2004 The NEST Initiative
#
# NEST is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
#
# NEST is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with NEST.  If not, see <http://www.gnu.org/licenses/>.

"""
Functions for connecting different networks
"""


import nest

from ..ll_api import check_stack, sli_func, spp, sps, sr


__all__ = [
    "Launch",
    "SetApplicationMap",
    "SetConnectivityMap",
]

@check_stack
def Launch(exe, n):
    return sli_func("Launch_l_i", exe, n)

@check_stack
def SetApplicationMap(dict):
    return sli_func("SetApplicationMap_D", dict)

@check_stack
def SetConnectivityMap(dict):
    return sli_func("SetConnectivityMap_D", dict)
