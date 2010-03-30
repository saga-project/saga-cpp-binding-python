//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>

#include <boost/function.hpp>
#include <boost/python.hpp>

#include <saga/saga.hpp>

#include "register_types.hpp"
#include "../../engine/container_mappings.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_1(std::string, saga::advert::entry, get_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::advert::entry, set_attribute, std::string, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::advert::entry, get_vector_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::advert::entry, set_vector_attribute, std::string, std::vector<std::string>)
DEFINE_PYTHON_WRAPPERS_0(std::vector<std::string>, saga::advert::entry, list_attributes)
DEFINE_PYTHON_WRAPPERS_1(void, saga::advert::entry, remove_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::advert::entry, find_attributes, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::advert::entry, attribute_exists, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::advert::entry, attribute_is_readonly, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::advert::entry, attribute_is_writable, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::advert::entry, attribute_is_vector, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::advert::entry, attribute_is_removable, std::string)

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_0(saga::url, saga::stream::stream, get_url)
DEFINE_PYTHON_WRAPPERS_0(saga::context, saga::stream::stream, get_context)
DEFINE_PYTHON_WRAPPERS_1_DEF_1(void, saga::stream::stream, connect, double, -1.0)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(std::vector<saga::stream::activity>, 
    saga::stream::stream, wait, saga::stream::activity, double, -1.0)
DEFINE_PYTHON_WRAPPERS_1_DEF_1(void, saga::stream::stream, close, double, 0.0)
// DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::ssize_t, saga::stream::stream, read, 
//     saga::mutable_buffer, saga::ssize_t, 0)
// DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::ssize_t, saga::stream::stream, write, 
//     saga::const_buffer, saga::ssize_t, 0)

///////////////////////////////////////////////////////////////////////////////
static saga::monitorable::cookie_handle 
add_stream_stream_cb(saga::stream::stream s, std::string name, 
  boost::python::object f)
{
    return s.add_callback(name, saga::python::python_callback(f));
}

///////////////////////////////////////////////////////////////////////////////
static saga::monitorable::cookie_handle 
add_stream_stream_cb_obj(saga::stream::stream s, std::string name, 
    boost::python::object o, boost::python::object f)
{
    return s.add_callback(name, saga::python::python_callback_obj(o, f));
}

///////////////////////////////////////////////////////////////////////////////
static saga::ssize_t py_write_1(saga::stream::stream s, 
    boost::python::object o)
{
    if (!PyString_Check(o.ptr())) {
        // parameter is not a string object, throw an error
        SAGA_THROW_VERBATIM(s,
            "First parameter needs to be a string object",
            saga::BadParameter);
        return 0;
    }
    saga::ssize_t len = PyString_Size(o.ptr());
    return s.write(saga::const_buffer(PyString_AsString(o.ptr()), len));
}

template <typename Type>
saga::task py_write_1_type(saga::stream::stream s, boost::python::object o)
{
    if (!PyString_Check(o.ptr())) {
        // parameter is not a string object, throw an error
        SAGA_THROW_VERBATIM(s,
            "First parameter needs to be a string object",
            saga::BadParameter);
        return saga::task();
    }
    saga::ssize_t len = PyString_Size(o.ptr());
    return s.write<Type>(saga::const_buffer(PyString_AsString(o.ptr()), len));
}

static saga::task py_write_task_1(saga::stream::stream s, 
    saga::python::routine_type type, boost::python::object o)
{
    switch(type) {
    case saga::python::Sync: 
        return py_write_1_type<saga::task_base::Sync>(s, o);
    case saga::python::Async: 
        return py_write_1_type<saga::task_base::Async>(s, o);
    case saga::python::Task: 
        return py_write_1_type<saga::task_base::Task>(s, o);
    default:
        SAGA_THROW_VERBATIM(s,
            "Invalid routine type (not Sync, Async or Task)",
            saga::BadParameter);
        break;
    }
    return saga::task();
}

///////////////////////////////////////////////////////////////////////////////
static boost::python::handle<> py_read_1(saga::stream::stream s, 
    std::size_t size = -1)
{
    saga::mutable_buffer buf(size);
    /*saga::ssize_t r = */s.read(buf, (size == std::size_t(-1)) ? 0 : size);
    return boost::python::handle<>(PyString_FromStringAndSize(
        (char const*)buf.get_data(), buf.get_size()));
}
BOOST_PYTHON_FUNCTION_OVERLOADS(py_read_1_ov, py_read_1, 1, 2)

template <typename Type>
saga::task py_read_1_type(saga::stream::stream s, std::size_t size)
{
    saga::mutable_buffer buf(size);
    return s.read<Type>(buf, (size == std::size_t(-1)) ? 0 : size);
}

static saga::task py_read_task_1(saga::stream::stream s, 
    saga::python::routine_type type, std::size_t size = -1)
{
    switch(type) {
    case saga::python::Sync: 
        return py_read_1_type<saga::task_base::Sync>(s, size);
    case saga::python::Async: 
        return py_read_1_type<saga::task_base::Async>(s, size);
    case saga::python::Task: 
        return py_read_1_type<saga::task_base::Task>(s, size);
    default:
        SAGA_THROW_VERBATIM(s,
            "Invalid routine type (not Sync, Async or Task)",
            saga::BadParameter);
        break;
    }
    return saga::task();
}
BOOST_PYTHON_FUNCTION_OVERLOADS(py_read_task_1_ov, py_read_task_1, 2, 3)

///////////////////////////////////////////////////////////////////////////////
void register_stream_stream()
{
    // std::vector<saga::exception>
    saga::python::tuple_mapping_variable_capacity<std::vector<saga::stream::activity> >();

    {
    scope stream_stream = // saga::stream::stream
        class_<saga::stream::stream, bases<saga::object> >("stream")

            // additional constructors
            .def(init<std::string>())
            .def(init<saga::url>())
            .def(init<saga::session>())
            .def(init<saga::session, saga::url>())
            .def(init<saga::session, std::string>())
            
            // saga::attribute interface 
            INSERT_PYTHON_WRAPPER_1(get_attribute, "returns a (scalar) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_2(set_attribute, "sets a (scalar) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_1(get_vector_attribute, "returns a (vector) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_2(set_vector_attribute, "sets a (vector) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_0(list_attributes, "lists the keys of all attributes associated with this object")
            INSERT_PYTHON_WRAPPER_1(remove_attribute, "removes an attribute associated with this object")
            INSERT_PYTHON_WRAPPER_1(find_attributes, "find attributes matching the given pattern")
            INSERT_PYTHON_WRAPPER_1(attribute_exists, "tests if the given attribute exists for this instance")
            INSERT_PYTHON_WRAPPER_1(attribute_is_readonly, "tests if the given attribute is read only")
            INSERT_PYTHON_WRAPPER_1(attribute_is_writable, "tests if the given attribute is writable")
            INSERT_PYTHON_WRAPPER_1(attribute_is_vector, "tests if the given attribute is a vector attribute")
            INSERT_PYTHON_WRAPPER_1(attribute_is_removable, "tests if the given attribute is removable")

            // monitorable interface
            .def("list_metrics", &saga::task::list_metrics, 
                "returns the list of metrics associated with this task instance")
            .def("get_metric", &saga::task::get_metric, 
                "returns a specific metric associated with this task instance")
            .def("add_callback", &add_stream_stream_cb, 
                "add a new callback to this stream instance")
            .def("add_callback", &add_stream_stream_cb_obj, 
                "add a new callback to this stream instance")
            .def("remove_callback", &saga::task::remove_callback,
                "remove the given callback from this task instance")

            // saga::job_service interface
            INSERT_PYTHON_WRAPPER_0(get_url, 
                "get the URL to be used to connect to this server")
            INSERT_PYTHON_WRAPPER_0(get_context, 
                "return remote authorization info")
            INSERT_PYTHON_WRAPPER_1_OVERLOADS(connect,
                "establishes a connection to the target defined during the construction of the stream")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(wait, 
                "check if stream is ready for reading/writing, or if it has entered an error state")
            INSERT_PYTHON_WRAPPER_1_OVERLOADS(close, 
                "closes an active connection")
            INSERT_PYTHON_WRAPPER_1_OVERLOADS(read, 
                "read a raw buffer from stream")
            INSERT_PYTHON_WRAPPER_1(write, 
                "write a raw buffer to stream")
        ;
    }

    // saga::advert::flags
    enum_<saga::stream::state>("state")
        .value("Unknown", saga::stream::Unknown)
        .value("New", saga::stream::New)
        .value("Open", saga::stream::Open)
        .value("Closed", saga::stream::Closed)
        .value("Dropped", saga::stream::Dropped)
        .value("Error", saga::stream::Error)
        .export_values()
    ;
}

