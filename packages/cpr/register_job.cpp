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
DEFINE_PYTHON_WRAPPERS_1_DEF_1(void, saga::cpr::job, checkpoint, saga::url, saga::url())
DEFINE_PYTHON_WRAPPERS_1_DEF_1(void, saga::cpr::job, recover, saga::url, saga::url())
DEFINE_PYTHON_WRAPPERS_1_DEF_1(void, saga::cpr::job, cpr_stage_in, saga::url, saga::url())
DEFINE_PYTHON_WRAPPERS_1_DEF_1(void, saga::cpr::job, cpr_stage_out, saga::url, saga::url())
DEFINE_PYTHON_WRAPPERS_0(std::vector<saga::url>, saga::cpr::job, cpr_list)
DEFINE_PYTHON_WRAPPERS_0(saga::url, saga::cpr::job, cpr_last)

///////////////////////////////////////////////////////////////////////////////
void register_cpr_job()
{
    {
    scope job = // saga::cpr
        class_<saga::cpr::job, bases<saga::job::job> >("job", no_init)

            // saga::cpr::job interface
            INSERT_PYTHON_WRAPPER_1_OVERLOADS(checkpoint, "checkpoint a job")
            INSERT_PYTHON_WRAPPER_1_OVERLOADS(recover, "recover a job")
            INSERT_PYTHON_WRAPPER_1_OVERLOADS(cpr_stage_in, "stage-in a checpoint file")
            INSERT_PYTHON_WRAPPER_1_OVERLOADS(cpr_stage_out, "stage-out a checpoint file")
            INSERT_PYTHON_WRAPPER_0(cpr_list, "get list of known checkpoints")
            INSERT_PYTHON_WRAPPER_0(cpr_last, "get last known checkpoint")
        ;
    }

    // saga::cpr::state
    enum_<saga::cpr::state>("cpr_job_state")
        .value("Unknown", saga::cpr::Unknown)
        .value("New", saga::cpr::New)
        .value("Running", saga::cpr::Running)
        .value("Failed", saga::cpr::Failed)
        .value("Done", saga::cpr::Done)
        .value("Canceled", saga::cpr::Canceled)
        .value("Suspended", saga::cpr::Suspended)
        .export_values()
    ;
}

