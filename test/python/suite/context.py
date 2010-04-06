# Copyright (c) 2008 Ole Weidner (oweidner@cct.lsu.edu)
# 
# Use, modification and distribution is subject to the Boost Software
# License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
# http://www.boost.org/LICENSE_1_0.txt)

import saga
import unittest

###############################################################################
#
class ContextTests(unittest.TestCase):
    """
    Tests for the saga.context API  
    http://saga.cct.lsu.edu/python/apidoc/saga.context.html  
    """
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
    def test_context_and_session(self):
        """
        Test for the saga.file.get_size() API call 
        http://saga.cct.lsu.edu/python/apidoc/saga.file._file.html#file-get_size
        """
        try:
            ctx1 = saga.context(saga.attributes.context_server)
            ctx2 = saga.context(saga.attributes.SSH)

            s = saga.session()
            s.add_context(ctx1)
            s.add_context(ctx2)
            l = s.list_contexts()
    
            print len(l)
            print l[0].type
            print l[1].type

            print ctx1.get_ctype()
            print ctx2.get_ctype()
    
        except saga.exception, e: 
            self.fail(e)
                
        #self.failUnless(s == os.path.getsize(self.filename_a))
        
