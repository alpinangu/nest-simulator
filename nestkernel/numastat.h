#pragma once

#include <map>
#include <string>

namespace nest
{

using Stats = std::map< std::string, long long >;

Stats read_numastat();

}  // namespace nest
