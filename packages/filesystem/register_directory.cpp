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
DEFINE_PYTHON_WRAPPERS_1(saga::off_t, saga::filesystem::directory, get_size, saga::url)
DEFINE_PYTHON_WRAPPERS_1_EX(saga::off_t, saga::filesystem::directory, get_size_string, get_size, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::filesystem::directory, is_file,  saga::url)
DEFINE_PYTHON_WRAPPERS_1_EX(bool, saga::filesystem::directory, is_file_string, is_file, std::string)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::filesystem::file, saga::filesystem::directory, 
    open, saga::url, int, saga::filesystem::Read)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(saga::filesystem::file, saga::filesystem::directory, 
    open_string, open, std::string, int, saga::filesystem::Read)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::filesystem::directory, saga::filesystem::directory, 
    open_dir, saga::url, int, saga::filesystem::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(saga::filesystem::directory, saga::filesystem::directory, 
    open_dir_string, open_dir, std::string, int, saga::filesystem::None)

///////////////////////////////////////////////////////////////////////////////
void register_directory()
{
    {
    scope directory = // saga::directory
        class_<saga::filesystem::directory, bases<saga::name_space::directory> >(
                "directory", init<saga::url>())
            // additional constructors
            .def(init<std::string>())
            .def(init<saga::url, int>())
            .def(init<std::string, int>())
            .def(init<saga::session, saga::url>())
            .def(init<saga::session, std::string>())
            .def(init<saga::session, saga::url, int>())
            .def(init<saga::session, std::string, int>())

            INSERT_PYTHON_WRAPPER_1(get_size, 
                "returns the size of this directory")
            INSERT_PYTHON_WRAPPER_1_EX(get_size_string, get_size, 
                "returns the size of this directory")

            INSERT_PYTHON_WRAPPER_1(is_file, 
                "returns, whether the given item refers to a file")
            INSERT_PYTHON_WRAPPER_1_EX(is_file_string, is_file, 
                "returns, whether the given item refers to a file")

            INSERT_PYTHON_WRAPPER_2_OVERLOADS(open, 
                "opens the given file in this directory")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(open_string, open, 
                "opens the given file in this directory")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(open_dir, 
                "opens the given directory in this directory")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(open_dir_string, open_dir, 
                "opens the given directory in this directory")
        ;
    }
}

