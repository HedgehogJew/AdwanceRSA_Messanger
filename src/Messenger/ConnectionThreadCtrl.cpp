#include "ConnectionThreadCtrl.h"

std::unique_ptr<ConnectionThreadCtrl> ConnectionThreadCtrl::instance;
std::once_flag ConnectionThreadCtrl::initFlag;

ConnectionThreadCtrl::ConnectionThreadCtrl(boost::asio::io_context& ioContext, std::vector<Connection>& conVec,
    const std::string& host, const std::string& port, int ctrlType)
    : ioContext(ioContext),
    connectionVec(conVec),
    stopFlag(false),
    host(host),
    port(port),
    controllerType(ctrlType)
{
    listenerThread = std::thread(&ConnectionThreadCtrl::run, this);
}

ConnectionThreadCtrl::~ConnectionThreadCtrl()
{
    {
        std::lock_guard<std::mutex> lock(stopMutex);
        stopFlag = true;
    }
    if (listenerThread.joinable()) {
        listenerThread.join();
    }
}

ConnectionThreadCtrl& ConnectionThreadCtrl::getInstance(boost::asio::io_context& ioContext, std::vector<Connection>& conVec,
    const std::string& host, const std::string& port, int ctrlType)
{
    std::call_once(initFlag, [&ioContext, &conVec, &host, &port, &ctrlType]() {
        instance.reset(new ConnectionThreadCtrl(ioContext, conVec, host, port, ctrlType));
    });
    return *instance;
}

void ConnectionThreadCtrl::run()
{
    while (true) {
        {
            if (stopFlag) {
                break;
            }
        }

        Connection serverConnection(ioContext);
        serverConnection.acceptAs(host, port);
        {
            std::lock_guard<std::mutex> lock(stopMutex);
            connectionVec.emplace_back(std::move(serverConnection));
        }
        
    }
}
