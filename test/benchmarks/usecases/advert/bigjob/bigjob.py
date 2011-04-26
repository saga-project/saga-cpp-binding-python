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
    @classmethod
    def name(self):
        # Return the name of the benchmark
        return "advert.bigjob"
        
    @classmethod
    def description(self):
        # Return the name of the benchmark
        return "Runs a pattern that simulates the bigjob framework"
        
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
    def test_01(self):
        i = 0
