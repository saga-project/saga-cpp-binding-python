# Copyright (c) 2011 Ole Weidner (oweidner@cct.lsu.edu)
# 
# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import saga
import os.path
import unittest

###############################################################################

class AdvertBigjobBenchmark(unittest.TestCase):
    """
    DESCRIPTION
    """
    @classmethod
    def name(self):
        # Return the name of the benchmark
        return "advert.bigjob"
        
    @classmethod
    def description(self):
        # Return the name of the benchmark
        return "Benchmark that simulates the 'SAGA Big-Job' advert service access pattern."
        
    def setUp(self):
        # Fixture:
        # called immediately before calling the test method
        nop = True

    def tearDown(self):
        # Fixture:
        # called immediately after the test method has been called
        nop = True
             
    ###########################################################################
    #
    def test_benchmark(self):
        print "Running benchmark..."
        i = 0
