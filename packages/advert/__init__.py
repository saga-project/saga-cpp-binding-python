#  Copyright (c) 2005-2009 Hartmut Kaiser
#  Copyright (c) 2010 Ole Weidner
# 
#  Distributed under the Boost Software License, Version 1.0. (See accompanying
#  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

# This file must be located inside a directory named SAGA/advert which in turn 
# should be located in a directory listed in your PYTHON_PATH

"""
The SAGA Advert Service API package. 

The SAGA advert service defines a hierarchical structure - similar to a filesystem -
that can be used to store key-value-based informations. It is often used 
to store (distributed) application data in a SAGA-based application. 

More on the concepts of the SAGA Advert Service can be found here:
U{http://www.ogf.org/OGF27/materials/1767/saga_adverts.pdf}
"""

from _advert import *         # import advert package

class directory(_advert.directory):
  pass
  
class entry(_advert.entry):
  pass
  
class flags(_advert.flags):
  pass