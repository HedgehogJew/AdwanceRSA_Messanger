#ifndef INPUT_THREAD_CTRL_H
#define INPUT_THREAD_CTRL_H

#include <vector>
#include <thread>
#include <mutex>
#include <string>
#include <boost/multiprecision/cpp_int.hpp>

#include "connection.h"
#include "threadQueue.h"
#include "netMessage.h"
#include "OutputThreadCtrl.h"

using namespace boost;

class InputThreadCtrl
{
private:
    std::thread inputThread;
    std::vector<Connection>& connectionsVec;
    bool stopFlag;
    std::mutex stopMutex;

    OutputThreadCtrl& outputThreadControl;

    static std::unique_ptr<InputThreadCtrl> instance;
    static std::once_flag initFlag;

    int controllerType;

public:
    InputThreadCtrl() = delete;
    InputThreadCtrl& operator=(const InputThreadCtrl&) = delete;

    static InputThreadCtrl& getInstance(OutputThreadCtrl& outputThreadControl, std::vector<Connection>& conVec, int ctrlType);

    ~InputThreadCtrl();

private:
    InputThreadCtrl(OutputThreadCtrl& outputThreadCtrl, std::vector<Connection>& conVec, int ctrlType);

    void run();
};

#endif // INPUT_THREAD_CTRL_H
