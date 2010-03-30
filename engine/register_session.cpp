//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <list>

#include <boost/function.hpp>
#include <boost/python.hpp>

#include <saga/saga.hpp>

#include "register_types.hpp"
#include "container_mappings.hpp"

using namespace boost::python;

static saga::session get_default_session()
{
    return saga::get_default_session();
}

///////////////////////////////////////////////////////////////////////////////
void register_session()
{
    // std::vector<saga::context>
    saga::python::tuple_mapping_variable_capacity<std::vector<saga::context> >();

    // saga::session
    class_<saga::session, bases<saga::object> >("session", "saga::session type")
        // additional constructor
//         .def(init<bool>())
        // saga::session interface
        .def("add_context", &saga::session::add_context,
            "add a context to the list of contexts held by this session")
        .def("remove_context", &saga::session::remove_context,
            "remove a context from the list of contexts held by this session")
        .def("list_contexts", &saga::session::list_contexts,
            "list all contexts held by this session")
        .def("close", &saga::session::close, "closes this session")
    ;

    def("get_the_session", &get_default_session, 
        "returns the global (predefined) saga::session object");
    def("get_default_session", &get_default_session, 
        "returns the global (predefined) saga::session object");
}
