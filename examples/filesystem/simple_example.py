## Copyright (c) 2011 Ole Weidner, Louisiana State University
##
## This is an example of ...

import saga

try:
  # create a new empty file
  f_orig = saga.filesystem.file("/tmp/hello.txt", saga.filesystem.Create)
  
  # write the string "Hello, World!" to the file
  f_orig.write("Hello, World!\n")
  
  # copy the file 
  f_orig.copy("/tmp/hello_copy.txt")
  
  # open the copy and read the content
  f_copy = saga.filesystem.file("/tmp/hello_copy.txt")
  print f_copy.read()
  
  # delete both files
  f_orig.remove()
  f_copy.remove()

except saga.exception, e:
  print "ERROR: " 
  print e.get_all_messages()