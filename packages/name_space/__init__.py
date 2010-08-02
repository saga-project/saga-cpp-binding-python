#  Copyright (c) 2005-2009 Hartmut Kaiser
#  Copyright (c) 2010 Ole Weidner
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# This file must be located inside a directory named SAGA/name_space which in 
# turn should be located in a directory listed in your PYTHON_PATH

from _name_space import *     # import name_space package

class directory(_name_space.directory):
  pass
  
class entry(_name_space.entry):
  pass
  
class flags(_name_space.flags):
  pass