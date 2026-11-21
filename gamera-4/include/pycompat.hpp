#ifndef GAMERA_PYCOMPAT_HPP
#define GAMERA_PYCOMPAT_HPP

#include <Python.h>

/*
 * Python 3.13 removed the old (pre-PEP 3118) buffer API:
 *   PyObject_CheckReadBuffer, PyObject_AsReadBuffer, PyObject_AsWriteBuffer.
 *
 * Gamera's kNN and GUI code still uses it.  On older Pythons keep the
 * original calls; on 3.13+ map them onto the buffer protocol.
 */

#if PY_VERSION_HEX < 0x030D0000

#define GAMERA_PyObject_CheckReadBuffer(o) PyObject_CheckReadBuffer(o)
#define GAMERA_PyObject_AsReadBuffer(o, b, l) PyObject_AsReadBuffer((o), (b), (l))
#define GAMERA_PyObject_AsWriteBuffer(o, b, l) PyObject_AsWriteBuffer((o), (b), (l))

#else

static inline int
GAMERA_PyObject_CheckReadBuffer(PyObject *obj)
{
    return PyObject_CheckBuffer(obj);
}

static inline int
GAMERA_PyObject_AsReadBuffer(PyObject *obj, const void **buf, Py_ssize_t *len)
{
    Py_buffer view;
    if (PyObject_GetBuffer(obj, &view, PyBUF_SIMPLE) < 0)
        return -1;
    *buf = view.buf;
    *len = view.len;
    PyBuffer_Release(&view);
    return 0;
}

static inline int
GAMERA_PyObject_AsWriteBuffer(PyObject *obj, void **buf, Py_ssize_t *len)
{
    Py_buffer view;
    if (PyObject_GetBuffer(obj, &view, PyBUF_WRITABLE) < 0)
        return -1;
    *buf = view.buf;
    *len = view.len;
    PyBuffer_Release(&view);
    return 0;
}

#endif

#endif /* GAMERA_PYCOMPAT_HPP */
