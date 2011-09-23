//  Copyright (c) 2011 Ole Weidner
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include <saga/saga/packages/isn/version.hpp>

#include <boost/function.hpp>
#include <boost/python.hpp>


#include "register_types.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
void register_isn_version()
{
    def("get_isn_package_version", &saga::get_isn_package_version, 
        "returns the version of the SAGA ISN package library");
    def("get_isn_package_api_version", &saga::get_isn_package_api_version, 
        "returns the SAGA isn package API version");
}
