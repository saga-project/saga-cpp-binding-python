//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include <boost/function.hpp>
#include <boost/python.hpp>


#include "register_types.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
void register_replica_version()
{
    def("get_replica_package_version", &saga::get_replica_package_version, 
        "returns the version of the SAGA replica package library");
    def("get_replica_package_api_version", &saga::get_replica_package_api_version, 
        "returns the SAGA replica package API version");
}
