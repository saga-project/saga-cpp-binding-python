//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>

#include <saga/saga.hpp>
#include <boost/function.hpp>
#include <boost/python.hpp>


#include "register_types.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_1(std::string, saga::replica::logical_file, get_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::replica::logical_file, set_attribute, std::string, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::replica::logical_file, get_vector_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::replica::logical_file, set_vector_attribute, std::string, std::vector<std::string>)
DEFINE_PYTHON_WRAPPERS_0(std::vector<std::string>, saga::replica::logical_file, list_attributes)
DEFINE_PYTHON_WRAPPERS_1(void, saga::replica::logical_file, remove_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::replica::logical_file, find_attributes, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::replica::logical_file, attribute_exists, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::replica::logical_file, attribute_is_readonly, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::replica::logical_file, attribute_is_writable, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::replica::logical_file, attribute_is_vector, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::replica::logical_file, attribute_is_removable, std::string)

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_1(void, saga::replica::logical_file, add_location, saga::url)
DEFINE_PYTHON_WRAPPERS_1(void, saga::replica::logical_file, remove_location, saga::url)
DEFINE_PYTHON_WRAPPERS_2(void, saga::replica::logical_file, update_location, saga::url, saga::url)
DEFINE_PYTHON_WRAPPERS_0(std::vector<saga::url>, saga::replica::logical_file, list_locations)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(void, saga::replica::logical_file, 
    replicate, saga::url, int, saga::replica::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(void, saga::replica::logical_file, 
    replicate_string, replicate, std::string, int, saga::replica::None)

///////////////////////////////////////////////////////////////////////////////
void register_logical_file()
{
    {
    scope logicalfile = // saga::logicalfile
        class_<saga::replica::logical_file, bases<saga::name_space::entry> >(
                "entry", init<saga::url>())
            // additional constructor
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

            // saga::replica interface
            INSERT_PYTHON_WRAPPER_1(add_location, "adds a new file location to this replca entry")
            INSERT_PYTHON_WRAPPER_1(remove_location, "removes an existing file location from this replica entry")
            INSERT_PYTHON_WRAPPER_2(update_location, "replaces an existing file location in this replica entry with a new file location")
            INSERT_PYTHON_WRAPPER_0(list_locations, "list all existing file locations from this replica entry")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(replicate, 
                "create a new copy of one of the file locations of this replica entry at the given file location")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(replicate_string, replicate, 
                "create a new copy of one of the file locations of this replica entry at the given file location")
        ;
    }

    // saga::replica::flags
    enum_<saga::replica::flags>("flags")
        .value("Unknown", saga::replica::Unknown)
        .value("None", saga::replica::None)
        .value("Overwrite", saga::replica::Overwrite)
        .value("Recursive", saga::replica::Recursive)
        .value("DeReference", saga::replica::Dereference)
        .value("Create", saga::replica::Create)
        .value("Excl", saga::replica::Exclusive)
        .value("Lock", saga::replica::Lock)
        .value("CreateParents", saga::replica::CreateParents)
        .value("Read", saga::replica::Read)
        .value("Write", saga::replica::Write)
        .value("ReadWrite", saga::replica::ReadWrite)
        .export_values()
    ;
}
