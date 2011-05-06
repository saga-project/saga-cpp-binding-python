#!/usr/bin/env python

import sys
import getopt
import os
import saga
import uuid
import time
import pdb

database_host=""
APPLICATION_NAME="MapReduce"
machine={}
qtimes=[]

i_fsize = 0
c_size = 0
s_out = 0
e_time = 0
nbr_reduces = 0
app_url =''
app_dir=''
b_uuid = ''
nbr_maps = ''

####################################################################################
# Get UUID
####################################################################################

def get_uuid():
    wd_uuid=""
    wd_uuid += str(uuid.uuid1())
    return wd_uuid

####################################################################################
# Check the input command line arguments and assign to the global variables.
####################################################################################

def conf_check_init():
    global i_fsize , c_size , s_out, e_time, nbr_maps, nbr_reduces, app_url, app_dir, qtimes,b_uuid
    global database_host

    options, remainder = getopt.getopt(sys.argv[1:], 'a:i:c:s:q:e:r:',["help"])
    
    for opt, arg in options:
        if opt == "-a":
            database_host = arg
        if opt == "-i":
            i_fsize = int(arg) * 1024 * 1024
        elif opt == "-c":
            c_size = int(arg) * 1024 * 1024
        elif opt == "-s":
            s_out = int(arg)
        elif opt == "-q":
            del qtimes[:]
            qtimes = arg.split(":")
        elif opt == "-e":
            e_time = int(arg)
        elif opt == "-r":
            nbr_reduces = int(arg)
        elif opt == "--help":
            print " Usage : python advert_mapreduce.py <-i|-c|-s|-q|-e|-r|-h> \n"
            print " -a : advert host "
            print " -i : input file size in MB "
            print " -c : chunk size in MB" 
            print " -s : scale out factor - > number of machines"
            print " -q : queue wait time for each machine seperated by column"
            print " -e : execution time for each subjob "
            print " -r : number of reduces "
            print " -h : Help option \n"
            print " Default execution is : python advert_mapreduce.py -a advert.cct.lsu.edu -i 2048 -c 64 -s 2 -q 10:10 -e 10 -r 8 \n" 
            sys.exit(0) 
            

    nbr_maps = int(i_fsize/c_size)
    if i_fsize%c_size > 0:
        nbr_maps = nbr_maps + 1
    
    for i in range(s_out):
        machine["machine" + str(i)]=qtimes[i]
    
    print " \n"
    print " input file size     - " + str(i_fsize)
    print " chunk size          - " + str(c_size)
    print " scale out machines  - " + str(s_out)
    print " machine, qtimes     - " + str(machine)
    print " number of maps      - " + str(nbr_maps)
    print " number of reduces   - " + str(nbr_reduces)
    print " exec time of subjob - " + str(e_time)
    print " \n"
    
    

####################################################################################    
# default value intialization to the global variables.    
####################################################################################

def init():
    global i_fsize , c_size , s_out, e_time, nbr_maps, nbr_reduces, app_url, app_dir, qtimes,b_uuid
    # input file size
    i_fsize = 2 * 1024 * 1024 * 1024
    # number of chunks
    c_size = 64 * 1024 * 1024
    s_out = 2
    e_time = 10
    nbr_reduces = 8
    qtimes.append(10)
    qtimes.append(10)
    

####################################################################################
# Create Advert entry for the bigjob 
####################################################################################

def start_bigjob():
    global i_fsize , c_size , s_out, e_time, nbr_maps, nbr_reduces, app_url, app_dir, qtimes,b_uuid
    global database_host
    
    print " init advert session at : " + database_host
    b_uuid = get_uuid()
    app_url = saga.url("advert://" + database_host + "/"+APPLICATION_NAME + "-" + str(b_uuid) + "/")
    app_dir = saga.advert.directory(app_url, saga.advert.Create | saga.advert.CreateParents | saga.advert.ReadWrite)
    print " created application directory " + str(app_url)

####################################################################################
# Create Advert entries for the machines mentioned as the scale out factor
# The status of each machine directory is Unknown for the queue time provided for that
# machine
####################################################################################

def m_dirs():
    global i_fsize , c_size , s_out, e_time, nbr_maps, nbr_reduces, app_url, app_dir, qtimes,b_uuid
    global database_host
    
    for m,q in machine.iteritems():    
        pilot_url = saga.url("advert://" + database_host + "/"+APPLICATION_NAME + "-" + str(b_uuid) + "/" + m + "/" )
        pilot_dir = saga.advert.directory(pilot_url, saga.advert.Create | saga.advert.CreateParents | saga.advert.ReadWrite)
        pilot_dir.set_attribute("state", "Unknown")
        print " created pilot directory " + str(app_url) + " with state Unknown"
    
        new_url = saga.url("advert://" + database_host + "/"+APPLICATION_NAME + "-" + str(b_uuid) + "/" + m + "/" + "new")
        new_dir = saga.advert.directory(new_url, saga.advert.Create | saga.advert.CreateParents | saga.advert.ReadWrite)
        print " created  new directory " + str(new_url) 
    for i in range(int(q)):
        pilot_dir.set_attribute("state", "Unknown")
    pilot_dir.set_attribute("state", "Running")


####################################################################################
# Create Advert entries for the map subjobs. Each mapsubjob is distributed
# among all the machines.
####################################################################################

def map_subjobs():         
    print " \n************************* Map Phase Started ****************************\n "

    global i_fsize , c_size , s_out, e_time, nbr_maps, nbr_reduces, app_url, app_dir, qtimes,b_uuid
    global database_host
    
    jobs=[]
    job_urls=[]    
    k=0
    
    for i in range(nbr_maps):
        j_uuid = get_uuid()
        job_url = saga.url("advert://" + database_host + "/"+APPLICATION_NAME + "-" + str(b_uuid) + "/machine" + str(k) + "/" + j_uuid )
        k = k + 1
        if k >= s_out:
            k = 0
            
        job_dir = saga.advert.directory(job_url, saga.advert.Create | saga.advert.CreateParents | saga.advert.ReadWrite)
        print " created Job directory " + str (job_url) 
        job_dir.set_attribute("state","Unknown")
        job_dir.set_attribute("WorkingDirectory","/home/Mr_Working_directory")
        job_dir.set_attribute("SPMDVariation","single/mpi")
        job_dir.set_attribute("NumberOfProcesses","1")
        job_dir.set_attribute("Executable","map_script")
        job_dir.set_attribute("Error","Map_error_file_name")
        job_dir.set_attribute("Arguments","1,2")
        print str(job_url) + "State - Unknown"
        time.sleep(1) 
        job_dir.set_attribute("state","New")
        print str(job_url) + "State - New"
        time.sleep(1)
        job_dir.set_attribute("state","Running")
        print str(job_url) + "State - Running"
        jobs.append(job_dir)
        job_urls.append(job_url)
        
    exec_job=[0] * len(jobs)
    count = 0
    while 1:
        i = 0
        for job in jobs:
            job.get_attribute("state")
            if exec_job[i] == e_time:
                job.set_attribute("state","Done")
                count = count + 1
            exec_job[i] = exec_job[i] + 1
            i = i + 1            
  	    print "{" + str(count) + " jobs Done ," + str(len(jobs)-count) + "jobs Running" + "}"
        if count == len(jobs):
            break
            
    print " \n************************* Map Phase completed ****************************\n "


####################################################################################
# Create Advert entries for the map subjobs. Each mapsubjob is distributed
# among all the machines.
####################################################################################

def reduce_subjobs():         

    print " \n************************* Reduce Phase Started ****************************\n "
    
    global i_fsize , c_size , s_out, e_time, nbr_maps, nbr_reduces, app_url, app_dir, qtimes,b_uuid
    global database_host
    
    jobs=[]
    job_urls=[]    
    k=0
    
    for i in range(nbr_reduces):
        j_uuid = get_uuid()
        job_url = saga.url("advert://" + database_host + "/"+APPLICATION_NAME + "-" + str(b_uuid) + "/machine" + str(k) + "/" + j_uuid )
        k = k + 1
        if k >= s_out:
            k = 0
            
        job_dir = saga.advert.directory(job_url, saga.advert.Create | saga.advert.CreateParents | saga.advert.ReadWrite)
        print " created Job directory " + str (job_url)
        job_dir.set_attribute("state","Unknown")
        job_dir.set_attribute("WorkingDirectory","/home/Mr_Working_directory")
        job_dir.set_attribute("SPMDVariation","single/mpi")
        job_dir.set_attribute("NumberOfProcesses","1")
        job_dir.set_attribute("Executable","reduce_script")
        job_dir.set_attribute("Error","reduce_Error_file_name")
        job_dir.set_attribute("Arguments","1")
        print str(job_url) + "State - Unknown"
        time.sleep(1) 
        job_dir.set_attribute("state","New")
        print str(job_url) + "State - New"
        time.sleep(1)
        job_dir.set_attribute("state","Running")
        print str(job_url) + "State - Running"
        jobs.append(job_dir)
        job_urls.append(job_url)
        
    exec_job=[0] * len(jobs)
    count = 0
    while 1:
        i = 0
        for job in jobs:
            job.get_attribute("state")
            if exec_job[i] == e_time:
                job.set_attribute("state","Done")
                count = count + 1
            exec_job[i] = exec_job[i] + 1
            i = i + 1            
  	    print "{" + str(count) + " jobs Done ," + str(len(jobs)-count) + "jobs Running" + "}"
        if count == len(jobs):
            break

    print " \n************************* Reduce Phase Completed ****************************\n "

def close_bigjob():
    global app_url    
    print " Close directory - " + str(app_url)
    app_dir.remove(app_url,saga.name_space.Recursive)        


def main():
    print " **************************  Advert pattern for Map Reduce Framework Started *************************\n "
    init() 
    conf_check_init()
    start_bigjob()
    m_dirs()
    map_subjobs()
    reduce_subjobs()
    close_bigjob()
    print " **************************  Advert pattern for Map Reduce Framework Stopped ************************* "
    sys.exit(0)
    

if __name__=="__main__":
    main()
    
