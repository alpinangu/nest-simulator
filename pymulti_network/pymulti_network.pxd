# The following include is needed to define a missing type MPI_Message
# which is lacking for a certain combination of MPI and mpi4py versions
cdef extern from "nest_multi_network/multi_network_c.h":
    pass

include "pyconfig.pxi"
cimport mpi4py.MPI as MPI

IF MPI4V2:
    from mpi4py.libmpi cimport *
ELSE:
    from mpi4py.mpi_c cimport *

from cpython.ref cimport PyObject
from libc.stddef cimport size_t
from libcpp cimport bool as cbool
from libcpp.string cimport string
from libcpp.vector cimport vector

###########################################################

cdef extern from "nest/multi_network/predict_rank.h" namespace "nest_mn":
    cdef int CPredictRank "nest_mn::predictRank" (int, char**)

cdef extern from "nest/multi_network/message.h" namespace "nest_mn":
    cdef cppclass CMessageHandler "nest_mn::MessageHandler":
        pass

cdef extern from "nest_multi_network/multi_network_c.h" namespace "nest_mn":
    cdef cppclass CEventHandler "nest_mn::EventHandler":
        pass
    cdef cppclass CEHLocal "nest_mn::EHLocal"(CEventHandler):
        CEHLocal(PyObject*)
    cdef cppclass CEHGlobal "nest_mn::EHGlobal"(CEventHandler):
        CEHGlobal(PyObject*)
    cdef cppclass CMHandler "nest_mn::MHandler"(CMessageHandler):
        CMHandler(PyObject*, cbool)

cdef extern from "nest/multi_network/event.h" namespace "nest_mn":
    cdef cppclass CEventHandlerGlobalIndex "nest_mn::EventHandlerGlobalIndex":
        pass
    cdef cppclass CEventHandlerLocalIndex "nest_mn::EventHandlerLocalIndex":
        pass
    cdef cppclass CEventHandlerPtr "nest_mn::EventHandlerPtr":
        pass

cdef extern from "nest/multi_network/data_map.h" namespace "nest_mn":
    cdef cppclass CDataMap "nest_mn::DataMap":
        pass

cdef extern from "nest/multi_network/index_map.h" namespace "nest_mn":
    cdef cppclass CIndex "nest_mn::Index":
        int WILDCARD_MAX
    ctypedef enum IndexType "nest_mn::Index::Type":
            IndexGLOBAL "nest_mn::Index::GLOBAL",
            IndexLOCAL "nest_mn::Index::LOCAL"

    cdef cppclass GlobalIndex(CIndex):
        GlobalIndex(int)

    cdef cppclass LocalIndex(CIndex):
        LocalIndex(int)

    cdef cppclass CIndexMap "nest_mn::IndexMap":
        pass

cdef extern from "nest/multi_network/permutation_index.h" namespace "nest_mn":
    cdef cppclass PermutationIndex(CIndexMap):
        PermutationIndex(GlobalIndex*, int)

cdef extern from "nest/multi_network/linear_index.h" namespace "nest_mn":
    cdef cppclass LinearIndex(CIndexMap):
        LinearIndex(GlobalIndex, int)

cdef extern from "nest/multi_network/array_data.h" namespace "nest_mn":
    cdef cppclass CArrayData "nest_mn::ArrayData"(CDataMap):
        CArrayData(void*, MPI_Datatype, int, int)
        CArrayData(void*, MPI_Datatype, CIndexMap*)

cdef extern from "nest/multi_network/port.h" namespace "nest_mn":
    cdef cppclass CPort "nest_mn::Port":
        cbool isConnected()
        cbool hasWidth()
        int width()

    cdef cppclass CContInputPort "nest_mn::ContInputPort"(CPort):
        pass

    cdef cppclass CContOutputPort "nest_mn::ContOutputPort"(CPort):
        pass

    cdef cppclass CEventInputPort "nest_mn::EventInputPort"(CPort):
        pass

    cdef cppclass CEventOutputPort "nest_mn::EventOutputPort"(CPort):
        pass

    cdef cppclass CMessageInputPort "nest_mn::MessageInputPort"(CPort):
        pass

    cdef cppclass CMessageOutputPort "nest_mn::MessageOutputPort"(CPort):
        void insertMessage(double, void*, size_t)

# This is necessary for the virtual downcast
cdef extern from *:
    CContInputPort*   dc_CContInputPort   \
        "dynamic_cast<nest_mn::ContInputPort*>" (CPort*)
    CContOutputPort*  dc_CContOutputPort  \
        "dynamic_cast<nest_mn::ContOutputPort*>"(CPort*)
    CEventInputPort*  dc_CEventInputPort  \
        "dynamic_cast<nest_mn::EventInputPort*>" (CPort*)
    CEventOutputPort* dc_CEventOutputPort \
        "dynamic_cast<nest_mn::EventOutputPort*>"(CPort*)
    CMessageInputPort*  dc_CMessageInputPort  \
        "dynamic_cast<nest_mn::MessageInputPort*>" (CPort*)
    CMessageOutputPort* dc_CMessageOutputPort \
        "dynamic_cast<nest_mn::MessageOutputPort*>"(CPort*)

cdef extern from "nest/multi_network/setup.h" namespace "nest_mn":
    cdef cppclass CSetup "nest_mn::Setup":
        CSetup(int&, char**&) except +
        CSetup(int&, char**&, int, int*) except +

        cbool config(string, string*)

        CContInputPort*     publishContInput(string)
        CContOutputPort*    publishContOutput(string)
        CEventInputPort*    publishEventInput(string)
        CEventOutputPort*   publishEventOutput(string)
        CMessageInputPort*  publishMessageInput(string)
        CMessageOutputPort* publishMessageOutput(string)

cdef extern from "nest/multi_network/runtime.h" namespace "nest_mn":
    cdef cppclass CRuntime "nest_mn::Runtime":
        CRuntime(CSetup*, double) except +
        void finalize()
        double time()
        void tick()

cdef extern void cython_callback(PyObject*, double, IndexType, int)

cdef extern from "nest_multi_network/multi_network_c.h" namespace "nest_mn":
    cdef inline MPI_Comm communicator(CSetup*)
    cdef inline MPI_Comm communicator(CRuntime*)
    cdef inline int toint(GlobalIndex)
    cdef inline int toint(LocalIndex)
    cdef inline cbool tick(CRuntime*) except False
    cdef inline CEventHandlerPtr getEventHandlerPtr(IndexType, CEventHandler*)

    cdef inline void mapImpl "nest_mn::Implementer::mapImpl" (
        CContInputPort*, CDataMap*, double, int, cbool)
    cdef inline void mapImpl "nest_mn::Implementer::mapImpl" (
        CContOutputPort*, CDataMap*, int)
    cdef inline void mapImpl "nest_mn::Implementer::mapImpl" (
        CEventInputPort*, CIndexMap*, IndexType,
        CEventHandlerPtr, double, int)
    cdef inline void mapImpl "nest_mn::Implementer::mapImpl" (
        CEventOutputPort*, CIndexMap*, IndexType, int)
    cdef inline void insertEventImpl "nest_mn::Implementer::insertEventImpl" (
        CEventOutputPort*, double, int)
    cdef inline void mapImpl "nest_mn::Implementer::mapImpl" (
        CMessageInputPort*, CMessageHandler*, double, int)
    cdef inline void mapImpl "nest_mn::Implementer::mapImpl" (
        CMessageOutputPort*, int)
    cdef inline void insertMessage "nest_mn::Implementer::insertMessage" (
        CMessageOutputPort*, double t, void*, size_t)

    cdef cbool pythonError
    cdef PyObject* etype
    cdef PyObject* evalue
    cdef PyObject* etraceback


###########################################################

cdef class Setup(object):
    cdef CSetup* ptr
    cdef list argv
    cdef int provided
    cdef readonly MPI.Intracomm comm
    cdef set ports

    cdef null(self)
    cpdef MPI.Intracomm getcomm(self)

###########################################################

cdef class Runtime(object):
    cdef CRuntime* ptr
    cdef readonly MPI.Intracomm comm
    cdef set ports
    cdef int isFinalized

###########################################################

cdef class Port(object):
    cdef CPort* ptr
    cpdef object null(self)

## Some day, virtual multiple inheritance will be
## a capital crime ^^^^

cdef class ContInputPort(Port):
    pass

cdef class ContOutputPort(Port):
    pass

cdef class EventInputPort(Port):
    cdef set events
    cpdef object null(self)

cdef class EventOutputPort(Port):
    pass

cdef class MessageInputPort(Port):
    cdef set events
    cpdef object null(self)

cdef class MessageOutputPort(Port):
    cdef bint pickled

##########################################################

from nest_multi_network.pybuffer cimport Buffer


cdef class DataMap(object):
    cdef CDataMap* ptr
    cdef Buffer buf

cdef class IndexMap(object):
    cdef CIndexMap* ptr
    cdef Buffer buf

cdef class EventHandler:
    cdef CEventHandler* ptr
    cdef object func

cdef class MessageHandler:
    cdef CMessageHandler* ptr
    cdef object func

cdef cbool EventCallback "nest_mn::EventCallback" ( \
  PyObject*, double, IndexType, int) except False

cdef cbool MessageCallback "nest_mn::MessageCallback" ( \
  PyObject*, double, void*, size_t, cbool) except False

###########################################################

cdef extern from "nest/multi_network/encoder.h" namespace "nest_mn":
    ctypedef vector[vector[double]] EncodedSpikeTrains

    cdef cppclass CEncoderHandler "nest_mn::EncoderHandler":
        EncodedSpikeTrains operator()(const double*, size_t)

cdef extern from "nest/multi_network/encoder_runner.h" namespace "nest_mn":
    cdef cppclass CEncoderRunner "nest_mn::EncoderRunner":
        CEncoderRunner(CEncoderHandler*)
        void run(int, char**)

cdef extern from "nest_multi_network/multi_network_c.h" namespace "nest_mn":
    cdef cppclass CEHandler "nest_mn::EHandler"(CEncoderHandler):
        CEHandler(PyObject*)

    cdef inline EncodedSpikeTrains callEncoderHandler(
        CEncoderHandler*,
        const double*,
        size_t)

cdef class EncoderHandler:
    cdef CEncoderHandler* ptr
    cdef object func

cdef cbool EncoderCallback "nest_mn::EncoderCallback" (
    PyObject*,
    const double*,
    size_t,
    EncodedSpikeTrains*
) except False
