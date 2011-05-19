#!/usr/bin/env python

import sys
import getopt
import saga
import time
import pdb
import socket
import os
import traceback
import logging
import subprocess
import multiprocessing
import socket
import threading
import signal
import ConfigParser
# import other BigJob packages
# import API
#import api.base
import uuid



# configurationg
advert_host = "advert.cct.lsu.edu"
BIGJOB_HOME= os.getcwd() + "/../"
       

class bigjob_agent:
    
    """BigJob Agent:
       - reads new job information from advert service
       - starts new jobs
       - monitors running jobs """
   
    """Constructor"""
    def __init__(self, args1, args2):
        
        self.database_host = args1
        # objects to store running jobs and processes
        self.jobs = []
        self.processes = {}
        self.freenodes = []
        self.busynodes = []
        self.restarted = {}

        self.CPR = "False"
        self.SHELL= "/bin/bash"
        self.MPIRUN= "mpirun"
        ##print "cpr: " + self.CPR + " mpi: " + self.MPIRUN + " shell: " + self.SHELL
        
        # init cpr monitoring
        self.init_cpr()
        # init rms (SGE/PBS)
        self.init_rms()

        self.failed_polls = 0
        # open advert service base url
        hostname = socket.gethostname()

        self.base_url =  args2
        #print "Open advert: " + self.base_url
        try:
            self.base_dir = saga.advert.directory(saga.url(self.base_url), saga.advert.Create | saga.advert.ReadWrite)
            self.new_job_dir = saga.advert.directory(saga.url(self.base_url+"/new/"), saga.advert.Create| saga.advert.ReadWrite)
        except:
            #print "No advert entry found at specified url: " + self.base_url
            traceback.print_exc(file=sys.stderr)
            

        # update state of glidin job to running
        self.update_glidin_state()
        # start background thread for polling new jobs and monitoring current jobs
        self.launcher_thread=threading.Thread(target=self.start_background_thread())
        self.launcher_thread.start()
        
    def update_glidin_state(self):     
        #print "update state of glidin job to: " + str(saga.job.Running)
        return self.base_dir.set_attribute("state", str(saga.job.Running))
    
    def init_rms(self):
        if(os.environ.get("PBS_NODEFILE")!=None):
            return self.init_pbs()
        elif(os.environ.get("PE_HOSTFILE")!=None):
            return self.init_sge()
        else:
            return self.init_local()
        return None

    def init_local(self):
        """ initialize free nodes list with dummy (for fork jobs)"""
        try:
            num_cpus = self.get_num_cpus()
            for i in range(0, num_cpus): 
                self.freenodes.append("localhost\n")
        except IOError:
            self.freenodes=["localhost\n"]

    def init_sge(self):
        """ initialize free nodes list from SGE environment """
        sge_node_file = os.environ.get("PE_HOSTFILE")    
        if sge_node_file == None:
                return
        f = open(sge_node_file)
        sgenodes = f.readlines()
        f.close()
        for i in sgenodes:    
        
            columns = i.split()                
            try:
                for j in range(0, int(columns[1])):
                    #print "add host: " + columns[0]
                    self.freenodes.append(columns[0]+"\n")
            except:
                    pass
        return self.freenodes            

    def init_pbs(self):
        """ initialize free nodes list from PBS environment """
        pbs_node_file = os.environ.get("PBS_NODEFILE")    
        if pbs_node_file == None:
            return
        f = open(pbs_node_file)
        self.freenodes = f.readlines()
        f.close()

        # check whether pbs node file contains the correct number of nodes
        num_cpus = self.get_num_cpus()
        node_dict={}
        for i in set(self.freenodes):
           node_dict[i] = self.freenodes.count(i)
           if node_dict[i] < num_cpus:
                node_dict[i] = num_cpus
    
        self.freenodes=[]
        for i in node_dict.keys():
            #print "host: " + i + " nodes: " + str(node_dict[i])
            for j in range(0, node_dict[i]):
                #print "add host: " + i
                self.freenodes.append(i)

    def get_num_cpus(self):
        cpuinfo = open("/proc/cpuinfo", "r")
        cpus = cpuinfo.readlines()
        cpuinfo.close()
        num = 0
        for i in cpus:
                if i.startswith("processor"):
                        num = num+1
        return num
        
        
    def print_attributes(self, advert_directory):
        """ for debugging purposes 
        #print attributes of advert directory """
        
        #print "Job: "+advert_directory.get_url().get_string()+ " State: " + advert_directory.get_attribute("state")

        #attributes = advert_directory.list_attributes()                
        #for i in attributes:
        #    if (advert_directory.attribute_is_vector(i)==False):
        #        #print "attribute: " + str(i) +  " value: " + advert_directory.get_attribute(i)
        #    else:
        #        #print "attribute: " + str(i)
        #        vector = advert_directory.get_vector_attribute(i) 
        #        for j in vector:
        #            #print j
     
    def execute_job(self, job_dir):
        """ obtain job attributes from advert and execute process """
        state=None
        try:
            state = job_dir.get_attribute("state")
        except:
            pass
            #print "Could not access job state... skip execution attempt"
        if(state==str(saga.job.Unknown) or
            state==str(saga.job.New)):
            try: 
                job_dir.set_attribute("state", str(saga.job.New))
                self.print_attributes(job_dir)        
                numberofprocesses = "1"
                if (job_dir.attribute_exists("NumberOfProcesses") == True):
                    numberofprocesses = job_dir.get_attribute("NumberOfProcesses")
                
                spmdvariation="single"
                if (job_dir.attribute_exists("SPMDVariation") == True):
                    spmdvariation = job_dir.get_attribute("SPMDVariation")
                
                arguments = ""
                if (job_dir.attribute_exists("Arguments") == True):
                    for i in job_dir.get_vector_attribute("Arguments"):
                        arguments = arguments + " " + i
                        
                environment = os.environ
                if (job_dir.attribute_exists("Environment") == True):
                    for i in job_dir.get_vector_attribute("Environment"):
                        env = i.split("=")
                        environment[env[0]]=env[1]                        
                 
                executable = job_dir.get_attribute("Executable")
                
                workingdirectory = os.getcwd() 
                if (job_dir.attribute_exists("WorkingDirectory") == True):
                        workingdirectory =  job_dir.get_attribute("WorkingDirectory")
                
                output="stdout"
                if (job_dir.attribute_exists("Output") == True):
                        output = job_dir.get_attribute("Output")
                        
                error="stderr"
                if (job_dir.attribute_exists("Error") == True):
                       error = job_dir.get_attribute("Error")
               
                # append job to job list
                self.jobs.append(job_dir)
                
                # create stdout/stderr file descriptors
                output_file = os.path.abspath(output)
                error_file = os.path.abspath(error)
                #print "stdout: " + output_file + " stderr: " + error_file + " env: " + str(environment)
                stdout = open(output_file, "w")
                stderr = open(error_file, "w")
                command = executable + " " + arguments
                
                # special setup for MPI NAMD jobs
                machinefile = self.allocate_nodes(job_dir)
                host = "localhost"
                try:
                    machine_file_handler = open(machinefile, "r")
                    node= machine_file_handler.readlines()
                    machine_file_handler.close()
                    host = node[0].strip()
                except:
                    pass


                if(machinefile==None):
                    #print "Not enough resources to run: " + job_dir.get_url().get_string() 
                    return # job cannot be run at the moment

                # start application process
                if (spmdvariation.lower( )=="mpi"):
                     command = "cd " + workingdirectory + "; " + self.MPIRUN + " -np " + numberofprocesses + " -machinefile " + machinefile + " " + command
                     #if (host != socket.gethostname()):
                     #    command ="ssh  " + host + " \"cd " + workingdirectory + "; " + command +"\""     
                else:
                    command ="ssh  " + host + " \"cd " + workingdirectory + "; " + command +"\""     
                shell = self.SHELL 
                #print "execute: " + command + " in " + workingdirectory + " from: " + str(socket.gethostname()) + " (Shell: " + shell +")"
                # bash works fine for launching on QB but fails for Abe :-(
                p = subprocess.Popen(args=command, executable=shell, stderr=stderr,
                                     stdout=stdout, cwd=workingdirectory, 
                                     env=environment, shell=True)
                #print "started " + command
                self.processes[job_dir] = p
                job_dir.set_attribute("state", str(saga.job.Running))
            except:
                traceback.print_exc(file=sys.stderr)
            
    def allocate_nodes(self, job_dir):
        """ allocate nodes
            allocated nodes will be written to machinefile advert-launcher-machines-<jobid>
            this method is only call by background thread and thus not threadsafe"""
        number_nodes = int(job_dir.get_attribute("NumberOfProcesses"))
        nodes = []
        if (len(self.freenodes)>=number_nodes):
            unique_nodes=set(self.freenodes)
            for i in unique_nodes:
                number = self.freenodes.count(i)
                #print "allocate: " + i + " number nodes: " + str(number) + " current busy nodes: " + str(self.busynodes) + " free nodes: " + str(self.freenodes)
                for j in range(0, number):
                    if(number_nodes > 0):
                        nodes.append(i)
                        self.freenodes.remove(i)                
                        self.busynodes.append(i)
                        number_nodes = number_nodes - 1
                    else:
                        break

            machine_file_name = self.get_machine_file_name(job_dir)
            machine_file = open(machine_file_name, "w")
            #machine_file.writelines(self.freenodes[:number_nodes])
            machine_file.writelines(nodes)
            machine_file.close() 
            #print "wrote machinefile: " + machine_file_name + " Nodes: " + str(nodes)
            # update node structures
            #self.busynodes.extend(self.freenodes[:number_nodes])
            #del(self.freenodes[:number_nodes])            
            return machine_file_name
        return None
    
    
    def setup_charmpp_nodefile(self, allocated_nodes):
        """ Setup charm++ nodefile to use for executing NAMD  
            HACK!! Method violates layering principle
            File $HOME/machinefile in charm++ nodefileformat is written to first node in list
        """
        # Nodelist format:
        # 
        # host tp-x001 ++cpus 2 ++shell ssh 
        # host tp-x002 ++cpus 2 ++shell ssh
        
        nodefile_string=""
        for i in allocated_nodes:
            
            
            if i.has_key("private_hostname"):
                nodefile_string=nodefile_string + "host "+ i["private_hostname"] + " ++cpus " + str(i["cpu_count"]) + " ++shell ssh\n"
            else:
                nodefile_string=nodefile_string + "host "+ i["hostname"] + " ++cpus " + str(i["cpu_count"]) + " ++shell ssh\n"
            
        # copy nodefile to rank 0 node
        jd = saga.job.description()
        jd.executable = "echo"
        jd.number_of_processes = "1"
        jd.spmd_variation = "single"
        # ssh root@tp-x001.ci.uchicago.edu "cat ~/.ssh/id_rsa.pub >> ~/.ssh/authorized_keys"
        jd.arguments = ["\""+nodefile_string+"\"", ">", "machinefile"]
        jd.output = "stdout.txt"
        jd.error = "stderr.txt"
        
        job_service_url = saga.url("ssh://root@"+allocated_nodes[0]["hostname"])
        job_service = saga.job.service(self.session, job_service_url)
        job = job_service.create_job(jd)
        job.run()
        job.wait()
    
    def print_machine_file(self, filename):
         fh = open(filename, "r")
         lines = fh.readlines()
         fh.close
         #print "Machinefile: " + filename + " Hosts: " + str(lines)
         
    def free_nodes(self, job_dir):
         #print "Free nodes ..."
         number_nodes = int(job_dir.get_attribute("NumberOfProcesses"))
         machine_file_name = self.get_machine_file_name(job_dir)
         #print "Machine file: " + machine_file_name
         allocated_nodes = ["localhost"]
         try:
                 machine_file = open(machine_file_name, "r")
                 allocated_nodes = machine_file.readlines()
                 machine_file.close()
         except:
             pass
         for i in allocated_nodes:
             #print "free node: " + str(i) + " current busy nodes: " + str(self.busynodes) + " free nodes: " + str(self.freenodes)
             self.busynodes.remove(i)
             self.freenodes.append(i)
         #print "Delete " + machine_file_name
         os.remove(machine_file_name)
               
            
    def get_machine_file_name(self, job_dir):
        """create machinefile based on jobid"""
        job_dir_url =job_dir.get_url().get_string()        
        job_dir_url = job_dir_url[(job_dir_url.rindex("/", 0, len(job_dir_url)-1)+1)
                                  :(len(job_dir_url)-1)]        
        homedir = os.path.expanduser('~')
        return homedir  + "/advert-launcher-machines-"+ job_dir_url
        
    def poll_jobs(self):
        """Poll jobs from advert service. """
        jobs = []
        # new algorithm separates new jobs and old jobs in separate dirs
        new_jobs = self.new_job_dir.list()
        #print "Base dir: " + self.new_job_dir.get_url().get_string() + " Number New jobs: " + str(len(new_jobs));
        for i in new_jobs:            
                 #print "check job: " + i.get_string()
                 #job_entry = self.new_job_dir.open_dir(i)
                 new_job_item = self.new_job_dir.open_dir(i.get_string(), saga.advert.Create | saga.advert.ReadWrite)
                 job_url = new_job_item.get_attribute("joburl")
                 #print "Found new job: " + str(job_url)
                 job_dir = None
                 try: #potentially racing condition (dir could be already deleted by RE-Manager
                     job_dir = self.base_dir.open_dir(saga.url(job_url), saga.advert.Create | saga.advert.ReadWrite)
                 except:
                     pass
                 if job_dir != None:
                     self.execute_job(job_dir)
                     #print "Execute: " + job_dir.get_attribute("Executable")
                     if job_dir.get_attribute("state")=="Running":
                          self.new_job_dir.remove(new_job_item.get_url(), saga.name_space.Recursive)
        
        # OLD unoptimized code
        #try:
        #jobs = self.base_dir.list()
        ##print "Found " + "%d"%len(jobs) + " jobs in " + str(self.base_dir.get_url().get_string())
        ##except:
        ##    pass
        #for i in jobs:  
        #    ##print i.get_string()
        #    job_dir = None
        #    try: #potentially racing condition (dir could be already deleted by RE-Manager
        #        job_dir = self.base_dir.open_dir(i.get_string(), saga.advert.Create | saga.advert.ReadWrite)
        #    except:
        #        pass
        #    if job_dir != None:
        #        self.execute_job(job_dir)
                
    
    def monitor_jobs(self):
        """Monitor running processes. """   
        for i in self.jobs:
            if self.processes.has_key(i): # only if job has already been starteds
                p = self.processes[i]
                p_state = p.poll()
                #print self.print_job(i) + " state: " + str(p_state) + " return code: " + str(p.returncode)
                if (p_state != None and (p_state==0 or p_state==255)):
                    #print self.print_job(i)  + " finished. "
                    i.set_attribute("state", str(saga.job.Done))
                    self.free_nodes(i)
                    del self.processes[i]
                elif p_state!=0 and p_state!=255 and p_state != None:
                    #print self.print_job(i) + " failed.  "
                    # do not free nodes => very likely the job will fail on these nodes
                    # self.free_nodes(i)
                    del self.processes[i]
                    if self.restarted.has_key(i)==False:
                        #print "Try to restart job " + self.print_job(i)
                        self.restarted[i]=True
                        self.execute_job(i)
                    else:
                        #print "do not restart job " + self.print_job(i)
                        i.set_attribute("state", str(saga.job.Failed))
    
    def print_job(self, job_dir):
        return  ("Job: " + job_dir.get_url().get_string() + " Working Dir: " 
                 + job_dir.get_attribute("WorkingDirectory") 
                 + " Excutable: " + job_dir.get_attribute("Executable"))
                                
    def monitor_checkpoints(self):
        """ parses all job working directories and registers files with Migol via SAGA/CPR """
        #get current files from AIS
        url = saga.url("advert_launcher_checkpoint");
        checkpoint = saga.cpr.checkpoint(url);
        files = checkpoint.list_files()
        for i in files:
            pass
            #print i      
        dir_listing = os.listdir(os.getcwd())
        for i in dir_listing:
            filename = dir+"/"+i
            if (os.path.isfile(filename)):
                if(check_file(files, filename==False)):
                      url = self.build_url(filename)
                      #print str(self.build_url(filename))
                        
    def build_url(self, filename):
        """ build gsiftp url from file path """
        hostname = socket.gethostname()
        file_url = saga.url("gsiftp://"+hostname+"/"+filename)
        return file_url
                
    def check_file(self, files, filename):
        """ check whether file has already been registered with CPR """
        for i in files:
            file_path = i.get_path()
            if (filename == filepath):
                return true
        return false
                        
    def start_background_thread(self):
        self.stop=False
        #print "\n"
        #print "##################################### New POLL/MONITOR cycle ##################################"
        #print "Free nodes: " + str(len(self.freenodes)) + " Busy Nodes: " + str(len(self.busynodes))
        while True and self.stop==False:
            if self.base_dir.exists(self.base_url) == False or self.base_dir.get_attribute("stopped")=="true":
                #print "Job dir deleted - terminate agent"
                break
            else:
                pass
                #print "Job dir: " + str(self.base_dir) + "exists."

            try:
                self.poll_jobs()
                self.monitor_jobs()            
                time.sleep(5)
                self.failed_polls=0
            except saga.exception:
                traceback.print_exc(file=sys.stdout)
                self.failed_polls=self.failed_polls+1
                if self.failed_polls>3: # after 3 failed attempts exit
                    break

    def stop_background_thread(self):        
        self.stop=True
    
    def init_cpr(self):
        # init cpr
        self.js=None
        if self.CPR == True:
            try:
                #print "init CPR monitoring for Agent"
                js = saga.cpr.service()
            except:
                sys.exc_traceback





def get_uuid():
    wd_uuid=""
    wd_uuid += str(uuid.uuid1())
    return wd_uuid


""" Config parameters (will move to config file in future) """
APPLICATION_NAME="BigJob/BigJob"

class bigjob():
    
    def __init__(self, database_host):        
        self.database_host = database_host
        #print "init advert service session at host: " + database_host
        self.uuid = get_uuid()
        self.app_url = saga.url("advert://" + database_host + "/"+APPLICATION_NAME + "-" + str(self.uuid) + "/")
        self.app_dir = saga.advert.directory(self.app_url, saga.advert.Create | saga.advert.CreateParents | saga.advert.ReadWrite)
        self.state=saga.job.Unknown
        self.pilot_url=""
        #print "created advert directory for application: " + self.app_url.get_string()
    
    def start_pilot_job(self, lrms_url,number_nodes):
        
        #register advert entry
        lrms_saga_url = saga.url(lrms_url)
        self.pilot_url = self.app_url.get_string() + "/" + lrms_saga_url.host
        #print "create advert entry: " + self.pilot_url
        self.pilot_dir = saga.advert.directory(saga.url(self.pilot_url), saga.advert.Create | saga.advert.CreateParents | saga.advert.ReadWrite)
        # application level state since globus adaptor does not support state detail
        self.pilot_dir.set_attribute("state", str(saga.job.Unknown)) 
        self.pilot_dir.set_attribute("stopped", "false")
        logging.debug("set pilot state to: " + self.pilot_dir.get_attribute("state"))
        self.number_nodes=int(number_nodes)        
        working_directory = os.getcwd()
                
        jd = saga.job.description()
        jd.number_of_processes = str(number_nodes)
        jd.spmd_variation = "single"
        jd.arguments = ["%s/bigjob_agent_launcher.sh"%(working_directory), self.database_host, self.pilot_url]
        jd.executable = "/bin/bash"
        jd.working_directory = working_directory
        
        js = saga.job.service(lrms_saga_url)
        self.job = js.create_job(jd)
        
        
        #print "Submit pilot job to: " + str(lrms_saga_url)
        argss =[]
        args = sys.argv
        #argss.append(self.database_host)
        argss.append(self.pilot_url)
        #print self.pilot_url
        
        cwd = os.getcwd()
        cmd = "cd " + cwd + "/ ; python " + " bigjob.py " + self.database_host + " " + self.pilot_url
        #print cmd 
        pid = subprocess.Popen(cmd,shell=True).pid
        
        #kproc = os.fork(bigjob_agent, args=(self.database_host,self.pilot_url))
        #time.sleep(10)
        #kproc.start()
        #self.job.run()
        return self.job
     
    def get_state(self):        
        """ duck typing for get_state of saga.cpr.job and saga.job.job  """
        try:
            return self.job.get_state()
        except:
            return None
    
    def get_state_detail(self): 
        try:
            return self.pilot_dir.get_attribute("state")
        except:
            return None
    
    def get_free_nodes(self):
        jobs = self.pilot_dir.list()
        number_used_nodes=0
        for i in jobs:
            job_dir=None
            try:
                job_dir = self.pilot_dir.open_dir(i.get_string(), saga.advert.Create | saga.advert.ReadWrite) 
            except:
                pass
            if job_dir != None and job_dir.attribute_exists("state") == True\
                and str(job_dir.get_attribute("state"))==str(saga.job.Running):
                job_np = "1"
                if (job_dir.attribute_exists("NumberOfProcesses") == True):
                    job_np = job_dir.get_attribute("NumberOfProcesses")

                number_used_nodes=number_used_nodes + int(job_np)
        return (self.number_nodes - number_used_nodes)

    
    def stop_pilot_job(self):
        """ mark in advert directory of pilot-job as stopped """
        try:
            #self.app_dir.change_dir("..")
            #print "stop pilot job: " + str(self.app_url)
            self.pilot_dir.set_attribute("stopped", "true")
        except:
            pass
    
    def cancel(self):        
        """ duck typing for cancel of saga.cpr.job and saga.job.job  """
        #print "Cancel Pilot Job"
        try:
            self.job.cancel()
        except:
            pass
            #traceback.print_stack()
        try:
            #self.app_dir.change_dir("..")
            #print "delete pilot job: " + str(self.app_url)
            self.app_dir.remove(self.app_url, saga.name_space.Recursive)    
        except:
            pass
            #traceback.print_stack()

    def __repr__(self):
        return self.pilot_url 

    def __del__(self):
        self.cancel()

                    
                    
class subjob():
    
    def __init__(self, database_host):
        """Constructor"""
        self.database_host = database_host
        self.job_url=None
        self.uuid = get_uuid()
        self.job_url = None
        
    def get_job_url(self, pilot_url):
        self.saga_pilot_url = saga.url(pilot_url)
        if(self.saga_pilot_url.scheme=="advert"): #
            pass

        else: # any other url, try to guess pilot job url
            host=""
            try:
                host = self.saga_pilot_url.host
            except:
                pass
            if host =="":
                host=socket.gethostname()
            # create dir for destination url
            self.saga_pilot_url = saga.url("advert://" +  self.database_host + "/"+APPLICATION_NAME + "/" + host)

        # create dir for job
        self.job_url = self.saga_pilot_url.get_string() + "/" + str(self.uuid)
        return self.job_url

    def submit_job(self, pilot_url, jd):
        """ submit job via advert service to NAMD-Launcher 
            dest_url - url reference to advert job or host on which the advert job is going to run"""
        #print "submit job: " + str(pilot_url)
        if self.job_url==None:
            self.job_url=self.get_job_url(pilot_url)

        for i in range(0,3):
            try:
                #print "create job entry "
                self.job_dir = saga.advert.directory(saga.url(self.job_url), 
                                             saga.advert.Create | saga.advert.CreateParents | saga.advert.ReadWrite)
                #print "initialized advert directory for job: " + self.job_url
                # put job description attributes to advert
                attributes = jd.list_attributes()                
                for i in attributes:          
                        if jd.attribute_is_vector(i):
                            self.job_dir.set_vector_attribute(i, jd.get_vector_attribute(i))
                        else:
                            logging.debug("Add attribute: " + str(i) + " Value: " + jd.get_attribute(i))
                            self.job_dir.set_attribute(i, jd.get_attribute(i))

                self.job_dir.set_attribute("state", str(saga.job.Unknown))
                
                #print "create notification flag in NEW dir"
                self.new_job_url = self.saga_pilot_url.get_string() + "/new/" + str(self.uuid)
                self.new_job_dir = saga.advert.directory(saga.url(self.new_job_url), 
                                             saga.advert.Create | saga.advert.CreateParents | saga.advert.ReadWrite)
                self.new_job_dir.set_attribute("joburl", self.job_url)
                                
                # return self object for get_state() query    
                #logging.debug("Submission time (time to create advert entries): " + str(time.time()-start) + " s")
                return self    
            except:
                traceback.print_exc(file=sys.stdout)
                time.sleep(2)
                #raise Exception("Unable to submit job")      

    def get_state(self):        
        """ duck typing for get_state of saga.cpr.job and saga.job.job  """
        return self.job_dir.get_attribute("state")
    
    def delete_job(self):
        #print "delete job and close dirs: " + self.job_url
        try:
            self.job_dir.change_dir("..")
            self.job_dir.remove(saga.url(self.job_url), saga.name_space.Recursive)
            self.job_dir.close()
        except:
            pass

    def __del__(self):
        self.delete_job()
    
    def __repr__(self):        
        if(self.job_url==None):
            return "None"
        else:
            return self.job_url
  
         
""" Test Job Submission via Advert """
if __name__ == "__main__":
    
    
    args = sys.argv
    num_args = len(args)
    
    #print num_args
    if (num_args==1):
        ##########################################################################################
        # Start BigJob
        num_subjobs = 1 # agents
        lrms_url = "fork://localhost" # resource url

        # start pilot job (bigjob_agent)
        #print "Start Pilot Job/BigJob:  bigjob_agent at: " + lrms_url
        bj = []
        for i in range(0, num_subjobs):
            bj.append(bigjob(advert_host))
            bj[i].start_pilot_job(lrms_url,2)
            #print "Pilot Job/BigJob URL: " + bj[i].pilot_url + " State: " + str(bj[i].get_state())

        ##########################################################################################
        # Submit SubJob through BigJob
        sj =[]
        sj_done = []
        for i in range(0, num_subjobs):
            jd = saga.job.description()
            jd.executable = "/bin/date"
            jd.number_of_processes = "1"
            jd.spmd_variation = "single"
            jd.arguments = [""]
            jd.working_directory = os.getcwd() 
            jd.output = "stdout-"+str(i)+".txt"
            jd.error = "stderr-"+str(i)+".txt"
            sj.append(subjob(advert_host))
            sj[i].submit_job(bj[i].pilot_url, jd)
            sj_done.append(0)
        k = 0
        # busy wait for completion
        
        
        while 1: 
            k =0
            for i in range(0, num_subjobs):
                
                state = str(sj[i].get_state())
                #print "state: " + state
                if(state=="Failed" or state=="Done"):
                   sj_done[i] = 1
            for i in range(0, num_subjobs):
                if (sj_done[i] == 1) :
                   k = k +1
            if (k >= num_subjobs):   
                break
            
            time.sleep(2)

        # Cleanup - stop BigJob
        for i in range(0, num_subjobs):   
           bj[i].cancel()    
    else:
        #bigjob_agent
        #print "start agent"
        bigjob_agent = bigjob_agent(args[1], args[2])
        
