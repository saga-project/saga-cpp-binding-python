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
DEFINE_PYTHON_WRAPPERS_1(saga::job::job, saga::job::service, create_job, saga::job::description)
//DEFINE_PYTHON_WRAPPERS_2((saga::job::job, saga::ostream, saga::istream, saga::istream), saga::job::service, run_job, std::string, std::string)
static boost::python::object py_run_job_2(saga::job::service o, 
    std::string commandline, std::string hostname)
{
    saga::job::istream stdout_strm;
    saga::job::istream stderr_strm;
    saga::job::ostream stdin_strm;

    saga::job::job j (o.run_job(commandline, hostname, stdin_strm, 
        stdout_strm, stderr_strm));

    boost::python::detail::do_python allow_python;
    return make_tuple(j, py_get_instream(stdin_strm, "<get_stdin>"), 
        py_get_outstream(stdout_strm, "<get_stdout>"), 
        py_get_outstream(stderr_strm, "<get_stderr>"));
}

static saga::task py_run_job_task_2(saga::job::job o,
    saga::python::routine_type type)
{
    SAGA_THROW_VERBATIM(o,
        "The asynchronous version of run_job is not implemented",
        saga::BadParameter);
    return saga::task();
}

DEFINE_PYTHON_WRAPPERS_2_EX(saga::job::job, saga::job::service, run_job_noio, run_job, std::string, std::string)
DEFINE_PYTHON_WRAPPERS_0(std::vector<std::string>, saga::job::service, list)
DEFINE_PYTHON_WRAPPERS_1(saga::job::job, saga::job::service, get_job, std::string)
DEFINE_PYTHON_WRAPPERS_0(saga::job::self, saga::job::service, get_self)

///////////////////////////////////////////////////////////////////////////////
void register_job_service()
{
    {
    scope job_service = // saga::job_service
        class_<saga::job::service, bases<saga::object> >("service")

            // additional constructors
            .def(init<char const*>())
            .def(init<saga::url>())
            .def(init<saga::session>())
            .def(init<saga::session, saga::url>())

            // saga::job_service interface
            INSERT_PYTHON_WRAPPER_1(create_job, 
                "create a new job using the given job description")
            INSERT_PYTHON_WRAPPER_2(run_job, 
                "create and run a new job given the executable path and arguments")
            INSERT_PYTHON_WRAPPER_2(run_job_noio, 
                "create and run a new job given the executable path and arguments")
            INSERT_PYTHON_WRAPPER_0(list, 
                "list the job id's of all jobs maintained by this instance")
            INSERT_PYTHON_WRAPPER_1(get_job, 
                "get the job given the job id maintained by this instance")
            INSERT_PYTHON_WRAPPER_0(get_self, 
                "get the job representing the running job instance")
        ;
    }
}

