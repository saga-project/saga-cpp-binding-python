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
DEFINE_PYTHON_WRAPPERS_1(void, saga::advert::entry, store_object, saga::object)

static std::string py_retrieve_string_object_0(saga::advert::entry o)
{
    return o.retrieve_object<std::string>();
}
static saga::task py_retrieve_string_object_task_0(saga::advert::entry o,
    saga::python::routine_type type)
{
    switch(type) {
    case saga::python::Sync: return o.retrieve_object<std::string, saga::task_base::Sync>();
    case saga::python::Async: return o.retrieve_object<std::string, saga::task_base::Async>();
    case saga::python::Task: return o.retrieve_object<std::string, saga::task_base::Task>();
    default:
        SAGA_THROW_VERBATIM(o,
            "Invalid routine type (not Sync, Async or Task)",
            saga::BadParameter);
        break;
    }
    return saga::task();
}

DEFINE_PYTHON_WRAPPERS_0(saga::object, saga::advert::entry, retrieve_object)
DEFINE_PYTHON_WRAPPERS_1(saga::object, saga::advert::entry, retrieve_object, saga::session)

#if !defined(SAGA_ADVERT_NO_DEPRECATED)
///////////////////////////////////////////////////////////////////////////////
// obsolete
DEFINE_PYTHON_WRAPPERS_1(void, saga::advert::entry, store_string, std::string)
DEFINE_PYTHON_WRAPPERS_0(std::string, saga::advert::entry, retrieve_string)
#endif

///////////////////////////////////////////////////////////////////////////////
void register_advert()
{
    {
    scope advert = // saga::advert
        class_<saga::advert::entry, bases<saga::name_space::entry> >(
                "entry", init<saga::url>())
            // additional constructors
            .def(init<std::string>())
            .def(init<saga::url, int>())
            .def(init<std::string, int>())
            .def(init<saga::session, saga::url>())
            .def(init<saga::session, std::string>())
            .def(init<saga::session, saga::url, int>())
            .def(init<saga::session, std::string, int>())

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

            // saga::advert::entry interface
            INSERT_PYTHON_WRAPPER_1(store_object, "store the given saga object as the data of this advert entry")
            INSERT_PYTHON_WRAPPER_0(retrieve_object, "retrieve the the data of this advert entry as a saga object")
            INSERT_PYTHON_WRAPPER_0_EX(retrieve_string_object, retrieve_object, 
                "retrieve the the data of this advert entry as a string")
            INSERT_PYTHON_WRAPPER_1(retrieve_object, "retrieve the the data of this advert entry as a saga object, use given session to instantiate the saga object")
#if !defined(SAGA_ADVERT_NO_DEPRECATED)
            INSERT_PYTHON_WRAPPER_1(store_string, "store the given string as the data of this advert entry")
            INSERT_PYTHON_WRAPPER_0(retrieve_string, "retrieve the the data of this advert entry as a string")
#endif
        ;
    }

    // saga::advert::flags
    enum_<saga::advert::flags>("flags")
        .value("Unknown", saga::advert::Unknown)
        .value("None", saga::advert::None)
        .value("Overwrite", saga::advert::Overwrite)
        .value("Recursive", saga::advert::Recursive)
        .value("DeReference", saga::advert::Dereference)
        .value("Create", saga::advert::Create)
        .value("Excl", saga::advert::Exclusive)
        .value("Lock", saga::advert::Lock)
        .value("CreateParents", saga::advert::CreateParents)
        .value("Read", saga::advert::Read)
        .value("Write", saga::advert::Write)
        .value("ReadWrite", saga::advert::ReadWrite)
        .export_values()
    ;
}
