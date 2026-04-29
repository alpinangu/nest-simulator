#include <nest/multi_network/EventOutPort.h>

void
EventOutPort::init( nest_mn::Setup* setup, std::string port_name )
{

  port = setup->publishEventOutput( port_name );

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

  // map linear index to event out port
  nest_mn::LinearIndex l_index( 0, output_dim );
  port->map( &l_index, nest_mn::Index::GLOBAL, 1 );
}

void
EventOutPort::send( int n, double t )
{

  // std::cout << "neuron is " << n << "time is " << t << std::endl;
  port->insertEvent( t, nest_mn::GlobalIndex( n ) );
}
