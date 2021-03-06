#  Copyright (c) 2005-2009 Hartmut Kaiser 
#  Copyright (c) 2010 Ole Weidner
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# This file must be located inside a directory named SAGA/file which in turn 
# should be located in a directory listed in your PYTHON_PATH

"""
The SAGA Filesystem API package. 

The SAGA filesystem API is used to operate on local and remote filesystems. 

More on the concepts of the SAGA API can be found here:
U{http://www.gridforum.org/documents/GFD.90.pdf}
"""

from _filesystem import *           # import file package

class directory(_filesystem.directory):
  pass
  
class file(_filesystem.file):
  pass
  
class flags(_filesystem.flags):
  pass