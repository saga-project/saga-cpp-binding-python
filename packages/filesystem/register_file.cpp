//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>

#include <saga/saga.hpp>
#include <boost/function.hpp>
#include <boost/python.hpp>

#include <saga/impl/exception.hpp>

#include "register_types.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_0(saga::off_t, saga::filesystem::file, get_size)
DEFINE_PYTHON_WRAPPERS_2(saga::off_t, saga::filesystem::file, seek, saga::off_t, 
    saga::filesystem::seek_mode)

///////////////////////////////////////////////////////////////////////////////
static saga::ssize_t py_native_write_1(saga::filesystem::file f, 
    boost::python::object o)
{
    if (!PyString_Check(o.ptr())) {
        // parameter is not a string object, throw an error
        SAGA_THROW_VERBATIM(f, "First parameter needs to be a string object",
            saga::BadParameter);
        return 0;
    }
    saga::ssize_t len = PyString_Size(o.ptr());
    return f.write(saga::const_buffer(PyString_AsString(o.ptr()), len));
}
BOOST_PYTHON_FUNCTION_OVERLOADS(py_native_write_1_ov, py_native_write_1, 2, 2)

template <typename Type>
saga::task py_native_write_1_type(saga::filesystem::file f, boost::python::object o)
{
    if (!PyString_Check(o.ptr())) {
        // parameter is not a string object, throw an error
        SAGA_THROW_VERBATIM(f, "First parameter needs to be a string object",
            saga::BadParameter);
        return saga::task();
    }
    saga::ssize_t len = PyString_Size(o.ptr());
    return f.write<Type>(saga::const_buffer(PyString_AsString(o.ptr()), len));
}

static saga::task py_native_write_task_1(saga::filesystem::file f, 
    saga::python::routine_type type, boost::python::object o)
{
    switch(type) {
    case saga::python::Sync: 
        return py_native_write_1_type<saga::task_base::Sync>(f, o);
    case saga::python::Async: 
        return py_native_write_1_type<saga::task_base::Async>(f, o);
    case saga::python::Task: 
        return py_native_write_1_type<saga::task_base::Task>(f, o);
    default:
        SAGA_THROW_VERBATIM(f,
            "Invalid routine type (not Sync, Async or Task)",
            saga::BadParameter);
        break;
    }
    return saga::task();
}
BOOST_PYTHON_FUNCTION_OVERLOADS(py_native_write_task_1_ov, py_native_write_task_1, 3, 3)

DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::ssize_t, saga::filesystem::file, write, 
    saga::mutable_buffer, saga::size_t, 0)

///////////////////////////////////////////////////////////////////////////////
static boost::python::handle<> py_native_read_1(saga::filesystem::file f, 
    std::size_t size = -1)
{
    saga::mutable_buffer buf(size);
    saga::ssize_t r = f.read(buf, (size == std::size_t(-1)) ? 0 : size);
    return boost::python::handle<>(PyString_FromStringAndSize(
        (char const*)buf.get_data(), r));
}
BOOST_PYTHON_FUNCTION_OVERLOADS(py_native_read_1_ov, py_native_read_1, 1, 2)

template <typename Type>
saga::task py_native_read_1_type(saga::filesystem::file f, std::size_t size)
{
    saga::mutable_buffer buf(size);
    return f.read<Type>(buf, (size == std::size_t(-1)) ? 0 : size);
}

static saga::task py_native_read_task_1(saga::filesystem::file f, 
    saga::python::routine_type type, std::size_t size = -1)
{
    switch(type) {
    case saga::python::Sync: 
        return py_native_read_1_type<saga::task_base::Sync>(f, size);
    case saga::python::Async: 
        return py_native_read_1_type<saga::task_base::Async>(f, size);
    case saga::python::Task: 
        return py_native_read_1_type<saga::task_base::Task>(f, size);
    default:
        SAGA_THROW_VERBATIM(f,
            "Invalid routine type (not Sync, Async or Task)",
            saga::BadParameter);
        break;
    }
    return saga::task();
}
BOOST_PYTHON_FUNCTION_OVERLOADS(py_native_read_task_1_ov, py_native_read_task_1, 2, 3)

DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::ssize_t, saga::filesystem::file, read, 
    saga::mutable_buffer, saga::size_t, 0)

///////////////////////////////////////////////////////////////////////////////
void register_file()
{
    {
    scope file = // saga::file
        class_<saga::filesystem::file, bases<saga::name_space::entry> >(
                "file", init<saga::url>())
            // additional constructors
            .def(init<std::string>())
            .def(init<saga::url, int>())
            .def(init<std::string, int>())
            .def(init<saga::session, saga::url>())
            .def(init<saga::session, std::string>())
            .def(init<saga::session, saga::url, int>())
            .def(init<saga::session, std::string, int>())

            // saga::filesystem::file interface
            INSERT_PYTHON_WRAPPER_0(get_size, "returns the size of this file")

            INSERT_PYTHON_WRAPPER_2_OVERLOADS(read, "reads the given number of bytes from the file")
            INSERT_PYTHON_WRAPPER_1_EX_OVERLOADS(native_read, read, "reads the given number of bytes from the file")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(write, "writes the given bytes to the file")
            INSERT_PYTHON_WRAPPER_1_EX_OVERLOADS(native_write, write, "writes the given bytes to the file")
            INSERT_PYTHON_WRAPPER_2(seek, "repositions the current file pointer")
        ;

        // saga::filesystem::seek_mode
        enum_<saga::filesystem::seek_mode>("seek_mode")
            .value("Start", saga::filesystem::Start)
            .value("End", saga::filesystem::End)
            .value("Current", saga::filesystem::Current)
            .export_values()
        ;
    }

    // saga::filesystem::flags
    enum_<saga::filesystem::flags>("flags")
        .value("Unknown", saga::filesystem::Unknown)
        .value("None", saga::filesystem::None)
        .value("Overwrite", saga::filesystem::Overwrite)
        .value("Recursive", saga::filesystem::Recursive)
        .value("Dereference", saga::filesystem::Dereference)
        .value("Create", saga::filesystem::Create)
        .value("Exclusive", saga::filesystem::Exclusive)
        .value("Lock", saga::filesystem::Lock)
        .value("CreateParents", saga::filesystem::CreateParents)
        .value("Truncate", saga::filesystem::Truncate)
        .value("Append", saga::filesystem::Append)
        .value("Read", saga::filesystem::Read)
        .value("Write", saga::filesystem::Write)
        .value("ReadWrite", saga::filesystem::ReadWrite)
        .value("Binary", saga::filesystem::Binary)
        .export_values()
    ;
}
