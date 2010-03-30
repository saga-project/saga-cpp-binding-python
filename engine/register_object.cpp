//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/function.hpp>
#include <boost/python.hpp>
#include <saga/saga.hpp>

#include "register_types.hpp"

using namespace boost::python;

static saga::session object_get_session(saga::object const& obj)
{
    return obj.get_session();
}

///////////////////////////////////////////////////////////////////////////////
void register_object()
{
    // saga::permission
  enum_<saga::permissions::permission>("permission")
        .value("None",  saga::permissions::None)
        .value("Query", saga::permissions::Query)
        .value("Read",  saga::permissions::Read)
        .value("Write", saga::permissions::Write)
        .value("Exec",  saga::permissions::Exec)
        .value("Owner", saga::permissions::Owner)
        .value("All",   saga::permissions::All)
        .export_values()
    ;

    {
    scope object =  // saga::object
        class_<saga::object>("object", "saga::object type")
            .def("get_id", &saga::object::get_id, 
                "returns the id of this instance")
            .add_property("id", &saga::object::get_id, 
                "returns the id of this instance")
            .def("get_type", &saga::object::get_type, 
                "returns the type of this instance")
            .add_property("type", &saga::object::get_type, 
                "returns the type of this instance")
            .def("get_session", object_get_session, 
                "return the session associated with this object instance")
            .add_property("session", object_get_session, 
                "return the session associated with this object instance")
            .def("clone", &saga::object::clone,
                "create a deep copy of the object")
        ;

        def("get_object_type_name", &saga::get_object_type_name,
            "return the name of a given object type");

        // saga::object::type
        enum_<saga::object::type>("object_type")
            .value("Exception", saga::object::Exception)
            .value("URL", saga::object::URL)
            .value("Buffer", saga::object::Buffer)
            .value("Session", saga::object::Session)
            .value("Context", saga::object::Context)
            .value("Task", saga::object::Task)
            .value("TaskContainer", saga::object::TaskContainer)
            .value("Metric", saga::object::Metric)
            .value("NSEntry", saga::object::NSEntry)
            .value("NSDirectory", saga::object::NSDirectory)
            .value("IOVec", saga::object::IOVec)
            .value("File", saga::object::File)
            .value("Directory", saga::object::Directory)
            .value("LogicalFile", saga::object::LogicalFile)
            .value("LogicalDirectory", saga::object::LogicalDirectory)
            .value("JobDescription", saga::object::JobDescription)
            .value("JobService", saga::object::JobService)
            .value("Job", saga::object::Job)
            .value("JobSelf", saga::object::JobSelf)
            .value("StreamServer", saga::object::StreamServer)
            .value("Stream", saga::object::Stream)
            .value("Parameter", saga::object::Parameter)
            .value("RPC", saga::object::RPC)
            .value("Msg", saga::object::Msg)
            .value("Endpoint", saga::object::Endpoint)
            .value("Advert", saga::object::Advert)
            .value("AdvertDirectory", saga::object::AdvertDirectory)
            .value("ServiceDescription", saga::object::ServiceDescription)
            .value("ServiceDiscoverer", saga::object::ServiceDiscoverer)
            .value("ServiceData", saga::object::ServiceData)
            .value("CPRJobDescription", saga::object::CPRJobDescription)
            .value("CPRJobService", saga::object::CPRJobService)
            .value("CPRJob", saga::object::CPRJob)
            .value("CPRJobSelf", saga::object::CPRJobSelf)
            .value("CPRCheckpoint", saga::object::CPRCheckpoint)
            .value("CPRDirectory", saga::object::CPRDirectory)
            .value("EntityData", saga::object::EntityData)
            .value("EntityDataSet", saga::object::EntityDataSet)
            .value("EntityNavigator", saga::object::EntityNavigator)
            .export_values()
        ;
    }
}
