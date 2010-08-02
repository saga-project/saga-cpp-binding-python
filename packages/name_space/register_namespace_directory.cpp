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
DEFINE_PYTHON_WRAPPERS_1(void, saga::name_space::directory, change_dir, std::string)
DEFINE_PYTHON_WRAPPERS_2_DEF_2(std::vector <saga::url>, saga::name_space::directory, 
    list, std::string, "*", int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(std::vector <saga::url>, saga::name_space::directory,  
    find, std::string, int, saga::name_space::Recursive)

DEFINE_PYTHON_WRAPPERS_1(saga::url, saga::name_space::directory, read_link, saga::url)
DEFINE_PYTHON_WRAPPERS_1_EX(saga::url, saga::name_space::directory, read_link_string, read_link, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::name_space::directory, exists, saga::url)
DEFINE_PYTHON_WRAPPERS_1_EX(bool, saga::name_space::directory, exists_string, exists, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::name_space::directory, is_dir, saga::url)
DEFINE_PYTHON_WRAPPERS_1_EX(bool, saga::name_space::directory, is_dir_string, is_dir, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::name_space::directory, is_entry, saga::url)
DEFINE_PYTHON_WRAPPERS_1_EX(bool, saga::name_space::directory, is_entry_string, is_entry, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::name_space::directory, is_link, saga::url)
DEFINE_PYTHON_WRAPPERS_1_EX(bool, saga::name_space::directory, is_link_string, is_link, std::string)

DEFINE_PYTHON_WRAPPERS_0(std::size_t, saga::name_space::directory, get_num_entries)
DEFINE_PYTHON_WRAPPERS_1(saga::url, saga::name_space::directory, get_entry, std::size_t)

DEFINE_PYTHON_WRAPPERS_3_DEF_1(void, saga::name_space::directory, 
    copy, saga::url, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    copy_string_1, copy, std::string, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    copy_string_2, copy, saga::url, std::string, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    copy_string_3, copy, std::string, std::string, int, saga::name_space::None)

DEFINE_PYTHON_WRAPPERS_3_DEF_1(void, saga::name_space::directory, 
    link, saga::url, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    link_string_1, link, std::string, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    link_string_2, link, saga::url, std::string, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    link_string_3, link, std::string, std::string, int, saga::name_space::None)

DEFINE_PYTHON_WRAPPERS_3_DEF_1(void, saga::name_space::directory, 
    move, saga::url, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    move_string_1, move, std::string, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    move_string_2, move, saga::url, std::string, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    move_string_3, move, std::string, std::string, int, saga::name_space::None)

DEFINE_PYTHON_WRAPPERS_2_DEF_1(void, saga::name_space::directory, 
    remove, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(void, saga::name_space::directory, 
    remove_string, remove, std::string, int, saga::name_space::None)

DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    copy_wildcard, copy, std::string, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    link_wildcard, link, std::string, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1_EX(void, saga::name_space::directory, 
    move_wildcard, move, std::string, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(void, saga::name_space::directory, 
    remove_wildcard, remove, std::string, int, saga::name_space::None)

DEFINE_PYTHON_WRAPPERS_2_DEF_1(void, saga::name_space::directory, 
    make_dir, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::name_space::entry, saga::name_space::directory, 
    open, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(saga::name_space::entry, saga::name_space::directory, 
    open_string, open, std::string, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(saga::name_space::directory, saga::name_space::directory, 
    open_dir, saga::url, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(saga::name_space::directory, saga::name_space::directory, 
    open_dir_string, open_dir, saga::url, int, saga::name_space::None)

// permissions, name_space::directory specific
DEFINE_PYTHON_WRAPPERS_4_DEF_1(void, saga::name_space::directory, 
    permissions_allow, saga::url, std::string, int, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_4_DEF_1(void, saga::name_space::directory, 
    permissions_deny, saga::url, std::string, int, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_4_DEF_1_EX(void, saga::name_space::directory, 
    permissions_allow_wildcard, permissions_allow, std::string, std::string, 
    int, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_4_DEF_1_EX(void, saga::name_space::directory, 
    permissions_deny_wildcard, permissions_deny, std::string, std::string, int, 
    int, saga::name_space::None)

///////////////////////////////////////////////////////////////////////////////
void register_namespace_directory()
{
    {
    scope namespace_directory = // saga::name_space::directory
        class_<saga::name_space::directory, bases<saga::name_space::entry> >(
                "directory", init<saga::url>())
            // additional constructors
            .def(init<std::string>())
            .def(init<saga::url, int>())
            .def(init<std::string, int>())
            .def(init<saga::session, saga::url>())
            .def(init<saga::session, std::string>())
            .def(init<saga::session, saga::url, int>())
            .def(init<saga::session, std::string, int>())

            // sage::name_space::directory
            INSERT_PYTHON_WRAPPER_1(change_dir, 
                "changes the current directory")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(list, 
                "lists entries matching a given pattern (contained in this directory)")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(find, 
                "finds entries matching a given pattern (contained in the tree below this directory)")

            INSERT_PYTHON_WRAPPER_0(get_num_entries, 
                "returns the number of entries in this directory")
            INSERT_PYTHON_WRAPPER_1(get_entry, 
                "returns a specific item based on the given index")

            INSERT_PYTHON_WRAPPER_1(read_link, 
                "returns the name of the items the given entry is linked to")
            INSERT_PYTHON_WRAPPER_1_EX(read_link_string, read_link, 
                "returns the name of the items the given entry is linked to")
            INSERT_PYTHON_WRAPPER_1(exists, 
                "returns, whether the given item exists")
            INSERT_PYTHON_WRAPPER_1_EX(exists_string, exists, 
                "returns, whether the given item exists")
            INSERT_PYTHON_WRAPPER_1(is_dir, 
                "returns, whether the given item refers to a directory")
            INSERT_PYTHON_WRAPPER_1_EX(is_dir_string, is_dir, 
                "returns, whether the given item refers to a directory")
            INSERT_PYTHON_WRAPPER_1(is_entry, 
                "returns, whether the given item refers to a simple entry")
            INSERT_PYTHON_WRAPPER_1_EX(is_entry_string, is_entry, 
                "returns, whether the given item refers to a simple entry")
            INSERT_PYTHON_WRAPPER_1(is_link, 
                "returns, whether the given item is a link to another entry")
            INSERT_PYTHON_WRAPPER_1_EX(is_link_string, is_link, 
                "returns, whether the given item is a link to another entry")

            INSERT_PYTHON_WRAPPER_3_OVERLOADS(copy, 
                "copies the the given item to the given name")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(copy_string_1, copy, 
                "copies the the given item to the given name")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(copy_string_2, copy, 
                "copies the the given item to the given name")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(copy_string_3, copy, 
                "copies the the given item to the given name")

            INSERT_PYTHON_WRAPPER_3_OVERLOADS(link, 
                "links the the given item to the given item")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(link_string_1, link, 
                "links the the given item to the given item")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(link_string_2, link, 
                "links the the given item to the given item")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(link_string_3, link, 
                "links the the given item to the given item")

            INSERT_PYTHON_WRAPPER_3_OVERLOADS(move, 
                "moves the the given item to the given name")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(move_string_1, move, 
                "moves the the given item to the given name")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(move_string_2, move, 
                "moves the the given item to the given name")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(move_string_3, move, 
                "moves the the given item to the given name")

            INSERT_PYTHON_WRAPPER_2_OVERLOADS(remove, 
                "removes the given item")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(remove_string, remove, 
                "removes the given item")

            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(copy_wildcard, copy, 
                "copies the the given items to the given name based on wildcard specification")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(link_wildcard, link, 
                "links the the given items to the given item based on wildcard specification")
            INSERT_PYTHON_WRAPPER_3_EX_OVERLOADS(move_wildcard, move, 
                "moves the the given items to the given name based on wildcard specification")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(remove_wildcard, remove, 
                "removes the given items based on wildcard specification")

            INSERT_PYTHON_WRAPPER_2_OVERLOADS(make_dir, "creates a new directory (without opening it)")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(open, "opens the given item")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(open_string, open, "opens the given item")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(open_dir, "opens the given directory")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(open_dir_string, open_dir, "opens the given directory")

            // saga::permissions
            INSERT_PYTHON_WRAPPER_4_EX_OVERLOADS(permissions_allow_wildcard, permissions_allow, 
                "enable a permission based on wildcard specification")
            INSERT_PYTHON_WRAPPER_4_EX_OVERLOADS(permissions_deny_wildcard, permissions_deny,
                "disable a permission based on wildcard specification")
            INSERT_PYTHON_WRAPPER_4_OVERLOADS(permissions_allow, 
                "enable a permission on a given namespace entry")
            INSERT_PYTHON_WRAPPER_4_OVERLOADS(permissions_deny, 
                "disable a permissionon a given this namespace entry")
        ;
    }
}
