//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(PYTHON_JOB_REGISTER_TYPES_HPP)
#define PYTHON_JOB_REGISTER_TYPES_HPP

#include "../../engine/routine_types.hpp"

///////////////////////////////////////////////////////////////////////////////
void register_job_version();
void register_job();            
void register_job_self();       
void register_job_service();    
void register_job_description();

///////////////////////////////////////////////////////////////////////////////
boost::python::object py_get_outstream(saga::job::istream const& strm, char const* name);
boost::python::object py_get_instream(saga::job::ostream const& strm, char const* name);

#endif // !defined(PYTHON_JOB_REGISTER_TYPES_HPP)
