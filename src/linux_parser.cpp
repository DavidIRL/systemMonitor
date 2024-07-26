#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <string>
#include <sstream>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    std::ifstream stream(kProcDirectory + kMeminfo);

    if (stream.is_open()) {
        string memTotal, memFree;
        string procName, procValue, usage;
        string line;
        
        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> procName >> procValue >> usage) {
                if (procName == "MemTotal:") {
                    memTotal = procValue;
                } else if (procName == "MemFree:") {
                    memFree = procValue;
                }
            }
        }
        return (float) memFree / (float) memTotal;
    }
    // stream not open so return 0
    return 0.0;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
    std::ifstream stream(kProcDirectory + kUptimeFilename);

    if (stream.is_open()) {
        string value, line;
        std::getline(stream, line);
        std::istringstream linestream(line);
        value = linestream;

        return stol(value);
    }
    // stream not open so return 0
    return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return ActiveJiffies() + IdleJiffies(); }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        string word, line;
        vector<string> words;
        std::getline(stream, line);
        std::istringstream linestream(line);

        while (getline(linestream, word, ' ')) {
            words.insert(words.end(), word);
        }
        long procTime = (stof(words[13])+stof(words[14])+stof(words[15])+stof(words[16]));
        return procTime / sysconf(_SC_CLK_TCK);
    }
    // stream not open so return 0
    return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
    vector<int> pids = LinuxParser::Pids();
    long actives;
    for (int pid : pids) {
        actives += LinuxParser::ActiveJiffies(pid);
    }
    return actives;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0;}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        string value, line;
        string cpu, user, nvalue, sys, idle;
    
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> cpu >> user >> nvalue >> sys >> idle;

        return vector<string>{user, nvalue, sys, idle};
    }
    // stream not open so return empty{}
    return {};
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        string key, line;
        string value = "";

        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "processes") {
                    return stoi(value);
                }
            }
        }
    }
    // stream not open so return 0
    return 0;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        string key, line;
        string value = "";

        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> key >> value) {
                if (key == "procs_running") {
                    return stoi(value);
                }
            }
        }
    }
    // stream not open so return 0
    return 0;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
    std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    if (stream.is_open()) {
        string line;
        
        while (std::getline(stream, line)) {
            return line;
        }
    }
    // stream not open so return ""
    return "";
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
    std:ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
        string line, procName, procValue;

        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> procName >> procValue) {
                if (procName == "VmSize:") {
                    int size = stoi(procValue) * .001;
                    return to_string(size);
                }
            }
        }
    }
    // stream not open so return ""
    return "";
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
        string line, procName, procValue;

        while (std::getline(stream, line)) {
            std::istringstream linestream(line);
            while (linestream >> procName >> procValue) {
                if (procName == "Uid:") {
                    return procValue
                }
            }
        }
    }
    // stream not open so return ""
    return "";
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {

    return "";
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if (stream.is_open()) {
        string word, line;

        std::getline(stream, line);
        string uptime = line[21];
        
        return stol(uptime) / sysconf(_SC_CLK_TCK);
    }
    return 0;
}
