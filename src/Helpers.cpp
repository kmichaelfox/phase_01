//
//  Helpers.cpp
//  phase_01
//
//  Created by Kelly Fox on 5/6/16.
//
//

#include "Helpers.hpp"
#include <stdio.h>
#include <unistd.h>

using namespace Helpers;
using namespace std;

std::string exec(const char * cmd);

int Helpers::scanForProcessPort(string processName, PortSearch::Type isBlocking) {
    const char * cmd = "lsof -i UDP | grep sclang";
    
    string procs = exec(cmd);
    
    size_t pos = procs.find("*:");
    
    // if tokens found, slice string and return as int
    if (pos != string::npos) {
        procs = procs.substr(pos+2, string::npos);
        cout << procs << endl;
        return atoi(procs.c_str());
    }
    
    // port not found, return 0
    return 0;
}

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer, 128, pipe.get()) != NULL)
            result += buffer;
    }
    return result;
}