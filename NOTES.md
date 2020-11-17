## Design choices

### On the language issue
See https://stackoverflow.com/questions/32681838/difference-between-zeromq-and-ipc
- 0MQ
    - Unfortunately seems to be documented-well for Linux only, so no point using it as a X-platform option
- Linux sockets
    - Too much overhead to read through
    
sudo apt-get install libzmqpp-dev

https://github.com/zeromq/libzmq
https://github.com/zeromq/zmqpp
