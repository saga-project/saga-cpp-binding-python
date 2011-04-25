# Copyright (c) 2011 Ole Weidner (oweidner@cct.lsu.edu)
# 
# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import sys
import unittest
from optparse import OptionParser

# import the test cases
from usecases import *

if __name__ == '__main__':

    parser = OptionParser()
    parser.add_option("-v", "--verbose",
                  action="store_true", dest="verbose", default=True,
                  help="make lots of noise [default]")
    parser.add_option("-a", "--advert-url", dest="advert_url",
                  help="url of the advert server to use", metavar="URL")

    (options, args) = parser.parse_args()

    if not options.advert_url:
        parser.error("The --advert-url is required!\n")

    suite_bigjob =    unittest.TestLoader().loadTestsFromTestCase(AdvertBigjobBenchmark)
    alltests = unittest.TestSuite([suite_bigjob])    
    result = unittest.TextTestRunner(verbosity=10).run(alltests)
    
    sys.exit(not result.wasSuccessful())
