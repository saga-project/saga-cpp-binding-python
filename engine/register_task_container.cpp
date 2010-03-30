//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>

#include <boost/function.hpp>
#include <boost/python.hpp>

#include <saga/saga.hpp>

#include "register_types.hpp"
#include "container_mappings.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
static saga::task_container::cookie_handle 
add_task_cb(saga::task_container tc, std::string name, boost::python::object f)
{
    return tc.add_callback(name, saga::python::python_callback(f));
}

///////////////////////////////////////////////////////////////////////////////
static saga::task_container::cookie_handle 
add_task_cb_obj(saga::task_container tc, std::string name, boost::python::object s, 
    boost::python::object f)
{
    return tc.add_callback(name, saga::python::python_callback_obj(s, f));
}

///////////////////////////////////////////////////////////////////////////////
void register_task_container()
{
    // std::vector<std::task>
    saga::python::tuple_mapping_variable_capacity<std::vector<saga::task> >();

    // std::vector<saga::task::state>
    saga::python::tuple_mapping_variable_capacity<std::vector<saga::task_base::state> >();

    {
    scope task_container =  // saga::task_container
        class_<saga::task_container, bases<saga::object> >(
                "task_container", "saga::task_container type")
            .def("run", &saga::task_container::run)
            .def("cancel", &saga::task_container::cancel)
            .def("wait", &saga::task_container::wait)
            .def("add_task", &saga::task_container::add_task)
            .def("remove_task", &saga::task_container::remove_task)
            .def("list_tasks", &saga::task_container::list_tasks)
            .def("get_states", &saga::task_container::get_states)

            // monitorable interface
            .def("list_metrics", &saga::task_container::list_metrics, 
                "returns the list of metrics associated with this task_container instance")
            .def("get_metric", &saga::task_container::get_metric, 
                "returns a specific metric associated with this task_container instance")
            .def("add_callback", &add_task_cb, 
                "add a new callback to this task_container instance")
            .def("add_callback", &add_task_cb_obj, 
                "add a new callback to this task_container instance")
            .def("remove_callback", &saga::task_container::remove_callback,
                "remove the given callback from this task_container instance")
        ;
    }
}
