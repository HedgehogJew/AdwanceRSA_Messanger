#include "OutputThreadCtrl.h"

using namespace boost;
using namespace boost::multiprecision;

std::unique_ptr<OutputThreadCtrl> OutputThreadCtrl::instance;
std::once_flag OutputThreadCtrl::initFlag;

OutputThreadCtrl::OutputThreadCtrl(std::vector<Connection>& conVec, int ctrlType)
    : connectionsVec(conVec), 
    stopFlag(false), 
    controllerType(ctrlType)
{
    outputThread = std::thread(&OutputThreadCtrl::run, this);
}

OutputThreadCtrl::~OutputThreadCtrl()
{
    {
        std::lock_guard<std::mutex> lock(stopMutex);
        stopFlag = true;
    }
    taskQueue.notify_all();
    if (outputThread.joinable()) {
        outputThread.join();
    }
}

OutputThreadCtrl& OutputThreadCtrl::getInstance(std::vector<Connection>& conVec, int ctrlType)
{
    std::call_once(initFlag, [&conVec, &ctrlType]() { instance.reset(new OutputThreadCtrl(conVec, ctrlType)); });
    return *instance;
}

void OutputThreadCtrl::addTask(const Message& msg, Connection& conn)
{
    taskQueue.push_back(std::make_pair(msg, std::ref(conn)));
}

void OutputThreadCtrl::run()
{
    
    while (true) {
        {
            std::lock_guard<std::mutex> lock(stopMutex);
            if (stopFlag) {
                break;
            }
        }

        auto task = taskQueue.wait_and_pop();
        if (task)
        {
            std::lock_guard<std::mutex> lock(stopMutex);
            auto& [msg, conn] = *task;
            handleMessage(msg, conn);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void OutputThreadCtrl::initCryptographi(int code)
{
    if(code == 1)
    {
        rsaWizard.generatePrimeNumbers();
        return;
    }
    rsaWizard.prepareKeys();   
    return;
}

void OutputThreadCtrl::handleMessage(Message msg, Connection& conn)
{
    switch (msg.header.id)
    {
    case ID_HANDSHAKE_OPEN_KEY_REQUEST:
        if(controllerType == ID_SERVER){
            auto openKeys = rsaWizard.getOpenKeys();

            std::stringstream ss;
            ss << openKeys.first << " " << openKeys.second;
            std::string data = ss.str();

            std::vector<uint8_t> body(data.begin(), data.end());

            Message res(ID_HANDSHAKE_OPEN_KEY_RESPONSE, body.size(), body);
            conn.sendMessage(res);
        } else {
            conn.sendMessage(Message(ID_ERROR,0,""));
        }
        break;

    case ID_HANDSHAKE_SESSION_KEY_RESPONSE:
        if(controllerType == ID_SERVER){
            std::string str = uint8tVectorToCString(msg.body);
            cpp_int sessionKey {str};

            cpp_int decrpyptedSessionKey = rsaWizard.decrypt(sessionKey);
            conn.setSessionKey(decrpyptedSessionKey);

        } else {
            conn.sendMessage(Message(ID_ERROR,0,""));
        }
        break;

    case ID_SEND_MESSAGE_TO_SERVER:
        if(controllerType == ID_SERVER){
            std::string decryptedStr = gostWizard.decrypt(msg.body, conn.getSessionKey());

            for(auto& it : connectionsVec) {
                if(it != conn) {
                    std::vector<uint8_t> cryptedStr = gostWizard.encrypt(decryptedStr, it.getSessionKey());
                    Message personalMessage(ID_SEND_MESSAGE_TO_CLIENT,cryptedStr.size(), cryptedStr);
                    it.sendMessage(personalMessage);

                }
            }
        } else {
            conn.sendMessage(Message(ID_ERROR,0,""));
        }
        break;

    default:
        conn.sendMessage(Message(ID_ERROR,0,""));
        break;
    }
}
