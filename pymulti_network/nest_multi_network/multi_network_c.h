#ifndef nest_mn_C_H
#define nest_mn_C_H

#include <Python.h>

#include "nest/multi_network/configuration.h"
#include "nest/multi_network/encoder.h"
#include "nest/multi_network/event.h"
#include "nest/multi_network/index_map.h"
#include "nest/multi_network/message.h"
#include "nest/multi_network/runtime.h"
#include "nest/multi_network/setup.h"
#include <nest/multi_network/decoder.h>

#include <iostream>
#include <mpi.h>
#include <stdexcept>
#include <string>

namespace nest_mn
{
using namespace std;

inline MPI_Comm
communicator( nest_mn::Setup* s )
{
  return ( MPI_Comm ) s->communicator();
}

inline MPI_Comm
communicator( nest_mn::Runtime* r )
{
  return ( MPI_Comm ) r->communicator();
}

inline int
toint( nest_mn::GlobalIndex i )
{
  return i;
}

inline int
toint( nest_mn::LocalIndex i )
{
  return i;
}

static bool pythonError;
static PyObject* etype;
static PyObject* evalue;
static PyObject* etraceback;

bool
tick( Runtime* ptr )
{
  ptr->tick();
  if ( !pythonError )
  {
    return true;
  }

  pythonError = false;
  PyErr_Restore( etype, evalue, etraceback );
  return false;
}

bool EventCallback( PyObject* func, double d, Index::Type t, int index );

class EventHandler
{ // All this just to insert a virtual d
public:
  PyObject* const func;

  EventHandler( PyObject* func )
    : func( func )
  {
  }
  virtual ~EventHandler()
  {
  }
  inline void
  callback( double d, Index::Type t, int i )
  {
    if ( pythonError )
    {
      return;
    }
    if ( EventCallback( func, d, t, i ) )
    {
      return;
    }

    pythonError = true;
    PyErr_Fetch( &etype, &evalue, &etraceback );
  }
};


class EHLocal : public EventHandler, public EventHandlerLocalIndex
{
public:
  EHLocal( PyObject* func )
    : EventHandler( func ) {};
  void
  operator()( double d, LocalIndex i )
  {
    callback( d, Index::LOCAL, i );
  }
};

class EHGlobal : public EventHandler, public EventHandlerGlobalIndex
{
public:
  EHGlobal( PyObject* func )
    : EventHandler( func ) {};
  void
  operator()( double d, GlobalIndex i )
  {
    callback( d, Index::GLOBAL, i );
  }
};

inline EventHandlerPtr
getEventHandlerPtr( Index::Type t, EventHandler* eh )
{
  return ( t == Index::GLOBAL ) ? EventHandlerPtr( ( EHGlobal* ) eh ) : EventHandlerPtr( ( EHLocal* ) eh );
}

bool MessageCallback( PyObject* func, double t, void* msg, size_t size, bool pickled );

class MHandler : public MessageHandler
{
public:
  PyObject* const func;
  const bool pickled;

  MHandler( PyObject* func, bool pickled )
    : func( func )
    , pickled( pickled )
  {
  }
  void
  operator()( double t, void* msg, size_t size )
  {
    if ( pythonError )
    {
      return;
    }
    if ( MessageCallback( func, t, msg, size, pickled ) )
    {
      return;
    }

    pythonError = true;
    PyErr_Fetch( &etype, &evalue, &etraceback );
  }
};

bool EncoderCallback( PyObject* func, const double* observation, size_t observation_size, EncodedSpikeTrains* result );

class EHandler : public EncoderHandler
{
public:
  PyObject* const func;

  EHandler( PyObject* func )
    : func( func )
  {
  }

  EncodedSpikeTrains
  operator()( const double* observation, size_t observation_size ) override
  {
    EncodedSpikeTrains result;

    if ( pythonError )
    {
      return result;
    }

    if ( EncoderCallback( func, observation, observation_size, &result ) )
    {
      return result;
    }

    pythonError = true;
    PyErr_Fetch( &etype, &evalue, &etraceback );

    return EncodedSpikeTrains();
  }
};

inline EncodedSpikeTrains
callEncoderHandler( EncoderHandler* handler, const double* observation, size_t observation_size )
{
  return ( *handler )( observation, observation_size );
}

bool DecoderCallback( PyObject* func, const DecodedSpikes& spikes, DecodedAction* result );

class DHandler : public DecoderHandler
{
public:
  PyObject* const func;

  DHandler( PyObject* func )
    : func( func )
  {
  }

  DecodedAction
  operator()( const DecodedSpikes& spikes ) override
  {
    DecodedAction result;

    if ( pythonError )
    {
      throw std::runtime_error( "Python decoder callback failed" );
    }

    if ( DecoderCallback( func, spikes, &result ) )
    {
      return result;
    }

    pythonError = true;
    PyErr_Fetch( &etype, &evalue, &etraceback );

    throw std::runtime_error( "Python decoder callback failed" );
  }
};

inline DecodedAction
callDecoderHandler( DecoderHandler* handler, const DecodedSpikes* spikes )
{
  return ( *handler )( *spikes );
}

class Implementer
{
public:
  static inline void
  mapImpl( ContInputPort* p, DataMap* d, double v, int i, bool b )
  {
    p->ContInputPort::mapImpl( d, v, i, b );
  }

  static inline void
  mapImpl( ContOutputPort* p, DataMap* d, int i )
  {
    p->ContOutputPort::mapImpl( d, i );
  }

  static inline void
  mapImpl( EventInputPort* p, IndexMap* m, Index::Type t, EventHandlerPtr e, double d, int i )
  {
    p->EventInputPort::mapImpl( m, t, e, d, i );
  }

  static inline void
  mapImpl( EventOutputPort* p, IndexMap* m, Index::Type t, int i )
  {
    p->EventOutputPort::mapImpl( m, t, i );
  }

  static inline void
  insertEventImpl( EventOutputPort* p, double d, int i )
  {
    p->EventOutputPort::insertEventImpl( d, i );
  }
  static inline void
  mapImpl( MessageInputPort* p, MessageHandler* handler, double accLatency, int maxBuffered )
  {
    p->MessageInputPort::mapImpl( handler, accLatency, maxBuffered );
  }

  static inline void
  mapImpl( MessageOutputPort* p, int maxBuffered )
  {
    p->MessageOutputPort::mapImpl( maxBuffered );
  }
};
}

#endif // nest_mn_C_H
