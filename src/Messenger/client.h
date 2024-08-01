#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <ctime>
#include <boost/asio.hpp>
#include <string>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <chrono>
#include <sstream>

#include "netMessage.h"
#include "connection.h"
#include "../AdvancedRSA/RSAWizard.h"
#include "../AdvancedRSA/GOST28147.h"

using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;
using namespace boost::multiprecision;

class Client
{
private:
    boost::asio::io_context io_context;
    Connection clientConnection;
    RSAWizard rsaWizard;
    GOSTWizard gostWizard;
    cpp_int sessionKey;
    bool stopFlag;

    std::mutex stopMutex;

public:
    Client() : clientConnection(io_context), stopFlag(false) {}
    
    Client(const Client&) = delete;

    ~Client()
    {
        stopFlag = true;
        clientConnection.disconnect();
    }
    
    bool ConnectTo(const std::string& host, const std::string& port);

    bool serverHandshake();

    bool sendString(const std::string& src);

    bool sendUserInput();

    bool getMessageFromServer();

    void waitAndPrintMessages();

    void run();

};

#endif  // CLIENT_H