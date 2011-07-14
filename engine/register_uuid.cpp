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
static bool compare_uuids(saga::uuid const& lhs, saga::uuid const& rhs)
{
    return lhs == rhs;
}

///////////////////////////////////////////////////////////////////////////////
void register_uuid()
{

    // saga::url
    class_<saga::uuid>("uuid", "saga::uuid type")
        .def(init<char const*>())
        .add_property("string", &saga::uuid::string, 
            "returns the string representation of this instance")
        .def("__str__", &saga::uuid::string, 
            "returns the string representation of this instance")
        .def("__cmp__", &compare_uuids, 
            "compares two saga::uuid instances")
    ;
}
