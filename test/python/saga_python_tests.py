# Copyright (c) 2008 Ole Weidner (oweidner@cct.lsu.edu)
# 
# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import sys
import unittest

# import the test cases
from suite import *

if __name__ == '__main__':
    suite_file =    unittest.TestLoader().loadTestsFromTestCase(FilePackageTests)
#    suite_context = unittest.TestLoader().loadTestsFromTestCase(ContextTests)
    suite_job =     unittest.TestLoader().loadTestsFromTestCase(JobTests)

    alltests = unittest.TestSuite([suite_file, 
                                   #suite_context,
                                   suite_job])
    
    result = unittest.TextTestRunner(verbosity=10).run(alltests)
    
    sys.exit(not result.wasSuccessful())
