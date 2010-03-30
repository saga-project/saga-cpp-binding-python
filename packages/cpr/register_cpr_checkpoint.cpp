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
DEFINE_PYTHON_WRAPPERS_1(std::string, saga::cpr::checkpoint, get_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::cpr::checkpoint, set_attribute, std::string, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::cpr::checkpoint, get_vector_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::cpr::checkpoint, set_vector_attribute, std::string, std::vector<std::string>)
DEFINE_PYTHON_WRAPPERS_0(std::vector<std::string>, saga::cpr::checkpoint, list_attributes)
DEFINE_PYTHON_WRAPPERS_1(void, saga::cpr::checkpoint, remove_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::cpr::checkpoint, find_attributes, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::cpr::checkpoint, attribute_exists, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::cpr::checkpoint, attribute_is_readonly, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::cpr::checkpoint, attribute_is_writable, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::cpr::checkpoint, attribute_is_vector, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::cpr::checkpoint, attribute_is_removable, std::string)

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_2_DEF_1(void, saga::cpr::checkpoint, 
    set_parent, saga::url, int, 1)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(void, saga::cpr::checkpoint, 
    set_parent_string, set_parent, std::string, int, 1)
DEFINE_PYTHON_WRAPPERS_1_DEF_1(saga::url, saga::cpr::checkpoint, 
    get_parent, int, 1)
DEFINE_PYTHON_WRAPPERS_0(std::vector<saga::url>, saga::cpr::checkpoint, 
    list_files)
DEFINE_PYTHON_WRAPPERS_1(int, saga::cpr::checkpoint, 
    add_file, saga::url)
DEFINE_PYTHON_WRAPPERS_1_EX(int, saga::cpr::checkpoint, 
    add_file_string, add_file, std::string)
DEFINE_PYTHON_WRAPPERS_1(saga::url, saga::cpr::checkpoint, get_file, int)

DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(saga::filesystem::file, saga::cpr::checkpoint, 
    open_file_idx, open_file, int, int, saga::cpr::CreateParents|saga::cpr::Lock|saga::cpr::ReadWrite)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::filesystem::file, saga::cpr::checkpoint, 
    open_file, saga::url, int, saga::cpr::CreateParents|saga::cpr::Lock|saga::cpr::ReadWrite)
DEFINE_PYTHON_WRAPPERS_1_EX(void, saga::cpr::checkpoint, remove_file_idx, remove_file, int)
DEFINE_PYTHON_WRAPPERS_1(void, saga::cpr::checkpoint, remove_file, saga::url)
DEFINE_PYTHON_WRAPPERS_2_EX(void, saga::cpr::checkpoint, update_file_idx, update_file, int, saga::url)
DEFINE_PYTHON_WRAPPERS_2(void, saga::cpr::checkpoint, update_file, saga::url, saga::url)
DEFINE_PYTHON_WRAPPERS_2_EX(void, saga::cpr::checkpoint, stage_file_idx, stage_file, int, saga::url)
DEFINE_PYTHON_WRAPPERS_2(void, saga::cpr::checkpoint, stage_file, saga::url, saga::url)

///////////////////////////////////////////////////////////////////////////////
void register_cpr_checkpoint()
{
    {
    scope checkpoint = // saga::cpr::checkpoint
        class_<saga::cpr::checkpoint, bases<saga::name_space::entry> >(
                "checkpoint", init<saga::url>())
            // additional constructors
            .def(init<saga::url, int>())
            .def(init<saga::session, saga::url>())
            .def(init<saga::session, saga::url, int>())
            
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

            // saga::cpr::checkpoint interface
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(set_parent, "set the previous checkpoint url")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(set_parent_string, set_parent, 
                "set the previous checkpoint url")
            INSERT_PYTHON_WRAPPER_1_OVERLOADS(get_parent, "get the previous checkpoint url")
            INSERT_PYTHON_WRAPPER_0(list_files, "list the available checkpoint files")
            INSERT_PYTHON_WRAPPER_1(add_file, "register a checkpoint file")
            INSERT_PYTHON_WRAPPER_1_EX(add_file_string, add_file, 
                "register a checkpoint file")

            INSERT_PYTHON_WRAPPER_1(get_file, "get the checkpoint file number n")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(open_file_idx, open_file, "open checkpoint file number n")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(open_file, "open checkpoint file by name")
            INSERT_PYTHON_WRAPPER_1_EX(remove_file_idx, remove_file, "remove a specific checkpoint file by id")
            INSERT_PYTHON_WRAPPER_1(remove_file, "remove a specific checkpoint file by name")
            INSERT_PYTHON_WRAPPER_2_EX(update_file_idx, update_file, "update a specific checkpoint file by id")
            INSERT_PYTHON_WRAPPER_2(update_file, "update a specific checkpoint file by name")
            INSERT_PYTHON_WRAPPER_2_EX(stage_file_idx, stage_file, "stage a specific checkpoint file by id")
            INSERT_PYTHON_WRAPPER_2(stage_file, "stage a specific checkpoint file by name")
        ;
    }

    // saga::cpr::flags
    enum_<saga::cpr::flags>("flags")
        .value("None", saga::cpr::None)
        .value("Overwrite", saga::cpr::Overwrite)
        .value("Recursive", saga::cpr::Recursive)
        .value("DeReference", saga::cpr::Dereference)
        .value("Create", saga::cpr::Create)
        .value("Excl", saga::cpr::Exclusive)
        .value("Lock", saga::cpr::Lock)
        .value("CreateParents", saga::cpr::CreateParents)
        .value("Truncate", saga::cpr::Truncate)
        .value("Append", saga::cpr::Append)
        .value("Read", saga::cpr::Read)
        .value("Write", saga::cpr::Write)
        .value("ReadWrite", saga::cpr::ReadWrite)
        .export_values()
    ;
}
