//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/python.hpp>
#include <saga/saga.hpp>

#include "register_types.hpp"

using namespace boost::python;

BOOST_PYTHON_MODULE(_job)
{
    register_job_version();
    register_job();               // saga::job
    register_job_self();          // saga::job_self
    register_job_service();       // saga::job_service
    register_job_description();   // saga::job_description
}

