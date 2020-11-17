#include <bits/stdc++.h>

void kill_processes_on_tcp_port(const std::string &port)
{
    const auto kill_port_cmd = "fuser -k " + port + "/tcp";
    std::cerr << "Killing processes on port '" << port << "'" << std::endl;
    auto pipe = popen(kill_port_cmd.c_str(), "r");
    if (!pipe)
        throw "'" + kill_port_cmd + "' failed to start.";
    
    const auto exit_code = pclose(pipe);
    if (exit_code)
        throw "'" + kill_port_cmd + "' failed with code: " + std::to_string(exit_code);
    
    std::cerr << "Killed processes on port '" << port << "'" << std::endl;
}