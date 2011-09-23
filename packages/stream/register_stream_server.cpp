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
DEFINE_PYTHON_WRAPPERS_2(void, saga::stream::server, permissions_allow, std::string, int)
DEFINE_PYTHON_WRAPPERS_2(void, saga::stream::server, permissions_deny, std::string, int)
DEFINE_PYTHON_WRAPPERS_2(bool, saga::stream::server, permissions_check, std::string, int)
DEFINE_PYTHON_WRAPPERS_0(std::string, saga::stream::server, get_owner)
DEFINE_PYTHON_WRAPPERS_0(std::string, saga::stream::server, get_group)

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_0(saga::url, saga::stream::server, get_url)
DEFINE_PYTHON_WRAPPERS_1_DEF_1(saga::stream::stream, saga::stream::server, connect, double, -1.0)
DEFINE_PYTHON_WRAPPERS_1_DEF_1(saga::stream::stream, saga::stream::server, serve, double, 0.0)
DEFINE_PYTHON_WRAPPERS_1_DEF_1(void, saga::stream::server, close, double, 0.0)

///////////////////////////////////////////////////////////////////////////////
static saga::monitorable::cookie_handle 
add_stream_server_cb(saga::stream::server s, std::string name, 
  boost::python::object f)
{
    return s.add_callback(name, saga::python::python_callback(f));
}

///////////////////////////////////////////////////////////////////////////////
static saga::monitorable::cookie_handle 
add_stream_server_cb_obj(saga::stream::server s, std::string name, 
    boost::python::object o, boost::python::object f)
{
    return s.add_callback(name, saga::python::python_callback_obj(o, f));
}

///////////////////////////////////////////////////////////////////////////////
void register_stream_server()
{
    {
    scope stream_server = // saga::stream::server
        class_<saga::stream::server, bases<saga::object> >("server")

            // additional constructors
            .def(init<std::string>())
            .def(init<saga::url>())
            .def(init<saga::session>())
            .def(init<saga::session, saga::url>())
            .def(init<saga::session, std::string>())
            
            // saga::permissions interface 
            INSERT_PYTHON_WRAPPER_2(permissions_allow, "set a permission flag for this object instance")
            INSERT_PYTHON_WRAPPER_2(permissions_deny, "remove a permission flag from this object instance")
            INSERT_PYTHON_WRAPPER_2(permissions_check, "check this object instance for a permission flag")
            INSERT_PYTHON_WRAPPER_0(get_owner, "returns the owner of this object instance")
            INSERT_PYTHON_WRAPPER_0(get_group, "returns the group of the owner of this object instance")

            // saga::monitorable interface
            .def("list_metrics", &saga::stream::server::list_metrics, 
                "returns the list of metrics associated with this stream server  instance")
            .def("get_metric", &saga::stream::server::get_metric, 
                "returns a specific metric associated with this stream server  instance")
            .def("add_callback", &add_stream_server_cb, 
                "add a new callback to this stream server instance")
            .def("add_callback", &add_stream_server_cb_obj, 
                "add a new callback to this stream server instance")
            .def("remove_callback", &saga::stream::server::remove_callback,
                "remove the given callback from this stream server  instance")

            // saga::job_service interface
            INSERT_PYTHON_WRAPPER_0(get_url, 
                "get the URL to be used to connect to this server")
            INSERT_PYTHON_WRAPPER_1_OVERLOADS(connect,
                "establishes a connection to the server")
            INSERT_PYTHON_WRAPPER_1_OVERLOADS(serve,
                "wait for incoming client connections")
            INSERT_PYTHON_WRAPPER_1_OVERLOADS(close, 
                "closes this stream service")
        ;
    }
}

