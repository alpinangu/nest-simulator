#ifndef MN_ENCODER_H
#define MN_ENCODER_H

#include <cstddef>
#include <vector>

namespace nest_mn
{

using EncodedSpikeTrains = std::vector< std::vector< double > >;

/**
 * User-defined encoder interface.
 *
 * It only converts an observation into spike-time lists.
 *
 * Convention:
 *   result[channel] = list of spike-time offsets for that channel
 */
class EncoderHandler
{
public:
  virtual ~EncoderHandler()
  {
  }

  virtual EncodedSpikeTrains operator()( const double* observation, std::size_t observation_size ) = 0;
};

} // namespace nest_mn

#endif // NEST_MULTI_NETWORK_ENCODER_H
