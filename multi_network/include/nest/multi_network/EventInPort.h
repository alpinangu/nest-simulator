#ifndef EVENT_IN_PORT_H
#define EVENT_IN_PORT_H

#include <nest/multi_network.h>
#include <queue>

/**
 * Implements the incoming MUSIC event port.
 */
class EventInPort : nest_mn::EventHandlerGlobalIndex
{
public:
  class Event
  {
  public:
    double t;
    int id;
    Event( double t_, int id_ )
      : t( t_ )
      , id( id_ )
    {
    }
    bool
    operator<( const Event& other ) const
    {
      return t > other.t;
    }
  };

  std::priority_queue< Event > spikes;

  void init( nest_mn::Setup* setup, char* port_name );

  int input_dim = 0; /* Width of the port | the dimensionality of the input to the decoder, output from NEST, number of
                        output neurons */
  double* data;

private:
  nest_mn::EventInputPort* port;

  void operator()( double t, nest_mn::GlobalIndex id );
};

#endif // EVENT_IN_PORT_H
