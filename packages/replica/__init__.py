#  Copyright (c) 2005-2009 Hartmut Kaiser
#  Copyright (c) 2010 Ole Weidner
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

# This file must be located inside a directory named SAGA/replica which in 
# turn should be located in a directory listed in your PYTHON_PATH

from _replica import *   # import logicalfile package

class directory(_replica.directory):
  pass
  
class entry(_replica.entry):
  pass
  
class flags(_replica.flags):
  pass
  