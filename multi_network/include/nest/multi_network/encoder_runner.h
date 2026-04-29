#ifndef MN_ENCODER_RUNNER_H
#define MN_ENCODER_RUNNER_H

#include <mpi.h>
#include <nest/multi_network.h>
#include <nest/multi_network/encoder.h>

#include <nest/multi_network/ContInPort.h>
#include <nest/multi_network/EventOutPort.h>

namespace nest_mn
{

class EncoderRunner
{
public:
  EncoderRunner( EncoderHandler* handler );
  void run( int argc, char** argv );

private:
  void init( int argc, char** argv );
  void run_loop();
  void finalize();
  void tick();
  void set_input_spiketrain( const EncodedSpikeTrains& trains );
  long tick_count;

  EncoderHandler* handler_; // borrowed; do not delete

  nest_mn::Setup* setup;
  nest_mn::Runtime* runtime;
  double stoptime;
  double timestep;
  MPI_Comm comm;
  ContInPort* port_in;
  EventOutPort* port_out;
};

} // namespace nest_mn

#endif // MN_ENCODER_RUNNER_H
