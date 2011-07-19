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
DEFINE_PYTHON_WRAPPERS_2(std::vector<saga::sd::service_description>, 
    saga::sd::discoverer, list_services, std::string, std::string)
DEFINE_PYTHON_WRAPPERS_3_EX(std::vector<saga::sd::service_description>, 
    saga::sd::discoverer, list_services_auth, list_services, std::string, 
    std::string, std::string)

///////////////////////////////////////////////////////////////////////////////
void register_sd_discoverer()
{
    {
    scope discoverer = // saga::sd::service_description
        class_<saga::sd::discoverer, bases<saga::object> >("discoverer")

            .def(init<>())
            .def(init<saga::url>())
            .def(init<saga::session>())
            .def(init<saga::session, saga::url>())

            INSERT_PYTHON_WRAPPER_2(list_services, 
                "Returns the set of services that pass the set of specified filters")
            INSERT_PYTHON_WRAPPER_3_EX(list_services_auth, list_services, 
                "Returns the set of services that pass the set of specified filters")

        ;
    }
}

