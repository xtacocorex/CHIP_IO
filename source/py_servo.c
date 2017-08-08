/*
Copyright (c) 2017 Robert Wolterman

Using CHIP_IO servo code from Brady Hurlburt as a basis for the servo code

Copyright (c) 2016 Brady Hurlburt

Original BBIO Author Justin Cooper
Modified for CHIP_IO Author Brady Hurlburt

This file incorporates work covered by the following copyright and
permission notice, all modified code adopts the original license:

Copyright (c) 2013 Adafruit
Author: Justin Cooper

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "Python.h"
#include "constants.h"
#include "common.h"
#include "c_softservo.h"

// python function toggle_debug()
static PyObject *py_toggle_debug(PyObject *self, PyObject *args)
{
    // toggle debug printing
    toggle_debug();

    Py_RETURN_NONE;
}

// python function cleanup()
static PyObject *py_cleanup(PyObject *self, PyObject *args)
{
    // TODO: PER PIN CLEANUP LIKE EVERYTHING ELSE

    // unexport the Servo
    servo_cleanup();

    Py_RETURN_NONE;
}

static int init_module(void)
{
    clear_error_msg();
    
    // If we make it here, we're good to go
    if (DEBUG)
        printf(" ** init_module: setup complete **\n");
    module_setup = 1;

    return 0;
}

// python function value = is_chip_pro()
static PyObject *py_is_chip_pro(PyObject *self, PyObject *args)
{
    PyObject *py_value;
    
    py_value = Py_BuildValue("i", is_this_chippro());

    return py_value;
}

// python function start(channel, angle, range)
static PyObject *py_start_channel(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int gpio;
    char key[8];
    char *channel = NULL;
    float angle = 0.0;
    float range = 180.0;
    int allowed = -1;
    static char *kwlist[] = {"channel", "angle", "range", NULL};

    clear_error_msg();

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|ff", kwlist, &channel, &angle, &range)) {
        return NULL;
    }
    ASSRT(channel != NULL);

    if (!module_setup) {
        init_module();
    }

    if (!get_key(channel, key)) {
        PyErr_SetString(PyExc_ValueError, "Invalid Servo key or name.");
        return NULL;
    }

    // check to ensure gpio is one of the allowed pins
    // Not protecting the call as if the get_key() fails, we won't make it here
    get_gpio_number(channel, &gpio);
    if ((gpio >= lookup_gpio_by_name("XIO-P0") && gpio <= lookup_gpio_by_name("XIO-P7"))) {
        PyErr_SetString(PyExc_ValueError, "Servo currently not available on XIO-P0 to XIO-P7");
        return NULL;
    }

    // Check to see if GPIO is allowed on the hardware
    // A 1 means we're good to go
    allowed = gpio_allowed(gpio);
    if (allowed == -1) {
        char err[2000];
        snprintf(err, sizeof(err), "Error determining hardware. (%s)", get_error_msg());
        PyErr_SetString(PyExc_ValueError, err);
        return NULL;
    } else if (allowed == 0) {
        char err[2000];
        snprintf(err, sizeof(err), "GPIO %d not available on current Hardware", gpio);
        PyErr_SetString(PyExc_ValueError, err);
        return NULL;
    }

    if (servo_start(key, angle, range) < 0) {
       printf("servo_start failed");
       char err[2000];
       snprintf(err, sizeof(err), "Error starting servo on pin %s (%s)", key, get_error_msg());
       PyErr_SetString(PyExc_RuntimeError, err);
       return NULL;
    }

    Py_RETURN_NONE;
}

// python function stop(channel)
static PyObject *py_stop_channel(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int gpio;
    char key[8];
    int allowed = -1;
    char *channel;

    clear_error_msg();

    if (!PyArg_ParseTuple(args, "s", &channel))
        return NULL;

    if (!get_key(channel, key)) {
        PyErr_SetString(PyExc_ValueError, "Invalid key or name");
        return NULL;
    }

    // check to ensure gpio is one of the allowed pins
    // Not protecting the call as if the get_key() fails, we won't make it here
    get_gpio_number(channel, &gpio);
    if ((gpio >= lookup_gpio_by_name("XIO-P0") && gpio <= lookup_gpio_by_name("XIO-P7"))) {
        PyErr_SetString(PyExc_ValueError, "Servo currently not available on XIO-P0 to XIO-P7");
        return NULL;
    }

    // Check to see if GPIO is allowed on the hardware
    // A 1 means we're good to go
    allowed = gpio_allowed(gpio);
    if (allowed == -1) {
        char err[2000];
        snprintf(err, sizeof(err), "Error determining hardware. (%s)", get_error_msg());
        PyErr_SetString(PyExc_ValueError, err);
        return NULL;
    } else if (allowed == 0) {
        char err[2000];
        snprintf(err, sizeof(err), "GPIO %d not available on current Hardware", gpio);
        PyErr_SetString(PyExc_ValueError, err);
        return NULL;
    }

    servo_disable(key);

    Py_RETURN_NONE;
}

// python method SERVO.set_range(channel, range)
static PyObject *py_set_range(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int gpio;
    char key[8];
    char *channel;
    float range = 180.0;
    int allowed = -1;
    static char *kwlist[] = {"channel", "range", NULL};

    clear_error_msg();

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|f", kwlist, &channel, &range))
        return NULL;

    if (range > 360.0) {
        PyErr_SetString(PyExc_ValueError, "range can not be greater than 360.0");
        return NULL;
    }

    if (!get_key(channel, key)) {
        PyErr_SetString(PyExc_ValueError, "Invalid key or name.");
        return NULL;
    }

    // check to ensure gpio is one of the allowed pins
    // Not protecting the call as if the get_key() fails, we won't make it here
    get_gpio_number(channel, &gpio);
    if ((gpio >= lookup_gpio_by_name("XIO-P0") && gpio <= lookup_gpio_by_name("XIO-P7"))) {
        PyErr_SetString(PyExc_ValueError, "Servo currently not available on XIO-P0 to XIO-P7");
        return NULL;
    }

    // Check to see if GPIO is allowed on the hardware
    // A 1 means we're good to go
    allowed = gpio_allowed(gpio);
    if (allowed == -1) {
        char err[2000];
        snprintf(err, sizeof(err), "Error determining hardware. (%s)", get_error_msg());
        PyErr_SetString(PyExc_ValueError, err);
        return NULL;
    } else if (allowed == 0) {
        char err[2000];
        snprintf(err, sizeof(err), "GPIO %d not available on current Hardware", gpio);
        PyErr_SetString(PyExc_ValueError, err);
        return NULL;
    }

    if (servo_set_range(key, range) == -1) {
        PyErr_SetString(PyExc_RuntimeError, "You must start() the Servo channel first");
        return NULL;
    }

    Py_RETURN_NONE;
}

// python method SERVO.set_angle(channel, angle)
static PyObject *py_set_angle(PyObject *self, PyObject *args, PyObject *kwargs)
{
    int gpio;
    char key[8];
    char *channel;
    float angle = 0.0;
    int allowed = -1;
    static char *kwlist[] = {"channel", "angle", NULL};

    clear_error_msg();

    if (!PyArg_ParseTupleAndKeywords(args, kwargs, "s|f", kwlist, &channel, &angle))
        return NULL;

    //if (range > 360.0) {
    //    PyErr_SetString(PyExc_ValueError, "range can not be greater than 360.0");
    //    return NULL;
    //}

    if (!get_key(channel, key)) {
        PyErr_SetString(PyExc_ValueError, "Invalid key or name.");
        return NULL;
    }

    // check to ensure gpio is one of the allowed pins
    // Not protecting the call as if the get_key() fails, we won't make it here
    get_gpio_number(channel, &gpio);
    if ((gpio >= lookup_gpio_by_name("XIO-P0") && gpio <= lookup_gpio_by_name("XIO-P7"))) {
        PyErr_SetString(PyExc_ValueError, "Servo currently not available on XIO-P0 to XIO-P7");
        return NULL;
    }

    // Check to see if GPIO is allowed on the hardware
    // A 1 means we're good to go
    allowed = gpio_allowed(gpio);
    if (allowed == -1) {
        char err[2000];
        snprintf(err, sizeof(err), "Error determining hardware. (%s)", get_error_msg());
        PyErr_SetString(PyExc_ValueError, err);
        return NULL;
    } else if (allowed == 0) {
        char err[2000];
        snprintf(err, sizeof(err), "GPIO %d not available on current Hardware", gpio);
        PyErr_SetString(PyExc_ValueError, err);
        return NULL;
    }

    if (servo_set_angle(key, angle) == -1) {
       char err[2000];
       snprintf(err, sizeof(err), "Error setting servo angle on pin %s (%s)", key, get_error_msg());
       PyErr_SetString(PyExc_RuntimeError, err);
       return NULL;
    }

    Py_RETURN_NONE;
}

static const char moduledocstring[] = "Software Servo functionality of a CHIP using Python";

PyMethodDef servo_methods[] = {
    {"start", (PyCFunction)py_start_channel, METH_VARARGS | METH_KEYWORDS, "Set up and start the Servo.  channel can be in the form of 'XIO-P0', or 'U14_13'"},
    {"stop", (PyCFunction)py_stop_channel, METH_VARARGS | METH_KEYWORDS, "Stop the Servo.  channel can be in the form of 'XIO-P0', or 'U14_13'"},
    {"set_range", (PyCFunction)py_set_range, METH_VARARGS, "Change the servo range\nrange - max angular range of the servo" },
    {"set_angle", (PyCFunction)py_set_angle, METH_VARARGS, "Change the servo angle\nangle - angle of the servo between +/-(range/2)" },
    {"cleanup", (PyCFunction)py_cleanup, METH_VARARGS, "Clean up by resetting All or one Servo that have been used by this program."},
    {"toggle_debug", py_toggle_debug, METH_VARARGS, "Toggles the enabling/disabling of Debug print output"},
    {"is_chip_pro", py_is_chip_pro, METH_VARARGS, "Is hardware a CHIP Pro? Boolean False for normal CHIP/PocketCHIP (R8 SOC)"},
    {NULL, NULL, 0, NULL}
};

#if PY_MAJOR_VERSION > 2
static struct PyModuleDef chipservomodule = {
    PyModuleDef_HEAD_INIT,
    "SERVO",       // name of module
    moduledocstring,  // module documentation, may be NULL
    -1,               // size of per-interpreter state of the module, or -1 if the module keeps state in global variables.
    servo_methods
};
#endif

#if PY_MAJOR_VERSION > 2
PyMODINIT_FUNC PyInit_SERVO(void)
#else
PyMODINIT_FUNC initSERVO(void)
#endif
{
    PyObject *module = NULL;

#if PY_MAJOR_VERSION > 2
    if ((module = PyModule_Create(&chipservomodule)) == NULL)
       return NULL;
#else
    if ((module = Py_InitModule3("SERVO", servo_methods, moduledocstring)) == NULL)
       return;
#endif

   define_constants(module);


#if PY_MAJOR_VERSION > 2
    return module;
#else
    return;
#endif
}
