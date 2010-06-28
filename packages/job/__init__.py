#  Copyright (c) 2005-2009 Hartmut Kaiser
#  Copyright (c) 2010 Ole Weidner
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# This file must be located inside a directory named SAGA/job which in turn 
# should be located in a directory listed in your PYTHON_PATH

from _job import *            # import job package

class description(_job.description):
  pass
  
class job(_job.job):
  pass
  
class job_state(_job.job_state):
  pass
  
class self(_job.self):
  pass

class service(_job.service):
  pass
  