//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>

#include <boost/function.hpp>
#include <boost/python.hpp>
#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>

#include <saga/saga.hpp>

#include "register_types.hpp"
#include "container_mappings.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
static saga::monitorable::cookie_handle 
add_task_cb(saga::task t, std::string name, boost::python::object f)
{
    return t.add_callback(name, saga::python::python_callback(f));
}

///////////////////////////////////////////////////////////////////////////////
static saga::monitorable::cookie_handle 
add_task_cb_obj(saga::task t, std::string name, boost::python::object s, 
    boost::python::object f)
{
    return t.add_callback(name, saga::python::python_callback_obj(s, f));
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(task_overloads, wait, 0, 1);

///////////////////////////////////////////////////////////////////////////////
void register_task()
{
    // std::vector<saga::url>
    saga::python::tuple_mapping_variable_capacity<std::vector<saga::url> >();

    {
    // get the function pointers to the get_task specializations
        std::string const& (saga::task::*get_result_string)() const = 
            &saga::task::get_result<std::string>;
        bool const& (saga::task::*get_result_bool)() const = 
            &saga::task::get_result<bool>;
        saga::url const& (saga::task::*get_result_url)() const = 
            &saga::task::get_result<saga::url>;
        std::vector<saga::url> const& (saga::task::*get_result_url_list)() const = 
            &saga::task::get_result<std::vector<saga::url> >;

    scope task = // saga::task
        class_<saga::task, bases<saga::object> >("task", "saga::task type")
            // additional constructor
            .def(init<saga::task_base::state>())

            // saga::task interface
            .def("run", &saga::task::run, 
                "run this task instance")
            .def("cancel", &saga::task::cancel, 
                "cancel this task instance")
            .def("wait", &saga::task::wait, 
                task_overloads("wait for this task instance to finish"))
            .def("rethrow", &saga::task::rethrow, 
                "rethrow any error occurred during execution of task")

            .def("get_state", &saga::task::get_state, 
                "returns the state of this task instance")
            .add_property("state", &saga::task::get_state, 
                "returns the state of this task instance")
            .def("get_object", &saga::task::get_object, 
                "returns the SAGA object associated with this task instance")
            .add_property("object", &saga::task::get_object, 
                "returns the SAGA object associated with this task instance")

            .def("get_result", get_result_string, 
                "returns the result of the operation executed by this task instance",
                return_value_policy<copy_const_reference>())
            .def("get_result", get_result_bool, 
                "returns the result of the operation executed by this task instance",
                return_value_policy<copy_const_reference>())
            .def("get_result", get_result_url, 
                "returns the result of the operation executed by this task instance",
                return_value_policy<copy_const_reference>())
            .def("get_result", get_result_url_list, 
                "returns the result of the operation executed by this task instance",
                return_value_policy<copy_const_reference>())

            // monitorable interface
            .def("list_metrics", &saga::task::list_metrics, 
                "returns the list of metrics associated with this task instance")
            .def("get_metric", &saga::task::get_metric, 
                "returns a specific metric associated with this task instance")
            .def("add_callback", &add_task_cb, 
                "add a new callback to this task instance")
            .def("add_callback", &add_task_cb_obj, 
                "add a new callback to this task instance")
            .def("remove_callback", &saga::task::remove_callback,
                "remove the given callback from this task instance")
        ;

        // saga::task::state
        enum_<saga::task_base::state>("task_state")
            .value("Unknown", saga::task_base::Unknown)
            .value("New", saga::task_base::New)
            .value("Running", saga::task_base::Running)
            .value("Failed", saga::task_base::Failed)
            .value("Done", saga::task_base::Done)
            .value("Canceled", saga::task_base::Canceled)
            .export_values()
        ;

        // routine_type
        enum_<saga::python::routine_type>("routine_type")
            .value("Sync", saga::python::Sync)
#if PY_VERSION_HEX >= 0x02040000
            .value("Async", saga::python::Async)
            .value("Task", saga::python::Task)
#endif
            .export_values()
        ;
    }
}
