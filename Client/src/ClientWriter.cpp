#include <iostream>
#include <boost/thread.hpp>
#include "../include/ConnectionHandler.h"
using namespace boost::this_thread;

class ClientWriter {
private:
    ConnectionHandler _handler;
    bool _shouldTerminate;
public:
    ClientWriter (ConnectionHandler& handler) : _handler(handler),_shouldTerminate(false) {}

    void operator()(){
        while (!_handler.shouldTerminate()) {
            try {//checks if thread was interrupted
                boost::this_thread::interruption_point();
                const short bufsize = 1024;
                char buf[bufsize];
                std::cin.getline(buf, bufsize);
                std::string line(buf);
                size_t len=line.length();
                if (!_handler.sendLine(line)) {
                    std::cout << "Disconnected. Exiting...\n" << std::endl;
                    break;
                }
                std::cout << "Sent " << len+1 << " bytes to server" << std::endl;
            }
            catch (boost::thread_interrupted&) {//
                std::cout << "Disconnected. Exiting...\n" << std::endl;
                break;
            }
};
 