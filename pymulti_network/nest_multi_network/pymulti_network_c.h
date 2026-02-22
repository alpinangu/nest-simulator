#ifndef PYMULTI_NETWORK_C_H
#define PYMULTI_NETWORK_C_H

#include "Python.h"

#if PY_MAJOR_VERSION >= 3
#define PyUnicodeString_FromString  PyUnicode_FromString
#else
#define PyUnicodeString_FromString  PyString_FromString
#endif

#endif // PYMULTI_NETWORK_C_H