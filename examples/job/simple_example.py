## Copyright (c) 2010 Ole Weidner, Louisiana State University
##
## This is an example of how to submit an MPI job through Globus GRAM. 
## A detailed description can be found at 
## http://saga.cct.lsu.edu/software/cpp/documentation/faq

import saga

try:
  jd = saga.job.description()
  
  jd.executable = "/bin/hostname"
  jd.output     = "stdout.log"
  
  js = saga.job.service("fork://localhost");

  job = js.create_job(jd)
  job.run()
  job.wait(-1)
  
except saga.exception, e:
  print e.get_all_messages()
  
