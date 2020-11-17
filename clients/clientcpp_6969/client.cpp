#include <zmqpp/zmqpp.hpp>
#include <string>
#include <iostream>
#include <chrono>
#include <thread>

#define PORT 6969

using namespace std;

int main(int argc, char *argv[])
{
    const string endpoint = "tcp://*:" + to_string(PORT);

    // initialize the 0MQ context
    zmqpp::context context;

    // generate a pull socket
    zmqpp::socket_type type = zmqpp::socket_type::reply;
    zmqpp::socket socket(context, type);

    // bind to the socket
    socket.bind(endpoint);
    // receive the message
    zmqpp::message message;
    // decompose the message
    socket.receive(message);
    string text;
    message >> text;

    //Do some 'work'
    std::this_thread::sleep_for(std::chrono::seconds(1));
    cout << "Received" << text << endl;
    socket.send("6969");
}