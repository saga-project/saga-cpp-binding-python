//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Distributed under the Boost Software License, Version 1.0. (See accompanying 
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(PYTHON_CPR_REGISTER_TYPES_HPP)
#define PYTHON_CPR_REGISTER_TYPES_HPP

#include "../../engine/routine_types.hpp"

///////////////////////////////////////////////////////////////////////////////
void register_cpr_version();
void register_cpr_checkpoint();
void register_cpr_directory();
void register_cpr_job();
void register_cpr_job_self();
void register_cpr_job_description();
void register_cpr_job_service();

///////////////////////////////////////////////////////////////////////////////

#endif
