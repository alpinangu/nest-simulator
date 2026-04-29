#ifndef EVENT_OUT_PORT_H
#define EVENT_OUT_PORT_H

#include <nest/multi_network.h>

#define DEBUG_OUTPUT false

/**
 * Implements the outgoing MUSIC event port.
 */
class EventOutPort
{
public:
  void init( nest_mn::Setup* setup, std::string port_name );
  void send( int n, double t );

  int output_dim = 0; // Number of neurons in the input layer of the network
  double* data;       /** Data for the current time step. */

private:
  nest_mn::EventOutputPort* port;
};

#endif // EVENT_OUT_PORT_H
