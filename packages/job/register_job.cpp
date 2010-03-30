//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <map>
#include <string>

#include <boost/function.hpp>
#include <boost/python.hpp>

#include <saga/saga.hpp>
#if defined(SAGA_WINDOWS)
#include <windows.h>    // for CloseHandle
#include <io.h>
#include <fcntl.h>
#endif

#include "register_types.hpp"

using namespace boost::python;

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_1(std::string, saga::job::job, get_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::job::job, set_attribute, std::string, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::job::job, get_vector_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_2(void, saga::job::job, set_vector_attribute, std::string, std::vector<std::string>)
DEFINE_PYTHON_WRAPPERS_0(std::vector<std::string>, saga::job::job, list_attributes)
DEFINE_PYTHON_WRAPPERS_1(void, saga::job::job, remove_attribute, std::string)
DEFINE_PYTHON_WRAPPERS_1(std::vector<std::string>, saga::job::job, find_attributes, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::job::job, attribute_exists, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::job::job, attribute_is_readonly, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::job::job, attribute_is_writable, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::job::job, attribute_is_vector, std::string)
DEFINE_PYTHON_WRAPPERS_1(bool, saga::job::job, attribute_is_removable, std::string)

///////////////////////////////////////////////////////////////////////////////
DEFINE_PYTHON_WRAPPERS_0(std::string, saga::job::job, get_job_id)
DEFINE_PYTHON_WRAPPERS_0(saga::job::state, saga::job::job, get_state)
DEFINE_PYTHON_WRAPPERS_0(saga::job::description, saga::job::job, get_description)
DEFINE_PYTHON_WRAPPERS_0(void, saga::job::job, run)
DEFINE_PYTHON_WRAPPERS_0(void, saga::job::job, suspend)
DEFINE_PYTHON_WRAPPERS_0(void, saga::job::job, resume)
DEFINE_PYTHON_WRAPPERS_0(void, saga::job::job, checkpoint)
DEFINE_PYTHON_WRAPPERS_1(void, saga::job::job, migrate, saga::job::description)
DEFINE_PYTHON_WRAPPERS_1(void, saga::job::job, signal, int)

DEFINE_PYTHON_WRAPPERS_1_DEF_1(void, saga::job::job, cancel, double, 0.0)
DEFINE_PYTHON_WRAPPERS_1_DEF_1(bool, saga::job::job, wait, double, -1.0)

///////////////////////////////////////////////////////////////////////////////
// this registry keeps saga streams alive as long as the Python file is open
template <typename SagaStream>
class handle_registry
{
private:
    typedef std::map<FILE*, SagaStream> registry_type;

public:
    bool add_handle(FILE* fp, SagaStream const& o)
    {
        typename registry_type::iterator it = registry_.find(fp);
        if (it != registry_.end())
        {
            (*it).second = o;
            return false;
        }
        registry_.insert(typename registry_type::value_type(fp, o));
        return true;
    }
    void remove_handle(FILE* fp)
    {
        registry_.erase(fp);
    }

private:
    registry_type registry_;
};

static handle_registry<saga::job::istream> iregistry;
static handle_registry<saga::job::ostream> oregistry;

static int iclose_file(FILE* fp)
{
    int result = fclose(fp);
    iregistry.remove_handle(fp);   // releases associated saga object
    return result;
}

static int oclose_file(FILE* fp)
{
    int result = fclose(fp);
    oregistry.remove_handle(fp);   // releases associated saga object
    return result;
}

///////////////////////////////////////////////////////////////////////////////
boost::python::object 
py_get_outstream(saga::job::istream const& strm, char const* name)
{
    saga::job::detail::handle_type h = saga::job::detail::get_handle(strm);
    if ((saga::job::detail::handle_type)(-1) == h) {
        SAGA_OSSTREAM strm;
        strm << "py_get_outstream(" << name 
             << "): invalid file handle: " << h << std::endl;
        SAGA_LOG_ERROR(SAGA_OSSTREAM_GETSTRING(strm).c_str());
        return boost::python::object();
    }

#if defined(SAGA_WINDOWS)
    FILE *fp = fdopen(_open_osfhandle((intptr_t)h, _O_RDONLY), "r");
#else
    FILE *fp = fdopen((int)h, "r");
#endif

    if (0 == fp) {
        SAGA_OSSTREAM strm;
        strm << "py_get_outstream(" << name 
             << "): failed to fdopen from handle: " << h << ", " 
             << strerror(errno) << std::endl;
        SAGA_LOG_ERROR(SAGA_OSSTREAM_GETSTRING(strm).c_str());
        return boost::python::object();
    }

    boost::python::object f (handle<>(PyFile_FromFile(fp, (char*)name, "r", iclose_file)));
    if (f.ptr() != NULL) {
        PyFile_SetBufSize(f.ptr(), -1);
        iregistry.add_handle(fp, strm);
    }
    else {
        SAGA_OSSTREAM strm;
        strm << "py_get_outstream(" << name 
             << "): failed to create Python file object from handle: " << h 
             << ", " << strerror(errno) << std::endl;
        SAGA_LOG_ERROR(SAGA_OSSTREAM_GETSTRING(strm).c_str());
        return boost::python::object();
    }

    saga::job::detail::get_handle(strm, true);    // detach handle
    {
        SAGA_OSSTREAM strm;
        strm << "py_get_outstream(" << name 
             << "): successfully created Python file object from handle: " << h 
             << std::endl;
        SAGA_LOG_DEBUG(SAGA_OSSTREAM_GETSTRING(strm).c_str());
    }

    return f;
}

boost::python::object 
py_get_instream(saga::job::ostream const& strm, char const* name)
{
    saga::job::detail::handle_type h = saga::job::detail::get_handle(strm);
    if ((saga::job::detail::handle_type)(-1) == h) {
        SAGA_OSSTREAM strm;
        strm << "py_get_instream(" << name 
             << "): invalid file handle: " << h << std::endl;
        SAGA_LOG_ERROR(SAGA_OSSTREAM_GETSTRING(strm).c_str());
        return boost::python::object();
    }

#if defined(SAGA_WINDOWS)
    FILE *fp = fdopen(_open_osfhandle((intptr_t)h, _O_RDWR), "w");
#else
    FILE *fp = fdopen((int)h, "w");
#endif

    if (0 == fp) {
        SAGA_OSSTREAM strm;
        strm << "py_get_instream(" << name 
             << "): failed to fdopen from handle: " << h << ", " 
             << strerror(errno) << std::endl;
        SAGA_LOG_ERROR(SAGA_OSSTREAM_GETSTRING(strm).c_str());
        return boost::python::object();
    }

    boost::python::object f(handle<>(PyFile_FromFile(fp, (char*)name, "w+", oclose_file)));
    if (f.ptr() != NULL) {
        PyFile_SetBufSize(f.ptr(), -1);
        oregistry.add_handle(fp, strm);
    }
    else {
        SAGA_OSSTREAM strm;
        strm << "py_get_instream(" << name 
             << "): failed to create Python file object from handle: " << h 
             << ", " << strerror(errno) << std::endl;
        SAGA_LOG_ERROR(SAGA_OSSTREAM_GETSTRING(strm).c_str());
        return boost::python::object();
    }

    saga::job::detail::get_handle(strm, true);    // detach handle

    {
        SAGA_OSSTREAM strm;
        strm << "py_get_instream(" << name 
             << "): successfully created Python file object from handle: " << h 
             << std::endl;
        SAGA_LOG_DEBUG(SAGA_OSSTREAM_GETSTRING(strm).c_str());
    }

    return f;
}

// DEFINE_PYTHON_WRAPPERS_0(boost::python::object, saga::job, get_stdin)
static boost::python::object py_get_stdin_0(saga::job::job o)
{
    saga::job::ostream strm (o.get_stdin());
    boost::python::detail::do_python allow_python;
    return py_get_instream(strm, "<get_stdin>");
}
static saga::task py_get_stdin_task_0(saga::job::job o,
    saga::python::routine_type type)
{
    SAGA_THROW_VERBATIM(o,
        "The asynchronous version of get_stdin is not implemented",
        saga::BadParameter);
    return saga::task();
}

// DEFINE_PYTHON_WRAPPERS_0(saga::istream, saga::job, get_stdout)
static boost::python::object py_get_stdout_0(saga::job::job o)
{
    saga::job::istream strm (o.get_stdout());
    boost::python::detail::do_python allow_python;
    return py_get_outstream(strm, "<get_stdout>");
}
static saga::task py_get_stdout_task_0(saga::job::job o,
    saga::python::routine_type type)
{
    SAGA_THROW_VERBATIM(o,
        "The asynchronous version of get_stdout is not implemented",
        saga::BadParameter);
    return saga::task();
}

// DEFINE_PYTHON_WRAPPERS_0(saga::istream, saga::job, get_stderr)
static boost::python::object py_get_stderr_0(saga::job::job o)
{
    saga::job::istream strm (o.get_stderr());
    boost::python::detail::do_python allow_python;
    return py_get_outstream(strm, "<get_stderr>");
}
static saga::task py_get_stderr_task_0(saga::job::job o,
    saga::python::routine_type type)
{
    SAGA_THROW_VERBATIM(o,
        "The asynchronous version of get_stderr is not implemented",
        saga::BadParameter);
    return saga::task();
}

///////////////////////////////////////////////////////////////////////////////
void register_job()
{
    {
    scope job = // saga::job
        class_<saga::job::job, bases<saga::task> >("job", no_init)

            // saga::attribute interface 
            INSERT_PYTHON_WRAPPER_1(get_attribute, "returns a (scalar) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_2(set_attribute, "sets a (scalar) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_1(get_vector_attribute, "returns a (vector) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_2(set_vector_attribute, "sets a (vector) attribute associated with this object")
            INSERT_PYTHON_WRAPPER_0(list_attributes, "lists the keys of all attributes associated with this object")
            INSERT_PYTHON_WRAPPER_1(remove_attribute, "removes an attribute associated with this object")
            INSERT_PYTHON_WRAPPER_1(find_attributes, "find attributes matching the given pattern")
            INSERT_PYTHON_WRAPPER_1(attribute_exists, "tests if the given attribute exists for this instance")
            INSERT_PYTHON_WRAPPER_1(attribute_is_readonly, "tests if the given attribute is read only")
            INSERT_PYTHON_WRAPPER_1(attribute_is_writable, "tests if the given attribute is writable")
            INSERT_PYTHON_WRAPPER_1(attribute_is_vector, "tests if the given attribute is a vector attribute")
            INSERT_PYTHON_WRAPPER_1(attribute_is_removable, "tests if the given attribute is removable")

            // saga::job interface
            INSERT_PYTHON_WRAPPER_0(get_job_id, "get the job id of this job")
            INSERT_PYTHON_WRAPPER_0(get_state, "get the state of this job")
            INSERT_PYTHON_WRAPPER_0(get_description, 
                "get a copy of the job description this job was created from")
            INSERT_PYTHON_WRAPPER_0(get_stdin, "get a file object representing the stdin of the spawned job")
            INSERT_PYTHON_WRAPPER_0(get_stdout, "get a file object representing the stdout of the spawned job")
            INSERT_PYTHON_WRAPPER_0(get_stderr, "get a file object representing the stderr of the spawned job")
            INSERT_PYTHON_WRAPPER_0(run, "run this job")
            INSERT_PYTHON_WRAPPER_0(suspend, "suspend this job")
            INSERT_PYTHON_WRAPPER_0(resume, "resume this job")
            INSERT_PYTHON_WRAPPER_0(checkpoint, "checkpoint this job")
            INSERT_PYTHON_WRAPPER_1(migrate, "migrate this job")
            INSERT_PYTHON_WRAPPER_1(signal, "send a signal to this job")

            INSERT_PYTHON_WRAPPER_1(cancel, "cancel this job")
            INSERT_PYTHON_WRAPPER_1(wait, "wait for this job")
        ;
    }

    // saga::job::state
    enum_<saga::job::state>("job_state")
        .value("Unknown", saga::job::Unknown)
        .value("New", saga::job::New)
        .value("Running", saga::job::Running)
        .value("Failed", saga::job::Failed)
        .value("Done", saga::job::Done)
        .value("Canceled", saga::job::Canceled)
        .value("Suspended", saga::job::Suspended)
        .export_values()
    ;
}

