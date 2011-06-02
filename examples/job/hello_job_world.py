## Copyright (c) 2011 Ole Weidner, Louisiana State University
##
##  This is part of the code examples on the SAGA website:
##  http://saga.cct.lsu.edu/documentation/writing_applications/hello-world

import saga

try:
  # create an "echo 'hello, world' job"
  jd = saga.job.description()
  jd.set_attribute("Executable", "/bin/echo")
  jd.set_vector_attribute("Arguments", ["Hello, World!"])
  jd.set_attribute("Interactive", "True")

  
  # connect to the local job service
  js = saga.job.service("fork://localhost");

  # submit the job
  job = js.create_job(jd)
  job.run()

  # wait for the job to complete
  job.wait(-1)
  
  # print the job's output
  output = job.get_stdout()
  print output.read()
  
except saga.exception, e:
  for err in e.get_all_messages():
    print err
