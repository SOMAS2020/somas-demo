#include <bits/stdc++.h>

// kill_processes_on_tcp_port does what it says. port is assumed to be a valid integer string.
// equivalent to running `fuser -k <port>/tcp`.
void kill_processes_on_tcp_port(const std::string &port);