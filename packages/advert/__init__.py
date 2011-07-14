#  Copyright (c) 2005-2009 Hartmut Kaiser
#  Copyright (c) 2010 Ole Weidner
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# XXX This file must be located inside a directory named SAGA/advert which in turn 
# should be located in a directory listed in your PYTHON_PATH

from _advert import *         # import advert package

class directory(_advert.directory):
"""
An advert directory. It can contain other advert directories or entries.
"""
  pass
  
class entry(_advert.entry):
  pass
"""
An advert entry.
"""
  
class flags(_advert.flags):
"""
Flags that can be used with the saga advert package. 
"""
  pass