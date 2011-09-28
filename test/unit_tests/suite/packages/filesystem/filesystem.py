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
class FilePackageTests(unittest.TestCase):
    """
    Tests for the saga.filesystem API package 
    http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html    
    """
    def setUp(self):
        # Fixture:
        # called immediately before calling the test method
        self.filename_a = "/tmp/test_a.dat"
        self.filename_b = "/tmp/test_b.dat"
        self.filename_c = "/tmp/test_c.dat"
        self.filename_d = "/tmp/test_d.dat"
        self.filename_e = "/tmp/test_e.dat"

        if os.path.exists(self.filename_a):
            os.remove(self.filename_a)
        if os.path.exists(self.filename_b):
            os.remove(self.filename_b)
        if os.path.exists(self.filename_c):
           os.remove(self.filename_c)
        if os.path.exists(self.filename_d):
           os.remove(self.filename_d)
        if os.path.exists(self.filename_e):
           os.remove(self.filename_e)
                      
        self.FILE_A = open(self.filename_a,"w")
        self.FILE_A.write("SAGA PYTHON TEST") # 16 bytes

    def tearDown(self):
        # Fixture:
        # called immediately after the test method has been called
        self.FILE_A.close()
        if os.path.exists(self.filename_a):
            os.remove(self.filename_a)
        if os.path.exists(self.filename_b):
            os.remove(self.filename_b)
        if os.path.exists(self.filename_c):
           os.remove(self.filename_c)
        if os.path.exists(self.filename_d):
           os.remove(self.filename_d)
        if os.path.exists(self.filename_e):
           os.remove(self.filename_e)
           
    #######################################
    # native saga.filesystem.file methods tests #
    #######################################
             
    ###########################################################################
    #
    def test_file_get_size(self):
        """
        Test for the saga.filesystem.get_size() API call 
        http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-get_size
        """
        try:
            source_url = saga.url("file:///"+self.filename_a)
    
            my_file = saga.filesystem.file(source_url)
            s = my_file.get_size()
            my_file.close()
    
        except saga.exception, e: 
            self.fail(e.get_full_message())
                
        self.failUnless(s == os.path.getsize(self.filename_a))
        
    # TODO: def test_file_read(self):
    # TODO: def test_file_seek(self):
    # TODO: def test_file_write(self):

    ############################################
    # inherited methods tests: namespace.entry #
    ############################################

    ###########################################################################
    #
    #def test_file_get_group(self):
    #    """
    #    Test for the saga.filesystem.copy() API call 
    #    http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-copy
    #    """
    #    try:
    #        source_url = saga.url("file:///"+self.filename_a)
    #
    #        my_file = saga.filesystem.file(source_url)
    #        g = my_file.get_group()
    #        my_file.close()
    #
    #    except saga.exception, e: 
    #        self.fail(e.get_full_message())


    ###########################################################################
    #
    #def test_file_get_owner(self):
    #    """
    #    Test for the saga.filesystem.copy() API call 
    #    http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-copy
    #    """
    #    try:
    #        source_url = saga.url("file:///"+self.filename_a)
    #
    #        my_file = saga.filesystem.file(source_url)
    #        o = my_file.get_owner()
    #        my_file.close()
    #
    #    except saga.exception, e: 
    #        self.fail(e.get_full_message())
            
                                  
    ###########################################################################
    #
    def test_file_copy(self):
        """
        Test for the saga.filesystem.copy() API call 
        http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-copy
        """
        try:
            source_url = saga.url("file:///"+self.filename_a)
            target_url = saga.url("file:///"+self.filename_b)

            my_file = saga.filesystem.file(source_url)
            my_file.copy(target_url)
            my_file.close()

        except saga.exception, e: 
            self.fail(e.get_full_message())
            
        self.failUnless(os.path.exists(self.filename_b))
        
    ###########################################################################
    #
    def test_file_move(self):
        """
        Test for the saga.filesystem.move() API call 
        http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-move
        """
        try:
            source_url = saga.url("file:///"+self.filename_a)
            target_url = saga.url("file:///"+self.filename_d)
    
            my_file = saga.filesystem.file(source_url)
            my_file.move(target_url)
            my_file.close()
                
        except saga.exception, e: 
            self.fail(e.get_full_message())

        self.failUnless(os.path.exists(self.filename_d))

    ###########################################################################
    #
    def test_file_remove(self):
        """
        Test for the saga.filesystem.remove() API call 
        http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-remove
        """
        try:
            FILE = open(self.filename_e,"w") # creates a new file
            FILE.close()
            
            source_url = saga.url("file:///"+self.filename_e)
    
            my_file = saga.filesystem.file(source_url)
            my_file.remove()
            my_file.close()
                
        except saga.exception, e: 
            self.fail(e.get_full_message())

        self.failUnless(not os.path.exists(self.filename_d))



    ###########################################################################
    #
    def test_file_get_cwd(self):
        """
        Test for the saga.filesystem.get_cwd() API call 
        http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-get_cwd
        """
        try:
            source_url = saga.url("file:///"+self.filename_a)
    
            my_file = saga.filesystem.file(source_url)
            s = my_file.get_cwd()
    
        except saga.exception, e: 
            self.fail(e.get_full_message())
            
        self.failUnless(s.path == os.path.dirname(self.filename_a))

    ###########################################################################
    #
    def test_file_get_name(self):
        """
        Test for the saga.filesystem.get_name() API call 
        http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-get_name
        """
        try:
            source_url = saga.url("file:///"+self.filename_a)
    
            my_file = saga.filesystem.file(source_url)
            s = my_file.get_name()
    
        except saga.exception, e: 
            self.fail(e.get_full_message())
            
        self.failUnless(s.get_string() == os.path.split(self.filename_a)[1])

    ###########################################################################
    #
    def test_file_get_url(self):
        """
        Test for the saga.filesystem.get_cwd() API call 
        http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-get_cwd
        """
        try:
            source_url = saga.url("file:///"+self.filename_a)
    
            my_file = saga.filesystem.file(source_url)
            s = my_file.get_url()
                
        except saga.exception, e: 
            self.fail(e.get_full_message())
            
        self.failUnless(s.path == self.filename_a)

    ###########################################################################
    #
    def test_file_is_dir(self):
        """
        Test for the saga.filesystem.is_dir() API call 
        http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-is_dir
        """
        try:
            source_url = saga.url("file:///"+self.filename_a)
    
            my_file = saga.filesystem.file(source_url)
            is_dir = my_file.is_dir()
                
        except saga.exception, e: 
            self.fail(e.get_full_message())

        self.failUnless(is_dir == False)        
        
    ###########################################################################
    #
    def test_file_is_entry(self):
        """
        Test for the saga.filesystem.entry() API call 
        http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-is_entry
        """
        try:
            source_url = saga.url("file:///"+self.filename_a)
    
            my_file = saga.filesystem.file(source_url)
            is_entry = my_file.is_entry()
                
        except saga.exception, e: 
            self.fail(e.get_full_message())

        self.failUnless(is_entry == True)              

    ###########################################################################
    #
    def test_file_link(self):
        """
        Test for the saga.filesystem.link() and file.is_link() API calls 
        http://saga.cct.lsu.edu/python/apidoc/saga.filesystem._file.html#file-link
        """
        try:
            source_url = saga.url("file:///"+self.filename_a)
            link_url = saga.url("file:///"+self.filename_c)

            my_file = saga.filesystem.file(source_url, saga.filesystem.Create)
            my_file.link(link_url)
            my_file2 = saga.filesystem.file(link_url) # throws if file doesn't exist
            is_link = my_file2.is_link()
            
            os.unlink(self.filename_c)
                
        except saga.exception, e: 
            self.fail(e.get_full_message()) 

        self.failUnless(is_link == True)
        

        
        
        
        
        
        
        
