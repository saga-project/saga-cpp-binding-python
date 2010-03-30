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
void register_stream_version()
{
    def("get_stream_package_version", &saga::get_stream_package_version, 
        "returns the version of the SAGA stream package library");
    def("get_stream_package_api_version", &saga::get_stream_package_api_version, 
        "returns the SAGA stream package API version");
}
