//  Copyright (c) 2011 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <string>

#include <saga/saga.hpp>
#include <saga/saga/isn.hpp>
#include <boost/function.hpp>
#include <boost/python.hpp>


#include "register_types.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_1(std::string, saga::isn::entity_data, get_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::isn::entity_data, set_attribute, std::string, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::isn::entity_data, get_vector_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::isn::entity_data, set_vector_attribute, std::string, std::vector<std::string>)
DEFINE_PYTHON_WRAPPERS_0(std::vector<std::string>, saga::isn::entity_data, list_attributes)
//DEFINE_PYTHON_WRAPPERS_1(void, saga::isn::entity_data, remove_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::isn::entity_data, find_attributes, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::isn::entity_data, attribute_exists, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::isn::entity_data, attribute_is_readonly, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::isn::entity_data, attribute_is_writable, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::isn::entity_data, attribute_is_vector, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::isn::entity_data, attribute_is_removable, std::string)

///////////////////////////////////////////////////////////////////////////////
void register_isn_entity_data()
{
    {
    scope entity_data = // saga::isn::emtity_data
        class_<saga::isn::entity_data, bases<saga::object> >("entity_data")

            // saga::attribute interface 
            INSERT_PYTHON_WRAPPER_1(get_attribute, "returns a (scalar) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_2(set_attribute, "sets a (scalar) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_1(get_vector_attribute, "returns a (vector) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_2(set_vector_attribute, "sets a (vector) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_0(list_attributes, "lists the keys of all attributes associated with this object")
            //INSERT_PYTHON_WRAPPER_1(remove_attribute, "removes an attribute associated with this object")
            INSERT_PYTHON_WRAPPER_1(find_attributes, "find attributes matching the given pattern")
            INSERT_PYTHON_WRAPPER_1(attribute_exists, "tests if the given attribute exists for this instance")
            INSERT_PYTHON_WRAPPER_1(attribute_is_readonly, "tests if the given attribute is read only")
            INSERT_PYTHON_WRAPPER_1(attribute_is_writable, "tests if the given attribute is writable")
            INSERT_PYTHON_WRAPPER_1(attribute_is_vector, "tests if the given attribute is a vector attribute")
            INSERT_PYTHON_WRAPPER_1(attribute_is_removable, "tests if the given attribute is removable")
            ;
    }
}
