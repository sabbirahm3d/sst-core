// -*- c++ -*-

// Copyright 2009-2018 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
//
// Copyright (c) 2009-2018, NTESS
// All rights reserved.
//
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.

#include "sst_config.h"
#include <Python.h>

#include <string.h>
#include <sstream>

#include <sst/core/warnmacros.h>
#include <sst/core/model/pymodel.h>
#include <sst/core/model/pymodel_unitalgebra.h>

#include <sst/core/unitAlgebra.h>

using namespace SST::Core;
extern SST::Core::SSTPythonModelDefinition *gModel;

extern "C" {


static int unitAlgebraInit(UnitAlgebraPy_t *self, PyObject *args, PyObject *UNUSED(kwds))
{
    char *init_str = NULL;
    // PyObject* obj;
    UnitAlgebraPy_t* obj;
    
    
    if ( PyArg_ParseTuple(args, "s", &init_str) ) {
        self->obj = new UnitAlgebra(init_str);
        return 0;
    }
    PyErr_Clear();
    if ( PyArg_ParseTuple(args, "O!", &PyModel_UnitAlgebraType, &obj) ) {
        self->obj = new UnitAlgebra(*(obj->obj));
        return 0;
    }
    PyErr_SetString(PyExc_TypeError,"sst.UnitAlgebra can only be initialized with another sst.UnitAlgebra or with a formatted string");
    return -1;
}

static void unitAlgebraDealloc(UnitAlgebraPy_t *self)
{
    delete self->obj;
    self->ob_type->tp_free((PyObject*)self);
}
    
static PyObject* unitAlgebraStr(PyObject* self)
{
    UnitAlgebraPy_t *ua = (UnitAlgebraPy_t*)self;
    return PyString_FromString(ua->obj->toStringBestSI().c_str());
}
    
static PyObject* unitAlgebraRichCmp (PyObject *self, PyObject *other, int op)
{
    UnitAlgebraPy_t *self_ua = (UnitAlgebraPy_t*)self;
    UnitAlgebraPy_t *other_ua = (UnitAlgebraPy_t*)other;
    switch (op) {
    case Py_LT:
        if ( *(self_ua->obj) < *(other_ua->obj) ) return Py_True;
        else return Py_False;
        break;
    case Py_LE:
        if ( *(self_ua->obj) <= *(other_ua->obj) ) return Py_True;
        else return Py_False;
        break;
    case Py_GT:
        if ( *(self_ua->obj) > *(other_ua->obj) ) return Py_True;
        else return Py_False;
        break;
    case Py_GE:
        if ( *(self_ua->obj) >= *(other_ua->obj) ) return Py_True;
        else return Py_False;
        break;
    case Py_EQ:
    case Py_NE:
    default:
        return Py_NotImplemented;
        break;
    }
    return Py_NotImplemented;
}


// Numerical functions
static PyObject* unitAlgebraAddIP(PyObject *self, PyObject *other)
{
    UnitAlgebraPy_t *self_ua = (UnitAlgebraPy_t*)self;
    UnitAlgebraPy_t *other_ua = (UnitAlgebraPy_t*)other;
    *(self_ua->obj) += *(other_ua->obj);

    PyObject *argList = Py_BuildValue("(O)", self);
    PyObject* ret = PyObject_CallObject((PyObject*)&PyModel_UnitAlgebraType, argList);    
    Py_DECREF(argList);
    
    return ret;
}

static PyObject* unitAlgebraAdd (PyObject *self, PyObject *other)
{
    PyObject *argList = Py_BuildValue("(O)", self);
    PyObject* ret = PyObject_CallObject((PyObject*)&PyModel_UnitAlgebraType, argList);    
    Py_DECREF(argList);
    
    UnitAlgebraPy_t *ret_ua = (UnitAlgebraPy_t*)ret;
    UnitAlgebraPy_t *other_ua = (UnitAlgebraPy_t*)other;
    *(ret_ua->obj) += *(other_ua->obj);
    return ret;
}
    
    
static PyObject* unitAlgebraSubIP(PyObject *self, PyObject *other)
{
    UnitAlgebraPy_t *self_ua = (UnitAlgebraPy_t*)self;
    UnitAlgebraPy_t *other_ua = (UnitAlgebraPy_t*)other;
    *(self_ua->obj) -= *(other_ua->obj);

    PyObject *argList = Py_BuildValue("(O)", self);
    PyObject* ret = PyObject_CallObject((PyObject*)&PyModel_UnitAlgebraType, argList);    
    Py_DECREF(argList);
    
    return ret;
}

static PyObject* unitAlgebraSub (PyObject *self, PyObject *other)
{
    PyObject *argList = Py_BuildValue("(O)", self);
    PyObject* ret = PyObject_CallObject((PyObject*)&PyModel_UnitAlgebraType, argList);    
    Py_DECREF(argList);

    UnitAlgebraPy_t *ret_ua = (UnitAlgebraPy_t*)ret;
    UnitAlgebraPy_t *other_ua = (UnitAlgebraPy_t*)other;
    *(ret_ua->obj) -= *(other_ua->obj);
    return ret;
}
    
static PyObject* unitAlgebraMulIP(PyObject *self, PyObject *other)
{
    UnitAlgebraPy_t *self_ua = (UnitAlgebraPy_t*)self;
    UnitAlgebraPy_t *other_ua = (UnitAlgebraPy_t*)other;
    *(self_ua->obj) *= *(other_ua->obj);

    PyObject *argList = Py_BuildValue("(O)", self);
    PyObject* ret = PyObject_CallObject((PyObject*)&PyModel_UnitAlgebraType, argList);    
    Py_DECREF(argList);
    
    return ret;
}

static PyObject* unitAlgebraMul (PyObject *self, PyObject *other)
{
    PyObject *argList = Py_BuildValue("(O)", self);
    PyObject* ret = PyObject_CallObject((PyObject*)&PyModel_UnitAlgebraType, argList);    
    Py_DECREF(argList);

    UnitAlgebraPy_t *ret_ua = (UnitAlgebraPy_t*)ret;
    UnitAlgebraPy_t *other_ua = (UnitAlgebraPy_t*)other;
    *(ret_ua->obj) *= *(other_ua->obj);
    return ret;
}
    
static PyObject* unitAlgebraDivIP(PyObject *self, PyObject *other)
{
    UnitAlgebraPy_t *self_ua = (UnitAlgebraPy_t*)self;
    UnitAlgebraPy_t *other_ua = (UnitAlgebraPy_t*)other;
    *(self_ua->obj) /= *(other_ua->obj);

    PyObject *argList = Py_BuildValue("(O)", self);
    PyObject* ret = PyObject_CallObject((PyObject*)&PyModel_UnitAlgebraType, argList);    
    Py_DECREF(argList);
    
    return ret;
}

static PyObject* unitAlgebraDiv (PyObject *self, PyObject *other)
{
    PyObject *argList = Py_BuildValue("(O)", self);
    PyObject* ret = PyObject_CallObject((PyObject*)&PyModel_UnitAlgebraType, argList);    
    Py_DECREF(argList);

    UnitAlgebraPy_t *ret_ua = (UnitAlgebraPy_t*)ret;
    UnitAlgebraPy_t *other_ua = (UnitAlgebraPy_t*)other;
    *(ret_ua->obj) /= *(other_ua->obj);
    return ret;
}

static PyObject* unitAlgebraToLong(PyObject *self)
{
    UnitAlgebraPy_t *self_ua = (UnitAlgebraPy_t*)self;
    int64_t val = self_ua->obj->getRoundedValue();
    PyObject* ret = PyLong_FromLong(val);
    return ret;
}
    
PyNumberMethods PyModel_UnitAlgebraNumMeth = {
    (binaryfunc)unitAlgebraAdd,    // binaryfunc nb_add
    (binaryfunc)unitAlgebraSub,    // binaryfunc nb_subtract
    (binaryfunc)unitAlgebraMul,    // binaryfunc nb_multiply
    (binaryfunc)unitAlgebraDiv,    // binaryfunc nb_divide
    0,                // binaryfunc nb_remainder
    0,                // binaryfunc nb_divmod
    0,                // ternaryfunc nb_power
    0,                // unaryfunc nb_negative
    0,                // unaryfunc nb_positive
    0,                // unaryfunc nb_absolute
    0,                // inquiry nb_nonzero       /* Used by PyObject_IsTrue */
    0,                // unaryfunc nb_invert
    0,                // binaryfunc nb_lshift
    0,                // binaryfunc nb_rshift
    0,                // binaryfunc nb_and
    0,                // binaryfunc nb_xor
    0,                // binaryfunc nb_or
    0,                // coercion nb_coerce       /* Used by the coerce() function */
    0,                // unaryfunc nb_int
    (unaryfunc)unitAlgebraToLong,  // unaryfunc nb_long
    0,                // unaryfunc nb_float
    0,                // unaryfunc nb_oct
    0,                // unaryfunc nb_hex
    
    /* Added in release 2.0 */
    (binaryfunc)unitAlgebraAddIP,  // binaryfunc nb_inplace_add
    (binaryfunc)unitAlgebraSubIP,  // binaryfunc nb_inplace_subtract
    (binaryfunc)unitAlgebraMulIP,  // binaryfunc nb_inplace_multiply
    (binaryfunc)unitAlgebraDivIP,  // binaryfunc nb_inplace_divide
    0,                // binaryfunc nb_inplace_remainder
    0,                // ternaryfunc nb_inplace_power
    0,                // binaryfunc nb_inplace_lshift
    0,                // binaryfunc nb_inplace_rshift
    0,                // binaryfunc nb_inplace_and
    0,                // binaryfunc nb_inplace_xor
    0,                // binaryfunc nb_inplace_or
    
    /* Added in release 2.2 */
    0,                // binaryfunc nb_floor_divide
    0,                // binaryfunc nb_true_divide
    0,                // binaryfunc nb_inplace_floor_divide
    0,                // binaryfunc nb_inplace_true_divide
    
    /* Added in release 2.5 */
    0,                // unaryfunc nb_index
}; 
    
// Other methods

static PyObject* unitAlgebraGetRoundedValue(PyObject *self, PyObject *UNUSED(args))
{
    return unitAlgebraToLong(self);
}

static PyObject* unitAlgebraHasUnits(PyObject* self, PyObject* args)
{
    char *units = NULL;
        
    if ( PyArg_ParseTuple(args, "s", &units) ) {
        UnitAlgebraPy_t *ua = (UnitAlgebraPy_t*)self;
        if ( ua->obj->hasUnits(units) ) Py_RETURN_TRUE;
    }
    Py_RETURN_FALSE;
}

static PyObject* unitAlgebraInvert(PyObject *self, PyObject *UNUSED(args))
{
    UnitAlgebraPy_t *ua = (UnitAlgebraPy_t*)self;
    ua->obj->invert();
    Py_INCREF(self);
    return self;
}

static PyMethodDef unitAlgebraMethods[] = {
    {   "getRoundedValue",
        unitAlgebraGetRoundedValue, METH_NOARGS,
        "Rounds value of UnitAlgebra to nearest whole number and returns a long"},
    {   "hasUnits",
        unitAlgebraHasUnits, METH_VARARGS,
        "Checks to see if the UnitAlgebra has the specified units"},
    {   "invert",
        unitAlgebraInvert, METH_NOARGS,
        "Inverts (1/X) the number and unit"},
    // {   "str",
    //     unitAlgebraStr, METH_NOARGS,
    //     "Creates a string representation of the UnitAlgebra."},
    {   NULL, NULL, 0, NULL }
};



PyTypeObject PyModel_UnitAlgebraType = {
    PyObject_HEAD_INIT(NULL)
    0,                         /* ob_size */
    "sst.UnitAlgebra",         /* tp_name */
    sizeof(UnitAlgebraPy_t),   /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)unitAlgebraDealloc,   /* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    &PyModel_UnitAlgebraNumMeth,/* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    (reprfunc)unitAlgebraStr,  /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,        /* tp_flags */
    "SST UnitAlgebra",         /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    (richcmpfunc)unitAlgebraRichCmp,  /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    unitAlgebraMethods,               /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)unitAlgebraInit, /* tp_init */
    0,                         /* tp_alloc */
    0,                         /* tp_new */
    0,                         /* tp_free */
    0,                         /* tp_is_gc */
    0,                         /* tp_bases */
    0,                         /* tp_mro */
    0,                         /* tp_cache */
    0,                         /* tp_subclasses */
    0,                         /* tp_weaklist */
    0,                         /* tp_del */
    0,                         /* tp_version_tag */
};



}  /* extern C */


