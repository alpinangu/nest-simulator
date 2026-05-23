#ifndef MN_DECODER_H
#define MN_DECODER_H

#include <vector>

namespace nest_mn
{

struct DecodedSpike
{
  int id;
  double t;

  DecodedSpike( int id_, double t_ )
    : id( id_ )
    , t( t_ )
  {
  }
};

using DecodedSpikes = std::vector< DecodedSpike >;
using DecodedAction = double;

/**
 * User-defined decoder interface.
 *
 * It converts a list of spikes collected over one decoding window
 * into an action.
 *
 * Convention:
 *   spikes[i].id = event channel / neuron id
 *   spikes[i].t  = spike time in MUSIC simulation seconds
 */
class DecoderHandler
{
public:
  virtual ~DecoderHandler()
  {
  }

  virtual DecodedAction operator()( const DecodedSpikes& spikes ) = 0;
};

} // namespace nest_mn

#endif // MN_DECODER_H
