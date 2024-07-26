#include <string>
#include <format>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    string hr = static_cast<string>(static_cast<int>(seconds / 3600));
    string min = static_cast<string>(static_cast<int>((seconds % 3600)/ 60));
    string sec = static_cast<string>(static_cast<int>((seconds % 3600)% 60));
    // prepend 0s as needed for HH:MM:SS
    string hrs = string(2-hr.length(), "0") + hr;
    string mins = string(2-min.length(), "0") + min;  
    string secs = string(2-sec.length(), "0") + sec;
    string result = "";
  
    if (hrs.compare("00") != 0) {
        result = std::format("{}:{}:{}", hrs, mins, secs);
    } else {
        result = std::format("{}:{}", mins, secs);
    }
    return result;
}
