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
void register_job_self()
{
    {
    scope job_self = // saga::job_self
        class_<saga::job::self, bases<saga::job::job> >("self", no_init)

            // steerable interface
            .def("add_metric", &saga::job::self::add_metric,
                "add a metric to this job instance")
            .def("remove_metric", &saga::job::self::remove_metric,
                "remove a metric from this job instance")
            .def("fire_metric", &saga::job::self::fire_metric,
                "fire the given metric")
        ;
    }
}

