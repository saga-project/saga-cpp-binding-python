# Copyright (c) 2008 Ole Weidner (oweidner@cct.lsu.edu)
# 
# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import saga
import os.path
import unittest

###############################################################################
#
class JobTests(unittest.TestCase):
    """
    Tests for the saga.job API package
    """
    def setUp(self):
        # Fixture:
        # called immediately before calling the test method
        self.filename_a = "/tmp/aloha.dat"
        if os.path.exists(self.filename_a):
            os.remove(self.filename_a)
                    

    def tearDown(self):
        nop = True
        # Fixture:
        # called immediately after the test method has been called
        if os.path.exists(self.filename_a):
            os.remove(self.filename_a)


    ###########################################################################
    #
    def test_attribute_if(self):
        """
        Tests for the saga.job.description attribute interface 
        """
        
        try:
            self.failUnless((saga.task.New == 1) and (saga.task.New == saga.job.New))
            self.failUnless((saga.task.Running == 2) and (saga.task.Running == saga.job.Running))
            self.failUnless((saga.task.Done == 3) and (saga.task.Done == saga.job.Done))
            self.failUnless((saga.task.Canceled == 4) and (saga.task.Canceled == saga.job.Canceled))
            self.failUnless((saga.task.Failed == 5) and (saga.task.Failed == saga.job.Failed))
            self.failUnless((saga.job.Suspended == 6))

            
        except saga.exception, e: 
            self.fail(e.get_full_message())
             
    ###########################################################################
    #
    def test_job_service(self):
        """
        Test for the saga.job.create_job() API call 
        """
        
        #print(os.path.exists(self.filename_a))
        
        try:
             js_url = saga.url("fork://localhost/")
             job_service = saga.job.service(js_url)

             job_desc = saga.job.description()
             job_desc.executable = "touch"
             job_desc.arguments = ["-a", self.filename_a]

             my_job = job_service.create_job(job_desc)
             my_job.run()
            
        except saga.exception, e: 
            self.fail(e.get_full_message())
                
    ###########################################################################
    #
    def test_job_ioredirection(self):
        """
        Test for the saga.job.service.run_job() API call 
        """

        date = ""        
        try:
             js_url = saga.url("fork://localhost/")
             job_service = saga.job.service(js_url)

             (job, sin, sout, serr) = job_service.run_job("/bin/echo -n HELLO SAGA", "fork://localhost")
             output = sout.read()
             if(output != "HELLO SAGA"):
                if(output == None):
                    self.fail("Unexpected job output: 'None'")
                else:
                    self.fail("Unexpected job output: " + output)
            
        except saga.exception, e: 
            self.fail(e.get_full_message())
                
