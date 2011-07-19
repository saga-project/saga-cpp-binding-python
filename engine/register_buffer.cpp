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
void register_buffer()
{
    // saga::url
    class_<saga::mutable_buffer>("buffer", "saga::buffer type")
        // additional constructor
        .def(init<saga::ssize_t>())

        // properties
        .add_property("size", &saga::mutable_buffer::get_size, 
            &saga::mutable_buffer::set_size, 
            "gets/sets the size of the buffer held internally")
    ;
}

