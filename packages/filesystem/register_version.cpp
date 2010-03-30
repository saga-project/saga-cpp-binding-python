//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/python.hpp>
#include <saga/saga.hpp>

#include "register_types.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
void register_file_version()
{
    def("get_file_package_version", &saga::get_filesystem_package_version, 
        "returns the version of the SAGA file package library");
    def("get_file_package_api_version", &saga::get_filesystem_package_api_version, 
        "returns the SAGA file package API version");
}
