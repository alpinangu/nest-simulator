#include <nest/multi_network/ContInPort.h>

void
ContInPort::init( nest_mn::Setup* setup, std::string port_name )
{

  port = setup->publishContInput( port_name );

  // get dimensions of data
  if ( port->hasWidth() )
  {
    input_dim = port->width();
  }
  else
  {
    std::cout << "ERROR: Port-width not defined" << std::endl;
    std::exit( -1 );
  }

  data = new double[ input_dim ];
  for ( int i = 0; i < input_dim; ++i )
  {
    data[ i ] = 0.;
  }

  // Declare where in memory to put command_data
  nest_mn::ArrayData dmap( data, MPI_DOUBLE, 0, input_dim );
  port->map( &dmap, 0.004, 1, false );
}
