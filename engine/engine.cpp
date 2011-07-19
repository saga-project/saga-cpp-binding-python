//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include <boost/function.hpp>
#include <boost/python.hpp>


#include "register_types.hpp"

using namespace boost::python;

#if defined(SAGA_APPLE)
#include <crt_externs.h>
char **environ = *_NSGetEnviron();
#endif

///////////////////////////////////////////////////////////////////////////////
struct init_python_interpreter
{
    init_python_interpreter()
    :   initialized_(false)
    {
        // initialize interpreter
        if (!Py_IsInitialized()) {
            initialized_ = true;
            Py_Initialize();
        }
    }
    ~init_python_interpreter()
    {
        // finalize interpreter
        if (initialized_)
            Py_Finalize();
    }

    bool initialized_;
};

init_python_interpreter ini_python;

///////////////////////////////////////////////////////////////////////////////
BOOST_PYTHON_MODULE(_engine)
{
#if PY_VERSION_HEX >= 0x02040000
// initialize threading
    PyEval_InitThreads();
//     PyThreadState* mainState = PyThreadState_Swap(NULL);
#endif

// register the extensions provided by this module
    register_version();
    register_exceptions();

    register_url();             // saga::url
    register_uuid();            // saga::uuid
//     register_buffer();          // saga::mutable_buffer, saga::const_buffer, saga::buffer

    register_object();          // saga::object
    register_context();         // saga::context
    register_session();         // saga::session
    register_task();            // saga::task
    register_task_container();  // saga::task_container
    register_metric();          // saga::metric

#if PY_VERSION_HEX >= 0x02040000
// restore thread context
    PyThreadState_Swap(PyThreadState_Get());
#endif
}

