#include <bits/stdc++.h>
#include <zmqpp/zmqpp.hpp>

class Client
{
public:
    Client(const std::string &path);
    ~Client();

    std::string get_port() const;
    std::string get_path() const;
    
    // run build.sh script
    void build() const;

    // run run.sh script
    void run() const;

    // send a simple request and get a response
    std::string request(const std::string &msg) const;


    friend std::ostream& operator<<(std::ostream& os, const Client& c);

private:
    void init_sockets();

    std::string port;
    std::string path;
};
