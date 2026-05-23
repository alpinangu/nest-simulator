#ifndef CONT_OUT_PORT_H
#define CONT_OUT_PORT_H

#include <nest/multi_network.h>

/**
 * Implements the outgoing MUSIC continuous port.
 */
class ContOutPort
{
public:
  void init( nest_mn::Setup* setup, char* port_name );

  int output_dim = 0; /* Width of the port | the dimensionality of the action space */
  double* data;

private:
  nest_mn::ContOutputPort* port;
};

#endif // CONT_OUT_PORT_H
