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
#include "../../engine/container_mappings.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_1(std::string, saga::sd::service_description, get_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::sd::service_description, get_vector_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_0(std::vector<std::string>, saga::sd::service_description, list_attributes)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::sd::service_description, find_attributes, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::sd::service_description, attribute_exists, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::sd::service_description, attribute_is_readonly, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::sd::service_description, attribute_is_writable, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::sd::service_description, attribute_is_vector, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::sd::service_description, attribute_is_removable, std::string)

///////////////////////////////////////////////////////////////////////////////
#define DEFINE_PYTHON_ATTRIBUTE(ns, target, name)                             \
    std::string get_ ## name ## _attr(target o)                               \
    {                                                                         \
        return o.get_attribute(ns ## _ ## name);                              \
    }                                                                         \
    /**/

#define INSERT_PYTHON_ATTRIBUTE(name)                                         \
    .add_property(#name, get_ ## name ## _attr,                               \
        "read the attribute '" #name "'")                                     \
    /**/

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_ATTRIBUTE(saga::sd::attributes::service_description, saga::sd::service_description, url)
DEFINE_PYTHON_ATTRIBUTE(saga::sd::attributes::service_description, saga::sd::service_description, type)
DEFINE_PYTHON_ATTRIBUTE(saga::sd::attributes::service_description, saga::sd::service_description, uid)
DEFINE_PYTHON_ATTRIBUTE(saga::sd::attributes::service_description, saga::sd::service_description, site)
DEFINE_PYTHON_ATTRIBUTE(saga::sd::attributes::service_description, saga::sd::service_description, name)
DEFINE_PYTHON_ATTRIBUTE(saga::sd::attributes::service_description, saga::sd::service_description, relatedservices)

///////////////////////////////////////////////////////////////////////////////
#define DEFINE_PYTHON_VECTOR_ATTRIBUTE(ns, target, name)                      \
    std::vector<std::string> get_ ## name ## _attr(target o)                  \
    {                                                                         \
        return o.get_vector_attribute(ns ## _ ## name);                       \
    }                                                                         \
    /**/

#define INSERT_PYTHON_VECTOR_ATTRIBUTE(name)                                  \
    .add_property(#name, get_ ## name ## _attr,                               \
        "read the vector attribute '" #name "'")                              \
    /**/

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_VECTOR_ATTRIBUTE(saga::sd::attributes::service_description, saga::sd::service_description, vo)
DEFINE_PYTHON_VECTOR_ATTRIBUTE(saga::sd::attributes::service_description, saga::sd::service_description, information_provider_url)

///////////////////////////////////////////////////////////////////////////////
void register_sd_service_description()
{
    // std::vector<std::string>
    saga::python::tuple_mapping_variable_capacity<std::vector<saga::sd::service_description> >();

    {
    scope service_description = // saga::sd::service_description
        class_<saga::sd::service_description, bases<saga::object> >(
            "service_description", no_init)

            // saga::attribute interface 
            INSERT_PYTHON_WRAPPER_1(get_attribute, "returns a (scalar) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_1(get_vector_attribute, "returns a (vector) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_0(list_attributes, "lists the keys of all attributes associated with this object")
            INSERT_PYTHON_WRAPPER_1(find_attributes, "find attributes matching the given pattern")
            INSERT_PYTHON_WRAPPER_1(attribute_exists, "tests if the given attribute exists for this instance")
            INSERT_PYTHON_WRAPPER_1(attribute_is_readonly, "tests if the given attribute is read only")
            INSERT_PYTHON_WRAPPER_1(attribute_is_writable, "tests if the given attribute is writable")
            INSERT_PYTHON_WRAPPER_1(attribute_is_vector, "tests if the given attribute is a vector attribute")
            INSERT_PYTHON_WRAPPER_1(attribute_is_removable, "tests if the given attribute is removable")

            // expose the possible attribute key names as members
            INSERT_PYTHON_VECTOR_ATTRIBUTE(information_provider_url)
            INSERT_PYTHON_ATTRIBUTE(name)
            INSERT_PYTHON_ATTRIBUTE(relatedservices)
            INSERT_PYTHON_ATTRIBUTE(site)
            INSERT_PYTHON_ATTRIBUTE(type)
            INSERT_PYTHON_ATTRIBUTE(uid)
            INSERT_PYTHON_ATTRIBUTE(url)
            INSERT_PYTHON_VECTOR_ATTRIBUTE(vo)

            // additional functionality
            .def("get_url", &saga::sd::service_description::get_url, 
                "Returns the URL to contact the service")
            .def("get_related_services", &saga::sd::service_description::get_related_services, 
                "Returns a vector of related services")
            .def("get_data", &saga::sd::service_description::get_data, 
                "Returns a service_data object with the service data key/value pairs",
                return_value_policy<copy_non_const_reference>() )
        ;
    }
}

