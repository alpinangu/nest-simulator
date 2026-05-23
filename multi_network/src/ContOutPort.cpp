#include <nest/multi_network/ContOutPort.h>

void
ContOutPort::init( nest_mn::Setup* setup, char* port_name )
{

  port = setup->publishContOutput( port_name );

  // get dimensions of data
  if ( port->hasWidth() )
  {
    output_dim = port->width();
  }
  else
  {
    std::cout << "ERROR: Port-width not defined" << std::endl;
    std::exit( -1 );
  }

  data = new double[ output_dim ];
  for ( int i = 0; i < output_dim; ++i )
  {
    data[ i ] = 0.;
  }

  // Declare where in memory to put command_data
  nest_mn::ArrayData dmap( data, MPI_DOUBLE, 0, output_dim );
  port->map( &dmap, 1 );
}
