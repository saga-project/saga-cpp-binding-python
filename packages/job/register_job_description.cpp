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
DEFINE_PYTHON_WRAPPERS_1(std::string, saga::job::description, get_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::job::description, set_attribute, std::string, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::job::description, get_vector_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::job::description, set_vector_attribute, std::string, std::vector<std::string>)
DEFINE_PYTHON_WRAPPERS_0(std::vector<std::string>, saga::job::description, list_attributes)
DEFINE_PYTHON_WRAPPERS_1(void, saga::job::description, remove_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::job::description, find_attributes, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::job::description, attribute_exists, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::job::description, attribute_is_readonly, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::job::description, attribute_is_writable, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::job::description, attribute_is_vector, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::job::description, attribute_is_removable, std::string)

///////////////////////////////////////////////////////////////////////////////
#define DEFINE_PYTHON_ATTRIBUTE(ns, target, name)                             \
    std::string get_ ## name ## _attr(target o)                               \
    {                                                                         \
        return o.get_attribute(ns ## _ ## name);                              \
    }                                                                         \
    void set_ ## name ## _attr(target o, std::string val)                     \
    {                                                                         \
        return o.set_attribute(ns ## _ ## name, val);                         \
    }                                                                         \
    /**/

#define INSERT_PYTHON_ATTRIBUTE(name)                                         \
    .add_property(#name, get_ ## name ## _attr, set_ ## name ## _attr,        \
        "accesses the attribute '" #name "'")                                 \
    /**/

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, executable)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, working_directory)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, interactive)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, input)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, output)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, error)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, cleanup)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, job_start_time)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, total_cpu_time)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, wall_time_limit)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, total_physical_memory)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, cpu_architecture)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, operating_system_type)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, queue)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, job_contact)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, spmd_variation)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, total_cpu_count)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, number_of_processes)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, processes_per_host)
DEFINE_PYTHON_ATTRIBUTE(saga::job::attributes::description, saga::job::description, threads_per_process)

///////////////////////////////////////////////////////////////////////////////
#define DEFINE_PYTHON_VECTOR_ATTRIBUTE(ns, target, name)                      \
    std::vector<std::string> get_ ## name ## _attr(target o)                  \
    {                                                                         \
        return o.get_vector_attribute(ns ## _ ## name);                       \
    }                                                                         \
    void set_ ## name ## _attr(target o, std::vector<std::string> const& val) \
    {                                                                         \
        return o.set_vector_attribute(ns ## _ ## name, val);                  \
    }                                                                         \
    /**/

#define INSERT_PYTHON_VECTOR_ATTRIBUTE(name)                                  \
    .add_property(#name, get_ ## name ## _attr, set_ ## name ## _attr,        \
        "accesses the vector attribute '" #name "'")                          \
    /**/

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_VECTOR_ATTRIBUTE(saga::job::attributes::description, saga::job::description, arguments)
DEFINE_PYTHON_VECTOR_ATTRIBUTE(saga::job::attributes::description, saga::job::description, environment)
DEFINE_PYTHON_VECTOR_ATTRIBUTE(saga::job::attributes::description, saga::job::description, file_transfer)
DEFINE_PYTHON_VECTOR_ATTRIBUTE(saga::job::attributes::description, saga::job::description, candidate_hosts)
DEFINE_PYTHON_VECTOR_ATTRIBUTE(saga::job::attributes::description, saga::job::description, job_project)

///////////////////////////////////////////////////////////////////////////////
void register_job_description()
{
    {
    scope job_description = // saga::job::description
        class_<saga::job::description, bases<saga::object> >("description")

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

            // expose the possible attribute key names as members
            INSERT_PYTHON_ATTRIBUTE(executable)
            INSERT_PYTHON_VECTOR_ATTRIBUTE(arguments)
            INSERT_PYTHON_VECTOR_ATTRIBUTE(environment)
            INSERT_PYTHON_ATTRIBUTE(working_directory)
            INSERT_PYTHON_ATTRIBUTE(interactive)
            INSERT_PYTHON_ATTRIBUTE(input)
            INSERT_PYTHON_ATTRIBUTE(output)
            INSERT_PYTHON_ATTRIBUTE(error)
            INSERT_PYTHON_VECTOR_ATTRIBUTE(file_transfer)
            INSERT_PYTHON_ATTRIBUTE(cleanup)
            INSERT_PYTHON_ATTRIBUTE(job_start_time)
            INSERT_PYTHON_ATTRIBUTE(total_cpu_time)
            INSERT_PYTHON_ATTRIBUTE(wall_time_limit)
            INSERT_PYTHON_ATTRIBUTE(total_physical_memory)
            INSERT_PYTHON_ATTRIBUTE(cpu_architecture)
            INSERT_PYTHON_ATTRIBUTE(operating_system_type)
            INSERT_PYTHON_VECTOR_ATTRIBUTE(candidate_hosts)
            INSERT_PYTHON_ATTRIBUTE(queue)
            INSERT_PYTHON_ATTRIBUTE(job_contact)
            INSERT_PYTHON_VECTOR_ATTRIBUTE(job_project)
            INSERT_PYTHON_ATTRIBUTE(spmd_variation)
            INSERT_PYTHON_ATTRIBUTE(total_cpu_count)
            INSERT_PYTHON_ATTRIBUTE(number_of_processes)
            INSERT_PYTHON_ATTRIBUTE(processes_per_host)
            INSERT_PYTHON_ATTRIBUTE(threads_per_process)
        ;
    }
}

