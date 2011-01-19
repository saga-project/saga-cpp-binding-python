## Copyright (c) 2010 Ole Weidner, Louisiana State University
##
## This is an example of how to submit an OpenMP job through Globus GRAM. 
## A detailed description can be found at 
## http://saga.cct.lsu.edu/software/cpp/documentation/faq

import saga

try:
  jd = saga.job.description()
  jd.executable = "/bin/date"
  jd.arguments = ["32", "/home/oweidner/input.dat"]

  # Name of the PBS/LSF/... queue you want to use
  jd.queue = "workq"              

  # Your allocation/project ID (if applicable)
  jd.job_project = ["TG-XXXxxxXXX"]  

  # Number of processes you want to run on
  jd.number_of_processes = "32"

  # URL of the GRAM resource mananger / backend
  js = saga.job.service("gram://qb1.loni.org/jobmanager-pbs")

  job = js.create_job(jd)
  job.run()
  
except saga.exception, e:
  print e.get_all_messages()