#ifndef MN_DECODER_RUNNER_H
#define MN_DECODER_RUNNER_H

#include <mpi.h>
#include <nest/multi_network.h>
#include <nest/multi_network/decoder.h>

#include <nest/multi_network/ContOutPort.h>
#include <nest/multi_network/EventInPort.h>

namespace nest_mn
{

class DecoderRunner
{
public:
  DecoderRunner( DecoderHandler* handler );
  void run( int argc, char** argv );

private:
  void init( int argc, char** argv );
  void run_loop();
  void finalize();
  void tick();
  long tick_count;

  DecoderHandler* handler_; // borrowed; do not delete
  nest_mn::DecodedSpikes window_spikes;

  nest_mn::Setup* setup;
  nest_mn::Runtime* runtime;
  double stoptime;
  double timestep;
  MPI_Comm comm;
  ContOutPort* port_out;
  EventInPort* port_in;
};

} // namespace nest_mn

#endif // MN_DECODER_RUNNER_H
