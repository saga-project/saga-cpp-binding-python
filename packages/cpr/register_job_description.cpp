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
void register_cpr_job_description()
{
    {
    scope job_description = // saga::cpr::description
        class_<saga::cpr::description, bases<saga::job::description> >("description")
        ;
    }
}

