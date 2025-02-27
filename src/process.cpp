#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

// TODO: Return this process's ID
int Process::Pid() { return pid ? pid : 0; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    long seconds = LinuxParse::UpTime(pid);
    long uptime = LinuxParse::ActiveJiffies(pid);

    return float(uptime) / float(seconds);
}

// TODO: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid); }

// TODO: Return this process's memory utilization
string Process::Ram() { return LinuxParse::Ram(pid); }

// TODO: Return the user (name) that generated this process
string Process::User() {
    string uid = LinuxParser::Uid()

    return string();
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a [[maybe_unused]]) const {
  return true;
}
