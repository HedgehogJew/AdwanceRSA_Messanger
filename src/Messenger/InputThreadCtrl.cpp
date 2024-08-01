#include "InputThreadCtrl.h"

using namespace boost;

std::unique_ptr<InputThreadCtrl> InputThreadCtrl::instance;
std::once_flag InputThreadCtrl::initFlag;

InputThreadCtrl::InputThreadCtrl(OutputThreadCtrl& outputThreadCtrl, std::vector<Connection>& conVec, int ctrlType)
    : connectionsVec(conVec),
    stopFlag(false),
    outputThreadControl(outputThreadCtrl),
    controllerType(ctrlType)
{
    inputThread = std::thread(&InputThreadCtrl::run, this);
}

InputThreadCtrl::~InputThreadCtrl()
{
    {
        std::lock_guard<std::mutex> lock(stopMutex);
        stopFlag = true;
    }
}

InputThreadCtrl& InputThreadCtrl::getInstance(OutputThreadCtrl& outputThreadControl, std::vector<Connection>& conVec, int ctrlType)
{
    std::call_once(initFlag, [&outputThreadControl, &conVec, &ctrlType]() { 
        instance.reset(new InputThreadCtrl(outputThreadControl, conVec, ctrlType)); });
    return *instance;
}

void InputThreadCtrl::run()
{
    while (true) {
        {
            std::lock_guard<std::mutex> lock(stopMutex);
            if (stopFlag) {
                break;
            }
        }

        for (auto& conn : connectionsVec) {
            if (conn.isConnected() && conn.is_have_data_on_socket()) {
                std::lock_guard<std::mutex> lock(stopMutex);
                Message msg = conn.read_message();
                this->outputThreadControl.addTask(msg, conn);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}
