#include <nest/multi_network/encoder_runner.h>

#include <cstdlib>
#include <iostream>

namespace nest_mn
{

EncoderRunner::EncoderRunner( EncoderHandler* handler )
  : handler_( handler )
  , setup( nullptr )
  , runtime( nullptr )
  , stoptime( 0.0 )
  , timestep( 0.0 )
  , comm( MPI_COMM_NULL )
  , port_in( nullptr )
  , port_out( nullptr )
  , tick_count( 0 )
{
  port_in = new ContInPort();
  port_out = new EventOutPort();
}

void
EncoderRunner::run( int argc, char** argv )
{
  init( argc, argv );
  run_loop();
  finalize();
}

void
EncoderRunner::init( int argc, char** argv )
{
  setup = new nest_mn::Setup( argc, argv );

  setup->config( "stoptime", &stoptime );
  setup->config( "timestep", &timestep );

  comm = setup->communicator();

  int nProcesses;
  MPI_Comm_size( comm, &nProcesses );

  if ( nProcesses > 1 )
  {
    std::cout << "ERROR: num processes (np) not equal 1" << std::endl;
    MPI_Abort( comm, 1 );
  }

  port_in->init( setup, "in" );
  port_out->init( setup, "out" );
}

void
EncoderRunner::run_loop()
{
  runtime = new nest_mn::Runtime( setup, timestep );
  tick_count = 0;

  for ( ; runtime->time() < stoptime; )
  {
    tick();
    runtime->tick();
    ++tick_count;
  }
}

void
EncoderRunner::tick()
{
  if ( ( tick_count + 1 ) % 200 != 0 )
  {
    return;
  }

  EncodedSpikeTrains trains = ( *handler_ )( port_in->data, static_cast< std::size_t >( port_in->input_dim ) );

  set_input_spiketrain( trains );
}

void
EncoderRunner::set_input_spiketrain( const EncodedSpikeTrains& trains )
{
  for ( std::size_t channel = 0; channel < trains.size(); ++channel )
  {
    for ( double time_stamp_offset : trains[ channel ] )
    {
      port_out->send( static_cast< int >( channel ), runtime->time() + time_stamp_offset );
    }
  }
}

void
EncoderRunner::finalize()
{
  if ( runtime != nullptr )
  {
    runtime->finalize();
    delete runtime;
    runtime = nullptr;
  }

  delete port_in;
  port_in = nullptr;

  delete port_out;
  port_out = nullptr;
}

} // namespace nest_mn
