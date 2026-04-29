#ifndef CONT_IN_PORT_H
#define CONT_IN_PORT_H

#include <mpi.h>
#include <nest/multi_network.h>

#define DEBUG_OUTPUT false

/**
 * Implements the imcoming MUSIC continuous port.
 */
class ContInPort
{
public:
  void init( nest_mn::Setup* setup, std::string port_name );

  int input_dim = 0; /**< Width of the port | the dimensionality of the input */
  double* data;      /** Data for the current time step. */


private:
  nest_mn::ContInputPort* port;
};

#endif // CONT_IN_PORT_H
