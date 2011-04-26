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

    #### Create list of benchmarks
    ##
    all_benchmarks = {}
    all_benchmarks[AdvertBigjobBenchmark.name()] = AdvertBigjobBenchmark
    all_benchmarks[AdvertSimpleBenchmark.name()] = AdvertSimpleBenchmark
    # ... add yours
    ##
    ####

    parser = OptionParser()
    
    parser.add_option("-l", "--list",
                  action="store_true", dest="list", default=False,
                  help="list all available benchmarks")

    parser.add_option("-b", "--benchmarks",
                      dest="benchmarks", metavar="LIST",
                      help="comma separated list of benchmarks to run [required]")
 
    (options, args) = parser.parse_args()

    if options.list:
        print("Available benchmarks:\n")
        for k, v in all_benchmarks.iteritems():
          print ("  " + k + " (" + v.description() + ")")
        print(" ")
        sys.exit(True)

    if not options.benchmarks:
        parser.error("The --benchmarks option is required!\n")
        sys.exit(False)
    else:
        suites = []
    
        benchmark_list = (options.benchmarks.replace(' ','')).split(',')
        for bm in benchmark_list:
          try:
            print "Loading benchmark: " + repr(all_benchmarks[bm])
            suites += unittest.TestLoader().loadTestsFromTestCase(all_benchmarks[bm])
            
          except KeyError:
            print "ERROR: A benchmark with the name '" + bm + "' doesn't exist! Use -l for a list of all available benchmarks."
            print ""  
            sys.exit(False)

    alltests = unittest.TestSuite(suites)    
    result = unittest.TextTestRunner(verbosity=10).run(alltests)
    
    sys.exit(not result.wasSuccessful())
