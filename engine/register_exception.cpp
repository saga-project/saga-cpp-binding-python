//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <stdexcept>
#include <iostream>

#include <saga/saga.hpp>
#include <boost/function.hpp>
#include <boost/python.hpp>


#include "register_types.hpp"
#include "container_mappings.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
namespace translators
{
    template <typename Exception>
    struct exception
    {
        static void register_()
        {
            register_exception_translator<Exception>(&exception::translate);

            // Register custom r-value converter
            // There are situations, where we have to pass the exception back  
            // to C++ library. This will do the trick
            converter::registry::push_back(&exception::convertible,
                &exception::construct, type_id<Exception>());
        }

        static void translate(Exception const& err)
        {
            object pimpl_err(err);
            object pyerr_class = pimpl_err.attr("py_err_class");
            object pyerr = pyerr_class(pimpl_err);
            PyErr_SetObject(pyerr_class.ptr(), incref(pyerr.ptr()));
        }

        // Sometimes, exceptions should be passed back to the library.
        static void* convertible(PyObject* py_obj)
        {
            if (1 != PyObject_IsInstance(py_obj, PyExc_Exception))
                return 0;

            if (!PyObject_HasAttrString(py_obj, "_pimpl"))
                return 0;

            object pyerr(handle<>(borrowed(py_obj)));
            object pimpl = getattr(pyerr, "_pimpl");
            extract<saga::exception> type_checker(pimpl);
            if (!type_checker.check())
                return 0;

            return py_obj;
        }

        static void 
        construct (PyObject* py_obj, converter::rvalue_from_python_stage1_data* data)
        {
            typedef converter::rvalue_from_python_storage<Exception> 
                storage_type;

            object pyerr(handle<>(borrowed(py_obj)));
            object pimpl = getattr(pyerr, "_pimpl");

            storage_type* the_storage = reinterpret_cast<storage_type*>(data);
            void* memory_chunk = the_storage->storage.bytes;
            /* Exception* cpp_err = */
                new (memory_chunk) Exception(extract<Exception>(pimpl));

            data->convertible = memory_chunk;
        }
    };

    struct already_thrown_exception
    {
        static void register_()
        {
            register_exception_translator<boost::python::error_already_set>(
                &already_thrown_exception::translate);
        }

        static void translate(boost::python::error_already_set const& err)
        {
            // isolate exceptions of type "Boost.Python.ArgumentError" and
            // translate them into saga::bad_parameter exceptions
            if (PyErr_ExceptionMatches(PyExc_TypeError)) {
                PyObject *type(NULL), *value(NULL), *traceback(NULL);
                PyErr_Fetch(&type, &value, &traceback);

                if (NULL != type && NULL != value) {
                    object t (handle<>(PyObject_Str(type)));
                    std::string cls(PyString_AS_STRING(t.ptr()));

                    if (cls.find("Boost.Python.ArgumentError")) {
                        object excmsg (handle<>(PyObject_Str(value)));
                        std::string cls(PyString_AS_STRING(excmsg.ptr()));

                        decref(type);
                        decref(value);
                        xdecref(traceback);

                        SAGA_THROW_NO_OBJECT(cls, saga::BadParameter);
                    }
                }

                // that's something different, just rethrow
                PyErr_Restore(type, value, traceback);
            }
            throw err;
        }
    };

///////////////////////////////////////////////////////////////////////////////
}

///////////////////////////////////////////////////////////////////////////////
//  wrapper for exception functions (needed because Boost.Python gets confused 
//  by the throw() specifiers
static char const* py_get_exception_message(saga::exception const& o) 
{
    return o.get_message();
}

static char const* py_get_full_exception_message(saga::exception const& o) 
{
    return o.what();
}

static saga::error py_get_exception_error(saga::exception const& o) 
{
    return o.get_error();
}

static std::vector<saga::exception> py_get_all_exceptions(saga::exception const& o) 
{
    return o.get_all_exceptions();
}

static saga::object py_get_exception_object(saga::exception const& o) 
{
    return o.get_object();
}

///////////////////////////////////////////////////////////////////////////////
void register_exceptions()
{
    // std::vector<saga::exception>
    saga::python::tuple_mapping_variable_capacity<std::vector<saga::exception> >();

    class_<saga::exception>("_exception", "saga::exception type", no_init)
        .def("get_full_message", &py_get_full_exception_message, 
            "get the full exception message")
        .def("__str__", &py_get_exception_message, 
            "get the full exception message")
        .def("get_message", &py_get_exception_message, 
            "get the top most (most significant) exception message")
        .def("get_error", &py_get_exception_error, 
            "get the error type of this exception")
        .def("get_object", &py_get_exception_object, 
            "get the saga::object associated with this exception")
        .def("get_all_exceptions", &py_get_all_exceptions,
            "get the list of all exceptions encapsulated in this exception")
        .def("get_all_messages", &saga::exception::get_all_messages,
            "get the list of all messages encapsulated in this exception")
    ;

    class_<saga::not_implemented, bases<saga::exception> >(
        "_not_implemented", "saga::not_implemented type", no_init)
    ;

    class_<saga::parameter_exception, bases<saga::exception> >(
        "_parameter_exception", "saga::parameter_exception type", no_init)
    ;

    class_<saga::incorrect_url, bases<saga::parameter_exception> >(
        "_incorrect_url", "saga::incorrect_url type", no_init)
    ;

    class_<saga::bad_parameter, bases<saga::parameter_exception> >(
        "_bad_parameter", "saga::bad_parameter type", no_init)
    ;

    class_<saga::state_exception, bases<saga::exception> >(
        "_state_exception", "saga::state_exception type", no_init)
    ;

    class_<saga::already_exists, bases<saga::state_exception> >(
        "_already_exists", "saga::already_exists type", no_init)
    ;

    class_<saga::does_not_exist, bases<saga::state_exception> >(
        "_does_not_exist", "saga::does_not_exist type", no_init)
    ;

    class_<saga::incorrect_state, bases<saga::state_exception> >(
        "_incorrect_state", "saga::incorrect_state type", no_init)
    ;

    class_<saga::timeout, bases<saga::state_exception> >(
        "_timeout", "saga::timeout type", no_init)
    ;

    class_<saga::security_exception, bases<saga::exception> >(
        "_security_exception", "saga::security_exception type", no_init)
    ;

    class_<saga::permission_denied, bases<saga::security_exception> >(
        "_permission_denied", "saga::permission_denied type", no_init)
    ;

    class_<saga::authorization_failed, bases<saga::security_exception> >(
        "_authorization_failed", "saga::authorization_failed type", no_init)
    ;

    class_<saga::authentication_failed, bases<saga::security_exception> >(
        "_authentication_failed", "saga::authentication_failed type", no_init)
    ;

    class_<saga::no_success, bases<saga::exception> >(
        "_no_success", "saga::no_success type", no_init)
    ;

    // saga::error
    enum_<saga::error>("error")
        .value("NotImplemented", saga::NotImplemented)
        .value("IncorrectURL", saga::IncorrectURL)
        .value("BadParameter", saga::BadParameter)
        .value("AlreadyExists", saga::AlreadyExists)
        .value("DoesNotExist", saga::DoesNotExist)
        .value("IncorrectState", saga::IncorrectState)
        .value("PermissionDenied", saga::PermissionDenied)
        .value("AuthorizationFailed", saga::AuthorizationFailed)
        .value("AuthenticationFailed", saga::AuthenticationFailed)
        .value("Timeout", saga::Timeout)
        .value("NoSuccess", saga::NoSuccess)
    ;

    translators::exception<saga::exception>::register_();
    translators::exception<saga::not_implemented>::register_();
    translators::exception<saga::parameter_exception>::register_();
    translators::exception<saga::incorrect_url>::register_();
    translators::exception<saga::bad_parameter>::register_();
    translators::exception<saga::state_exception>::register_();
    translators::exception<saga::already_exists>::register_();
    translators::exception<saga::does_not_exist>::register_();
    translators::exception<saga::incorrect_state>::register_();
    translators::exception<saga::timeout>::register_();
    translators::exception<saga::security_exception>::register_();
    translators::exception<saga::permission_denied>::register_();
    translators::exception<saga::authorization_failed>::register_();
    translators::exception<saga::authentication_failed>::register_();
    translators::exception<saga::no_success>::register_();

    // needs to be the last registered exception translator as it needs to 
    // be invoked as the first one (allowing to rethrow saga::exceptions while
    // translating the caught exception)
    translators::already_thrown_exception::register_();
}
