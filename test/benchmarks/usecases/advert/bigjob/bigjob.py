# Copyright (c) 2011 Ole Weidner (oweidner@cct.lsu.edu)
# 
# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import saga
import os.path
import unittest

###############################################################################
#
class AdvertBigjobBenchmark(unittest.TestCase):
    """
    BigJob benchmark for the advert service
    """
    def setUp(self):
        # Fixture:
        # called immediately before calling the test method
        i = 0

    def tearDown(self):
        # Fixture:
        # called immediately after the test method has been called
        i = 0
             
    ###########################################################################
    #
    def test_file_get_size(self):
        i = 0
