## Copyright (c) 2010 Ole Weidner, Louisiana State University
##
## This is an example of how to submit an MPI job through Globus GRAM. 
## A detailed description can be found at 
## http://saga.cct.lsu.edu/software/cpp/documentation/faq

import saga

try:
  jd = saga.job.description()
  
  jd.executable = "/home/oweidner/hello-mpi"
  jd.error  = "/home/oweidner/hello-error"
  jd.output = "/home/oweidner/hello-output"
  
  # VERY IMPORTANT: Tell the backend that we want 
  # to run an MPI job. It won't work otherwise.
  jd.spmd_variation = "mpi"
  
  # Name of the PBS/LSF/... queue you want to use
  jd.queue = "checkpt"              
  
  # Number of processes you want to run your application on
  jd.number_of_processes = "32"
  
  # Your allocation/project ID (if applicable)
  jd.job_project = ["TG-XXXxxxXXX"]  

  # URL of the GRAM resource mananger / backend
  js = saga.job.service("gram://qb1.loni.org/jobmanager-pbs")

  job = js.create_job(jd)
  job.run()
  
except saga.exception, e:
  print e.get_all_messages()
  
  
  ## FYI: this is how the Globus RSL file that saga internally
  ## submits to the Globus resource manager looks like:
  ## 
  ##  &(stderr=/home/oweidner/hello-error)
  ##   (executable=/home/oweidner/hello-mpi)
  ##   (count=4)
  ##   (stdout=/home/oweidner/hello-output)
  ##   (queue=checkpt)
  ##   (jobType=mpi)
  ##   (project= "TG-XXXxxxXXX")