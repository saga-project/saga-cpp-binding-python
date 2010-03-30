//  Copyright (c) 2005-2009 Hartmut Kaiser
// 
//  Use, modification and distribution is subject to the Boost Software
//  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#if !defined(PYTHON_ENGINE_REGISTER_TYPES_HPP)
#define PYTHON_ENGINE_REGISTER_TYPES_HPP

#include "routine_types.hpp"

///////////////////////////////////////////////////////////////////////////////
void register_url();
void register_uuid();
void register_buffer();
void register_context();
void register_session();
void register_task();
void register_task_container();
void register_metric();
void register_version();
void register_object();
void register_exceptions();

///////////////////////////////////////////////////////////////////////////////

#endif // !defined(PYTHON_ENGINE_REGISTER_TYPES_HPP)
