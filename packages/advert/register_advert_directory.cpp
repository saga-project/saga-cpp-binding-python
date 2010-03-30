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
DEFINE_PYTHON_WRAPPERS_1(std::string, saga::advert::directory, get_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::advert::directory, set_attribute, std::string, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::advert::directory, get_vector_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::advert::directory, set_vector_attribute, std::string, std::vector<std::string>)
DEFINE_PYTHON_WRAPPERS_0(std::vector<std::string>, saga::advert::directory, list_attributes)
DEFINE_PYTHON_WRAPPERS_1(void, saga::advert::directory, remove_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::advert::directory, find_attributes, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::advert::directory, attribute_exists, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::advert::directory, attribute_is_readonly, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::advert::directory, attribute_is_writable, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::advert::directory, attribute_is_vector, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::advert::directory, attribute_is_removable, std::string)

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::advert::entry, saga::advert::directory, 
    open, saga::url, int, saga::advert::Read)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(saga::advert::entry, saga::advert::directory, 
    open_string, open, std::string, int, saga::advert::Read)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::advert::directory, saga::advert::directory, 
    open_dir, saga::url, int, saga::advert::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(saga::advert::directory, saga::advert::directory, 
    open_dir_string, open_dir, std::string, int, saga::advert::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1(std::vector<saga::url>, saga::advert::directory, 
    find, std::string, std::vector<std::string>, int, saga::advert::Recursive)

///////////////////////////////////////////////////////////////////////////////
void register_advert_directory()
{
    {
    scope advert_directory = // saga::advert::directory
        class_<saga::advert::directory, bases<saga::name_space::directory> >(
                "directory", init<saga::url>())
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

            // saga::advert::directory interface
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(open, 
                "opens the given file in this advert directory")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(open_string, open, 
                "opens the given file in this advert directory")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(open_dir, 
                "opens the given directory in this advert directory")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(open_dir_string, open_dir, 
                "opens the given directory in this advert directory")
            INSERT_PYTHON_WRAPPER_3_OVERLOADS(find, 
                "finds replica entries matching the given patterns")
        ;
    }
}

