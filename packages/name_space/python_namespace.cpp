//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/python.hpp>
#include <saga/saga.hpp>

#include "register_types.hpp"

using namespace boost::python;

BOOST_PYTHON_MODULE(_name_space)
{
    register_namespace_version();
    register_namespace_entry();        // saga::namespace_entry
    register_namespace_directory();    // saga::namespace_dir
}

