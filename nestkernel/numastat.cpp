#include "numastat.h"

#include <cstdio>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace nest
{

Stats
read_numastat()
{
  FILE* pipe = popen( "LC_ALL=C numastat", "r" );
  if ( !pipe )
  {
    throw std::runtime_error( "popen failed" );
  }

  std::string output;
  char buffer[ 4096 ];
  while ( fgets( buffer, sizeof( buffer ), pipe ) )
  {
    output += buffer;
  }

  if ( pclose( pipe ) != 0 )
  {
    throw std::runtime_error( "numastat failed" );
  }

  std::istringstream input( output );
  std::string header, node, metric;
  std::getline( input, header );

  std::vector< std::string > nodes;
  std::istringstream columns( header );
  while ( columns >> node )
  {
    nodes.push_back( node );
  }
  if ( nodes.empty() )
  {
    throw std::runtime_error( "Missing header" );
  }

  Stats stats;
  while ( input >> metric )
  {
    for ( const auto& name : nodes )
    {
      long long value;
      if ( !( input >> value ) )
      {
        throw std::runtime_error( "Unexpected output" );
      }
      stats[ metric + " " + name ] = value;
    }
  }
  return stats;
}

}  // namespace nest
