## Copyright (c) 2011 Ole Weidner, Louisiana State University
##
## This is an example of how to submit an MPI job through Globus GRAM. 
## A detailed description can be found at 
## http://saga.cct.lsu.edu/software/cpp/documentation/faq

import saga

try:
  # create an "echo 'hello, world' job"
  jd = saga.job.description()
  jd.executable = "/bin/echo"
  jd.arguments  = ["Hello, World!"]
  jd.interactive  = True

  
  # connect to the local job service
  js = saga.job.service("fork://localhost");

  # submit the job
  job = js.create_job(jd)
  job.run()

  # wait for the job to complete
  job.wait(-1)
  
  # print the job's output
  print job.get_stdout()
  
except saga.exception, e:
  print "ERROR: " 
  for err in e.get_all_messages():
    print err
