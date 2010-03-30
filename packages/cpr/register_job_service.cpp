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
DEFINE_PYTHON_WRAPPERS_1_EX(saga::cpr::job, saga::cpr::service, create_job_cpr, 
    create_job, saga::cpr::description)
DEFINE_PYTHON_WRAPPERS_2_EX(saga::cpr::job, saga::cpr::service, create_job_restart, 
    create_job, saga::cpr::description, saga::cpr::description)

///////////////////////////////////////////////////////////////////////////////
void register_cpr_job_service()
{
    {
    scope job_service = // saga::job_service
        class_<saga::cpr::service, bases<saga::job::service> >("service")

            // additional constructors
            .def(init<saga::url>())
            .def(init<saga::session>())
            .def(init<saga::session, saga::url>())
            
            // saga::job_service interface
            INSERT_PYTHON_WRAPPER_1_EX(create_job_cpr, create_job, 
                "create a new job using the given job description")
            INSERT_PYTHON_WRAPPER_2_EX(create_job_restart, create_job,
                "create a new job using the given job description")
        ;
    }
}

