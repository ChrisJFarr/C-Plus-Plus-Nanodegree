#include "system.h"
#include <unistd.h>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::fstream;
using std::ifstream;
using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

Process System::CreateProcess(int pid){
    // Create the Process, set the PID
    Process new_process;
    new_process.SetPid(pid);

    // Collect information?
    new_process.CpuUtilization();
    new_process.UpTime();
    return new_process;
}

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 

    // // Clear processes_
    // processes_.clear();
    // Create a new vector to build a new instead of deleting elements from original
    vector<Process> target_processes_ = {};
    // processes_ original vector
    // loop over each 
    // pid_processes_ must be sorted ascending (this is maintained through the algorithm below)
    // LinuxParser::Pids() must be sorted ascending (usually sorted so this shouldn't cost anything using std::sort)
    vector<int> new_pids = LinuxParser::Pids();
    std::sort(new_pids.begin(), new_pids.end());
    // initialize position indices target-position and new-position
    unsigned int p_pos, n_pos;

    // Algorithm for reconciling two sorted vectors
    
    // get first pid
    
    // set pos=0
    p_pos = 0;
    n_pos = 0;
    //target_processes_
    // loop until pid == pid_processes_[-1].GetPid()
    while(true){
        if(pid_processes_.size() == 0) {
            // Being ran the first time
            // if pid_processes_.size() == pos:
            //  // This portion starts adding for new lists and ends the loop
            //  create new process, push_back process in pid_processes_
            //  increment pos
            target_processes_.push_back(CreateProcess(new_pids[n_pos]));
            //  get next pid
            n_pos++;
        } else if(new_pids[n_pos] > pid_processes_[p_pos].Pid()){
            // 
            // else if pid > pid_processes_[pos].GetPid():
            //  remove pid at pos from pid_processes_, this pid is gone
            // Skip adding the existing process to the target list
            // increment pos
            p_pos++;
        } else if(new_pids[n_pos] == pid_processes_[p_pos].Pid()) {
            // else if pid == pid_processes_[pos].GetPid():
            //  increment pos
            //  get next pid
            target_processes_.push_back(CreateProcess(new_pids[n_pos]));
            p_pos++;
            n_pos++;
        } else if(new_pids[n_pos] < pid_processes_[p_pos].Pid()){
            // This is a new process
            // else if pid < pid_processes_[pos].GetPid():
            //  create new process, insert into pid_processes_ at pos
            //  increment pos
            //  get next pid
            target_processes_.push_back(CreateProcess(new_pids[n_pos]));
            n_pos++;
        } else {  // No other possibility
            break;
        }

        // TODO Check for stopping criteria
        // Check if both n_pos == new_pids size and p_pos
        if(new_pids.size() <= n_pos && pid_processes_.size() <= p_pos) break;
        n_pos = (new_pids.size() <= n_pos) ? n_pos - 1 : n_pos;
    }

    // Copy target_processes_ to pid_processes_
    // Copy pid_processes_ to processes_
    pid_processes_.clear();
    processes_.clear();
    for(Process p : target_processes_){
        pid_processes_.push_back(p);
        processes_.push_back(p);
    }

    // sort processes_ (sorts by overridden operator)
    std::sort(processes_.begin(), processes_.end());

    // for(int p: LinuxParser::Pids()){
    //     // Loop through each process PID

    //     // Create the Process, set the PID
    //     Process new_process;
    //     new_process.SetPid(p);

    //     // Collect information?
    //     new_process.CpuUtilization();
    //     new_process.UpTime();

    //     // Pushback processes_ vector
    //     processes_.push_back(new_process);
    // }

    // // Copy to processes_ and sort 
    // std::sort(processes_.begin(), processes_.end());

    return processes_; 
    
    }

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }