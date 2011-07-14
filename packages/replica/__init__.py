#  Copyright (c) 2005-2009 Hartmut Kaiser
#  Copyright (c) 2010 Ole Weidner
# 
#  Use, modification and distribution is subject to the Boost Software
#  License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
#  http://www.boost.org/LICENSE_1_0.txt)

# This file must be located inside a directory named SAGA/replica which in 
# turn should be located in a directory listed in your PYTHON_PATH

"""
The SAGA Replica API package. 

The SAGA replica API is used to interact with local and remote file replica
catalogs. A replica catalog maintains a list of physical location for a
logical file.

More on the concepts of the SAGA API can be found here:
U{http://www.gridforum.org/documents/GFD.90.pdf}
"""

from _replica import *   # import logicalfile package

class directory(_replica.directory):
  pass
  
class entry(_replica.entry):
  pass
  
class flags(_replica.flags):
  pass
  