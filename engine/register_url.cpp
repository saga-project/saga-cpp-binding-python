//  Copyright (c) 2005-2009 Hartmut Kaiser
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/python.hpp>
#include <saga/saga.hpp>

#include "register_types.hpp"
#include "routine_types.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_1_EX(saga::url, saga::url, translate_without_session, 
    translate, std::string)
DEFINE_PYTHON_WRAPPERS_2_EX(saga::url, saga::url, translate_with_session, 
    translate, saga::session, std::string)

///////////////////////////////////////////////////////////////////////////////
static bool compare_urls(saga::url const& lhs, saga::url const& rhs)
{
    return lhs == rhs;
}

///////////////////////////////////////////////////////////////////////////////
void register_url()
{
    // saga::url
    class_<saga::url>("url", "saga::url type")
        .def(init<std::string>())

        INSERT_PYTHON_WRAPPER_1_EX(translate_without_session, 
            translate, "translates the given string into a different url")
        INSERT_PYTHON_WRAPPER_2_EX(translate_with_session, 
            translate, "translates the given string into a different url, using the given session")

        .add_property("url", &saga::url::get_url, &saga::url::set_url, 
            "returns or modifies the string representation of the full "
            "saga::url held by this instance")
        .add_property("scheme", &saga::url::get_scheme, &saga::url::set_scheme, 
            "returns or modifies the scheme contained in this saga::url instance")
        .add_property("host", &saga::url::get_host, &saga::url::set_host, 
            "returns or modifies the host contained in this saga::url instance")
        .add_property("port", &saga::url::get_port, &saga::url::set_port, 
            "returns or modifies the port number contained in this saga::url instance")
        .add_property("fragment", &saga::url::get_fragment, &saga::url::set_fragment, 
            "returns or modifies the fragment contained in this saga::url instance")
        .add_property("path", &saga::url::get_path, &saga::url::set_path, 
            "returns or modifies the path contained in this saga::url instance")
        .add_property("userinfo", &saga::url::get_userinfo, &saga::url::set_userinfo, 
            "returns or modifies the userinfo contained in this saga::url instance")
        .def("clone", &saga::url::clone, 
            "returns a new (deep) copy of the saga::url instance")

        .add_property("authority", &saga::url::get_authority, &saga::url::set_authority, 
            "returns or modifies the authority contained in this saga::url instance")
        .add_property("query", &saga::url::get_query, &saga::url::set_query, 
            "returns or modifies the query contained in this saga::url instance")
        .add_property("username", &saga::url::get_username, &saga::url::set_username, 
            "returns or modifies the username contained in this saga::url instance")
        .add_property("password", &saga::url::get_password, &saga::url::set_password, 
            "returns or modifies the password contained in this saga::url instance")

        .def("get_string", &saga::url::get_string, 
            "returns the string representation of this instance")

        .def("__str__", &saga::url::get_url, 
            "returns the string representation of this instance")
        .def("__cmp__", &compare_urls, 
            "compares two saga::url instances")
    ;
}
