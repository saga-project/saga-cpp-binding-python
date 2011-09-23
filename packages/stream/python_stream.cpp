//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <saga/saga.hpp>
#include <boost/function.hpp>
#include <boost/python.hpp>


#include "register_types.hpp"

using namespace boost::python;

BOOST_PYTHON_MODULE(_sd)
{
    register_stream_version();
    register_stream_stream();              // saga::stream::stream
    register_stream_server();              // saga::stream::server
}

