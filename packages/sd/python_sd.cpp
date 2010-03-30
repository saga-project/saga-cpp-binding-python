//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/python.hpp>
#include <saga/saga.hpp>

#include "register_types.hpp"

using namespace boost::python;

BOOST_PYTHON_MODULE(_sd)
{
    register_sd_version();
    register_sd_service_description();    // saga::sd::service_description
    register_sd_service_data();           // saga::sd::service_data
    register_sd_discoverer();             // saga::sd::discoverer
}

