//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/python.hpp>
#include <saga/saga.hpp>

#include "register_types.hpp"

using namespace boost::python;

BOOST_PYTHON_MODULE(_cpr)
{
    register_cpr_version();
    register_cpr_checkpoint();        // saga::cpr::checkpoint
    register_cpr_directory();         // saga::cpr::directory
    register_cpr_job_description();   // saga::cpr::job_description
    register_cpr_job();               // saga::cpr::job
    register_cpr_job_self();          // saga::cpr::job_self
    register_cpr_job_service();       // saga::cpr::service
}

