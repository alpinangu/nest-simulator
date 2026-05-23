#include <nest/multi_network/decoder_runner.h>

#include <cstdlib>
#include <iostream>

namespace nest_mn
{

DecoderRunner::DecoderRunner( DecoderHandler* handler )
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
  port_out = new ContOutPort();
  port_in = new EventInPort();
}

void
DecoderRunner::run( int argc, char** argv )
{
  init( argc, argv );
  run_loop();
  finalize();
}

void
DecoderRunner::init( int argc, char** argv )
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
DecoderRunner::run_loop()
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
DecoderRunner::tick()
{
  double next_t = runtime->time() + timestep;

  while ( !port_in->spikes.empty() && port_in->spikes.top().t < next_t )
  {
    int id = port_in->spikes.top().id;
    double t = port_in->spikes.top().t;

    window_spikes.push_back( nest_mn::DecodedSpike( id, t ) );

    port_in->spikes.pop();
  }

  if ( ( tick_count + 1 ) % 200 != 0 )
  {
    return;
  }
  DecodedAction action = ( *handler_ )( window_spikes );

  port_out->data[ 0 ] = action;

  window_spikes.clear();
}

void
DecoderRunner::finalize()
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
