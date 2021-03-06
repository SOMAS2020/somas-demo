#include <zmqpp/zmqpp.hpp>
#include <bits/stdc++.h>
#include "client.hpp"
#include "consts.hpp"

// get_clients gets a mapping from the port string to the `clients/<teamname>_<portnum>`
// representation of the client.
// Health checks like port number >=1024 and repeat port numbers are run.
void get_clients(std::vector<Client *> &clients)
{
    if (!std::filesystem::exists("clients"))
    {
        std::cerr << "'clients' directory does not exist!" << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::regex path_regex("clients/[a-zA-Z0-9]+_[0-9]{4}");

    std::unordered_set<std::string> used_ports;

    // iterate through all directories in `clients`
    for (const auto &p : std::filesystem::directory_iterator("clients"))
        if (p.is_directory())
        {
            const std::string path = p.path();
            if (!std::regex_match(path, path_regex))
            {
                std::cerr << "Ignoring '" << path << "': does not match required regex" << std::endl;
                continue;
            }

            try
            {
                Client *c = new Client(path);
                auto port = c->get_port();
                if (used_ports.count(port))
                {
                    std::stringstream ss;
                    ss << "Error for '" << c << "': port number '"
                              << port << "' already used" << std::endl;
                    throw ss.str();
                }
                used_ports.insert(port);
                clients.push_back(c);
            }
            catch (std::string &e)
            {
                std::stringstream ss;
                ss << "Failed to create client for path: '" << path << "': " << e;
                throw ss.str();
            }
        }
}

// build_clients runs the build.sh scripts of the client.
void build_clients(const std::vector<Client *> &clients)
{
    for (const auto &x : clients)
    {
        try
        {
            x->build();
        }
        catch (std::string &e)
        {
            std::stringstream ss;
            ss << "Can't build '" << *x << "': " << e << std::endl;
            throw ss.str();
        }
    }
}

// run_clients kills all processes on the port then runs the run.sh scripts
// of the clients.
void run_clients(const std::vector<Client *> &clients)
{
    for (const auto &x : clients)
    {
        try
        {
            x->run();
        }
        catch (std::string &e)
        {   
            std::stringstream ss;
            ss << "Can't run '" << *x << "': " << e << std::endl;
            throw ss.str();
        }
    }
}

void send_first_hello(const std::vector<Client *> &clients)
{
    for (const auto &x : clients)
    {
        std::cerr << "Trying to send first hello to '" << *x << "'" << std::endl;

        // try to see if we can get back the port
        const auto port = x->get_port();
        auto got = x->request(port);

        if (got != port)
        {
            std::stringstream ss;
            ss << "got != want: " << got << " != " << port << std::endl;
            throw ss.str();
        }
        std::cerr << *x << " OK! " << std::endl;
    }
}

void teardown(const std::vector<Client *> clients) {
    for (const auto &x : clients)
    {
        x->~Client();
    }
}

int main(int argc, char *argv[])
{
    std::vector<Client *> clients;
    try {
        get_clients(clients);
        std::cerr << "Got clients: " << std::endl;
        for (const auto &x : clients)
            std::cerr << "\t" << *x << std::endl;

        std::cerr << "Building clients!" << std::endl;
        build_clients(clients);
        std::cerr << "Finished starting clients!" << std::endl;
        std::cerr << "Starting clients!" << std::endl;
        run_clients(clients);
        std::cerr << "Finished starting clients!" << std::endl;
        std::cerr << "Sending first requests!" << std::endl;
        send_first_hello(clients);
        std::cerr << "Finished requests!" << std::endl;
    } 
    catch (std::string& e)
    {
        teardown(clients);
        std::cerr << e << std::endl;
        return EXIT_FAILURE;
    }
    catch (std::exception& e)
    {
        teardown(clients);
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        teardown(clients);
        std::cerr << "Unknown exception occurred" << std::endl;
        return EXIT_FAILURE;
    }
}
