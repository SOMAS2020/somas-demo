#include <bits/stdc++.h>
#include <zmqpp/zmqpp.hpp>
#include "client.hpp"
#include "helper.hpp"

Client::Client(const std::string &_path)
{
    path = _path;
    const auto port_str = path.substr(path.length() - 4);
    const auto port_num = std::stoi(port_str);
    if (port_num < 1024)
    {
        std::stringstream ss;
        ss << "Error for: '" << path << "': port number '"
           << port_str << "' not >= 1024" << std::endl;
        throw ss.str();
    }

    port = port_str;
}

Client::~Client()
{
    try
    {
        kill_processes_on_tcp_port(port);
    }
    catch (std::string &e)
    {
        std::cerr << "Failed to kill client '" << *this << "' on port '" << port << "': " << e;
    }
}

std::string Client::get_port() const { return port; }
std::string Client::get_path() const { return path; }

void Client::build() const
{
    const auto build_cmd = path + "/build.sh";
    std::cerr << "START Building: " << build_cmd << std::endl;
    auto exit_code = system(build_cmd.c_str());
    if (exit_code)
    {
        std::stringstream ss;
        ss << "Build script '" << build_cmd << "' for '" << *this
           << "' failed with code: " << exit_code << std::endl;
        throw ss.str();
    }
    std::cerr << "FINISHED Building: " << build_cmd << std::endl;
}

void Client::run() const
{
    // kill all processes on that port first (throwable)
    kill_processes_on_tcp_port(port);

    const auto run_cmd = path + "/run.sh";
    std::cerr << "START client: '" << run_cmd << "'" << std::endl;
    auto pipe = popen(run_cmd.c_str(), "r");
    if (!pipe)
    {
        std::stringstream ss;
        ss << run_cmd << "' failed to start." << std::endl;
        throw ss.str();
    }
    std::cerr << "Client '" << *this << "' started!" << std::endl;
    // don't pclose here - the endpoints are starting :)
}

std::string Client::request(const std::string &msg) const
{
    const auto endpoint = "tcp://localhost:" + port;
    // initialize the 0MQ context
    zmqpp::context context;

    // generate a push socket
    zmqpp::socket_type type = zmqpp::socket_type::req;
    zmqpp::socket socket(context, type);

    // open the connection
    socket.connect(endpoint);
    zmqpp::message message;

    message << msg;
    socket.send(message);
    std::string ret;
    socket.receive(ret);
    return ret;
        
}

std::ostream &operator<<(std::ostream &os, const Client &c)
{
    os << c.path;
    return os;
}
