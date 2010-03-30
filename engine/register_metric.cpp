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
DEFINE_PYTHON_WRAPPERS_1(std::string, saga::metric, get_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::metric, set_attribute, std::string, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::metric, get_vector_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::metric, set_vector_attribute, std::string, std::vector<std::string>)
DEFINE_PYTHON_WRAPPERS_0(std::vector<std::string>, saga::metric, list_attributes)
DEFINE_PYTHON_WRAPPERS_1(void, saga::metric, remove_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::metric, find_attributes, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::metric, attribute_exists, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::metric, attribute_is_readonly, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::metric, attribute_is_writable, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::metric, attribute_is_vector, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::metric, attribute_is_removable, std::string)

///////////////////////////////////////////////////////////////////////////////
static saga::metric::metric_cookie 
add_metric_cb(saga::metric m, boost::python::object f)
{
    return m.add_callback(saga::python::python_callback(f));
}

///////////////////////////////////////////////////////////////////////////////
static saga::metric::metric_cookie 
add_metric_cb_obj(saga::metric m, boost::python::object s, 
    boost::python::object f)
{
    return m.add_callback(saga::python::python_callback_obj(s, f));
}

///////////////////////////////////////////////////////////////////////////////
void register_metric()
{
    // std::vector<saga::metric>
    saga::python::tuple_mapping_variable_capacity<std::vector<saga::metric> >();
    
    {
    scope metric =  // saga::metric
        class_<saga::metric, bases<saga::object> >(
                "metric", "saga::metric type")

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
            
            // additional constructor
            .def(init<saga::object, std::string, std::string, std::string, 
                      std::string, std::string, std::string>())

            // saga::metric interface
            .def("add_callback", add_metric_cb)
            .def("add_callback", add_metric_cb_obj)
            .def("remove_callback", &saga::metric::remove_callback)

            .def("fire", &saga::metric::fire, 
                "push the current state to the implementation")
        ;

        // enum saga::metric::frequency
        enum_<saga::metric::frequency>("frequency")
            .value("Unknown", saga::metric::Unknown)
            .value("Cont", saga::metric::Cont)
            .value("Descreet", saga::metric::Descreet)
            .value("Pull", saga::metric::Pull)
            .value("Push", saga::metric::Push)
            .value("Event", saga::metric::Event)
            .export_values()
        ;
    }
}
