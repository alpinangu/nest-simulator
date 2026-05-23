#include <nest/multi_network/EventInPort.h>

void
EventInPort::init( nest_mn::Setup* setup, char* port_name )
{

  port = setup->publishEventInput( port_name );

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

  // map linear index to event out port
  nest_mn::LinearIndex l_index( 0, input_dim );
  port->map( &l_index, this, 0.003, 1 ); // TODO acceptable latency of 0. Does that work?
}

void
EventInPort::operator()( double t, nest_mn::GlobalIndex id )
{
  spikes.push( Event( t, id ) ); // TODO add acceptable latency to t?
  // std::cout << "(Decoder Input) sender=" << id << ", time=" << (t*1000) << ".000 ms" << std::endl;
}
