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
DEFINE_PYTHON_WRAPPERS_0(saga::url, saga::name_space::entry, get_url)
DEFINE_PYTHON_WRAPPERS_0(saga::url, saga::name_space::entry, get_cwd)
DEFINE_PYTHON_WRAPPERS_0(saga::url, saga::name_space::entry, get_name)

DEFINE_PYTHON_WRAPPERS_0(saga::url, saga::name_space::entry, read_link)
DEFINE_PYTHON_WRAPPERS_0(bool, saga::name_space::entry, is_dir)
DEFINE_PYTHON_WRAPPERS_0(bool, saga::name_space::entry, is_entry)
DEFINE_PYTHON_WRAPPERS_0(bool, saga::name_space::entry, is_link)

DEFINE_PYTHON_WRAPPERS_2_DEF_1(void, saga::name_space::entry, copy, saga::url, 
    int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(void, saga::name_space::entry, 
    copy_string, copy, std::string, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(void, saga::name_space::entry, link, saga::url, 
    int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(void, saga::name_space::entry, 
    link_string, link, std::string, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1(void, saga::name_space::entry, move, saga::url, 
    int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_2_DEF_1_EX(void, saga::name_space::entry, 
    move_string, move, std::string, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_0(void, saga::name_space::entry, remove)
DEFINE_PYTHON_WRAPPERS_0(void, saga::name_space::entry, close)

// permissions, generic
DEFINE_PYTHON_WRAPPERS_2(void, saga::name_space::entry, permissions_allow, std::string, int)
DEFINE_PYTHON_WRAPPERS_2(void, saga::name_space::entry, permissions_deny, std::string, int)
DEFINE_PYTHON_WRAPPERS_2(bool, saga::name_space::entry, permissions_check, std::string, int)
DEFINE_PYTHON_WRAPPERS_0(std::string, saga::name_space::entry, get_owner)
DEFINE_PYTHON_WRAPPERS_0(std::string, saga::name_space::entry, get_group)

// permissions, name_space::entry specific
DEFINE_PYTHON_WRAPPERS_3_DEF_1(void, saga::name_space::entry, permissions_allow, std::string, int, int, saga::name_space::None)
DEFINE_PYTHON_WRAPPERS_3_DEF_1(void, saga::name_space::entry, permissions_deny, std::string, int, int, saga::name_space::None)

///////////////////////////////////////////////////////////////////////////////
void register_namespace_entry()
{
    {
    scope namespace_entry = // saga::name_space::entry
        class_<saga::name_space::entry, bases<saga::object> >(
                "entry", init<saga::url>())
            // additional constructors
            .def(init<std::string>())
            .def(init<saga::url, int>())
            .def(init<std::string, int>())
            .def(init<saga::session, saga::url>())
            .def(init<saga::session, std::string>())
            .def(init<saga::session, saga::url, int>())
            .def(init<saga::session, std::string, int>())

            // sage::name_space::entry
            INSERT_PYTHON_WRAPPER_0(get_url, "returns the full url of this item")
            INSERT_PYTHON_WRAPPER_0(get_cwd, "returns the directory name this item is located in")
            INSERT_PYTHON_WRAPPER_0(get_name, "returns the name of this item")

            INSERT_PYTHON_WRAPPER_0(read_link, "returns the name of the item this entry is linked to")
            INSERT_PYTHON_WRAPPER_0(is_dir, "returns, whether this item refers to a directory")
            INSERT_PYTHON_WRAPPER_0(is_entry, "returns, whether this item refers to a simple entry")
            INSERT_PYTHON_WRAPPER_0(is_link, "returns, whether this item is a link to another entry")

            INSERT_PYTHON_WRAPPER_2_OVERLOADS(copy, 
                "copies this item to the given name (url)")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(copy_string, copy, 
                "copies this item to the given name (string)")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(link, 
                "links this item to the given item (url)")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(link_string, link, 
                "links this item to the given item (string)")
            INSERT_PYTHON_WRAPPER_2_OVERLOADS(move, 
                "moves this item to the given name (url)")
            INSERT_PYTHON_WRAPPER_2_EX_OVERLOADS(move_string, move, 
                "moves this item to the given name (string)")
            INSERT_PYTHON_WRAPPER_0(remove, "removes this entry")
            INSERT_PYTHON_WRAPPER_0(close, "closes this entry")

            // saga::permissions
            INSERT_PYTHON_WRAPPER_2(permissions_allow, 
                "generically enable a permission on this namespace entry")
            INSERT_PYTHON_WRAPPER_2(permissions_deny, 
                "generically disable a permission on this namespace entry")
            INSERT_PYTHON_WRAPPER_2(permissions_check, 
                "generically check a permission on this namespace entry")
            INSERT_PYTHON_WRAPPER_0(get_owner, 
                "returns the owner of this object instance")
            INSERT_PYTHON_WRAPPER_0(get_group, 
                "returns the group of the owner of this object instance")
                
            INSERT_PYTHON_WRAPPER_3_OVERLOADS(permissions_allow, 
                "enable a permission on this namespace entry")
            INSERT_PYTHON_WRAPPER_3_OVERLOADS(permissions_deny, 
                "disable a permission on this namespace entry")
        ;
    }

    // saga::name_space::flags
    enum_<saga::name_space::flags>("flags")
        .value("Unknown", saga::name_space::Unknown)
        .value("None", saga::name_space::None)
        .value("Overwrite", saga::name_space::Overwrite)
        .value("Recursive", saga::name_space::Recursive)
        .value("Dereference", saga::name_space::Dereference)
        .value("Create", saga::name_space::Create)
        .value("Exclusive", saga::name_space::Exclusive)
        .value("Lock", saga::name_space::Lock)
        .value("CreateParents", saga::name_space::CreateParents)
        .export_values()
    ;
}
