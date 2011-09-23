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
DEFINE_PYTHON_WRAPPERS_1(std::string, saga::cpr::directory, get_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::cpr::directory, set_attribute, std::string, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::cpr::directory, get_vector_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::cpr::directory, set_vector_attribute, std::string, std::vector<std::string>)
DEFINE_PYTHON_WRAPPERS_0(std::vector<std::string>, saga::cpr::directory, list_attributes)
DEFINE_PYTHON_WRAPPERS_1(void, saga::cpr::directory, remove_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::cpr::directory, find_attributes, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::cpr::directory, attribute_exists, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::cpr::directory, attribute_is_readonly, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::cpr::directory, attribute_is_writable, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::cpr::directory, attribute_is_vector, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::cpr::directory, attribute_is_removable, std::string)

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_1(bool, saga::cpr::directory, is_checkpoint, saga::url);
DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::cpr::checkpoint, saga::cpr::directory, 
    open, saga::url, int, saga::cpr::Read)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(saga::cpr::checkpoint, saga::cpr::directory, 
    open_string, open, std::string, int, saga::cpr::Read)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::cpr::directory, saga::cpr::directory, 
    open_dir, saga::url, int, saga::cpr::Read)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(saga::cpr::directory, saga::cpr::directory, 
    open_dir_string, open_dir, std::string, int, saga::cpr::Read)
DEFINE_PYTHON_WRAPPERS_4_DEF_2(std::vector<saga::url>, saga::cpr::directory, find, 
    std::string,  std::vector<std::string>, int, saga::cpr::Recursive, std::string, "")

DEFINE_PYTHON_WRAPPERS_3_DEF_1(void, saga::cpr::directory, 
    set_parent, saga::url, saga::url, int, 1)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::cpr::directory, 
    set_parent_string_1, set_parent, std::string, saga::url, int, 1)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::cpr::directory, 
    set_parent_string_2, set_parent, saga::url, std::string, int, 1)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::cpr::directory, 
    set_parent_string_3, set_parent, std::string, std::string, int, 1)

DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::url, saga::cpr::directory, 
    get_parent, saga::url, int, 1)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(saga::url, saga::cpr::directory, 
    get_parent_string, get_parent, std::string, int, 1)
DEFINE_PYTHON_WRAPPERS_1(int, saga::cpr::directory, 
    get_file_num, saga::url)
DEFINE_PYTHON_WRAPPERS_1_EX(int, saga::cpr::directory, 
    get_file_num_string, get_file_num, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<saga::url>, saga::cpr::directory, 
    list_files, saga::url)
DEFINE_PYTHON_WRAPPERS_1_EX(std::vector<saga::url>, saga::cpr::directory, 
    list_files_string, list_files, std::string)
DEFINE_PYTHON_WRAPPERS_2(int, saga::cpr::directory, 
    add_file, saga::url, saga::url)
DEFINE_PYTHON_WRAPPERS_2_EX(int, saga::cpr::directory, 
    add_file_string, add_file, saga::url, std::string)
DEFINE_PYTHON_WRAPPERS_2(saga::url, saga::cpr::directory, 
    get_file, saga::url, int)
DEFINE_PYTHON_WRAPPERS_2_EX(saga::url, saga::cpr::directory, 
    get_file_string, get_file, std::string, int)

DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(saga::filesystem::file, saga::cpr::directory, 
    open_file_idx, open_file, saga::url, int, int, saga::cpr::CreateParents|saga::cpr::Lock|saga::cpr::ReadWrite)
DEFINE_PYTHON_WRAPPERS_3_DEF_1(saga::filesystem::file, saga::cpr::directory, 
    open_file, saga::url, saga::url, int, saga::cpr::CreateParents|saga::cpr::Lock|saga::cpr::ReadWrite)
DEFINE_PYTHON_WRAPPERS_2_EX(void, saga::cpr::directory, remove_file_idx, remove_file, saga::url, int)
DEFINE_PYTHON_WRAPPERS_2(void, saga::cpr::directory, remove_file, saga::url, saga::url)
DEFINE_PYTHON_WRAPPERS_3_EX(void, saga::cpr::directory, update_file_idx, update_file, saga::url, int, saga::url)
DEFINE_PYTHON_WRAPPERS_3(void, saga::cpr::directory, update_file, saga::url, saga::url, saga::url)
DEFINE_PYTHON_WRAPPERS_3_EX(void, saga::cpr::directory, stage_file_idx, stage_file, saga::url, int, saga::url)
DEFINE_PYTHON_WRAPPERS_3(void, saga::cpr::directory, stage_file, saga::url, saga::url, saga::url)

///////////////////////////////////////////////////////////////////////////////
void register_cpr_directory()
{
    {
    scope directory = // saga::cpr::directory
        class_<saga::cpr::directory, bases<saga::name_space::directory> >(
                "directory", init<saga::url>())
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

            // saga::cpr::directory interface
            INSERT_PYTHON_WRAPPER_1(is_checkpoint, "returns, whether the given item refers to a checkpoint entry")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(open, 
                "opens the given file in this directory")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(open_string, open, 
                "opens the given file in this directory")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(open_dir, 
                "opens the given directory in this directory")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(open_dir_string, open_dir, 
                "opens the given directory in this directory")
            INSERT_PYTHON_WRAPPER_4_OVERLOADS(find, 
                "finds cpr entries matching the given patterns")
            INSERT_PYTHON_WRAPPER_3_OVERLOADS(set_parent, 
                "set the previous checkpoint url")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(set_parent_string_1, set_parent, 
                "set the previous checkpoint url")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(set_parent_string_2, set_parent, 
                "set the previous checkpoint url")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(set_parent_string_3, set_parent, 
                "set the previous checkpoint url")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(get_parent, 
                "get the previous checkpoint url")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(get_parent_string, get_parent, 
                "get the previous checkpoint url")
            INSERT_PYTHON_WRAPPER_1(get_file_num, 
                "get number of checkpoint files")
            INSERT_PYTHON_WRAPPER_1_EX(get_file_num_string, get_file_num, 
                "get number of checkpoint files")
            INSERT_PYTHON_WRAPPER_1(list_files, 
                "list the available checkpoint files")
            INSERT_PYTHON_WRAPPER_1_EX(list_files_string, list_files, 
                "list the available checkpoint files")
            INSERT_PYTHON_WRAPPER_2(add_file, 
                "register a checkpoint file")
            INSERT_PYTHON_WRAPPER_2_EX(add_file_string, add_file, 
                "register a checkpoint file")
            INSERT_PYTHON_WRAPPER_2(get_file, 
                "get the checkpoint file number n")
            INSERT_PYTHON_WRAPPER_2_EX(get_file_string, get_file, 
                "get the checkpoint file number n")

            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(open_file_idx, open_file, "open checkpoint file number n")
            INSERT_PYTHON_WRAPPER_3_OVERLOADS(open_file, "open checkpoint file by name")
            INSERT_PYTHON_WRAPPER_2_EX(remove_file_idx, remove_file, "remove a specific checkpoint file by id")
            INSERT_PYTHON_WRAPPER_2(remove_file, "remove a specific checkpoint file by name")
            INSERT_PYTHON_WRAPPER_3_EX(update_file_idx, update_file, "update a specific checkpoint file by id")
            INSERT_PYTHON_WRAPPER_3(update_file, "update a specific checkpoint file by name")
            INSERT_PYTHON_WRAPPER_3_EX(stage_file_idx, stage_file, "stage a specific checkpoint file by id")
            INSERT_PYTHON_WRAPPER_3(stage_file, "stage a specific checkpoint file by name")
        ;
    }
}

