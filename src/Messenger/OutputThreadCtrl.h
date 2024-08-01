#ifndef OUTPUT_THREAD_CTRL_H
#define OUTPUT_THREAD_CTRL_H

#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>

#include "connection.h"
#include "threadQueue.h"
#include "netMessage.h"
#include "../AdvancedRSA/RSAWizard.h"
#include "../AdvancedRSA/GOST28147.h"
#include "../AdvancedRSA/stringAndSplit.h"

class OutputThreadCtrl
{
private:
    std::thread outputThread;
    std::vector<Connection>& connectionsVec;
    tQueue<std::pair<Message, Connection&>> taskQueue;
    bool stopFlag;
    std::mutex stopMutex;

    static std::unique_ptr<OutputThreadCtrl> instance;
    static std::once_flag initFlag;

    int controllerType;
    RSAWizard rsaWizard;
    GOSTWizard gostWizard;

public:
    OutputThreadCtrl(const OutputThreadCtrl&) = delete;
    OutputThreadCtrl& operator=(const OutputThreadCtrl&) = delete;

    static OutputThreadCtrl& getInstance(std::vector<Connection>& conVec, int ctrlType);
    void addTask(const Message& msg, Connection& conn);
    void initCryptographi(int code);

    ~OutputThreadCtrl();

private:
    OutputThreadCtrl(std::vector<Connection>& conVec, int ctrlType);
    void run();
    void handleMessage(Message msg, Connection& conn);
};

#endif
