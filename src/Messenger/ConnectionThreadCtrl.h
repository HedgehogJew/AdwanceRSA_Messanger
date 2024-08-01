#ifndef CONNECTION_THREAD_CTRL_H
#define CONNECTION_THREAD_CTRL_H

#include <vector>
#include <thread> 
#include <mutex>
#include <memory>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/asio.hpp>

#include "connection.h"
#include "threadQueue.h"
#include "netMessage.h"

class ConnectionThreadCtrl
{
private:
    boost::asio::io_context& ioContext;
    std::thread listenerThread;
    std::vector<Connection>& connectionVec;
    bool stopFlag;
    std::mutex stopMutex;
    
    std::string host;
    std::string port;

    static std::unique_ptr<ConnectionThreadCtrl> instance;
    static std::once_flag initFlag;

    int controllerType;

public:
    ConnectionThreadCtrl() = delete;
    ConnectionThreadCtrl& operator=(const ConnectionThreadCtrl&) = delete;

    static ConnectionThreadCtrl& getInstance(boost::asio::io_context& ioContext, std::vector<Connection>& conVec,
        const std::string& host, const std::string& port, int ctrlType);

    ~ConnectionThreadCtrl();

private:
    ConnectionThreadCtrl(boost::asio::io_context& ioContext, std::vector<Connection>& conVec,
        const std::string& host, const std::string& port, int ctrlType);

    void run();
};

#endif // CONNECTION_THREAD_CTRL_H
