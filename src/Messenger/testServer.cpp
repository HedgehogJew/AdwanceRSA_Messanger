#include <iostream>
#include <ctime>
#include <boost/asio.hpp>
#include <string>

#include "netMessage.h"
#include "connection.h"
#include "InputThreadCtrl.h"
#include "ConnectionThreadCtrl.h"
#include "OutputThreadCtrl.h"
    
using namespace boost::asio;
using namespace boost::asio::ip;
using namespace std;

int main() {
    try {
    boost::asio::io_context io_context;
    std::vector<Connection> conVec;

    OutputThreadCtrl& outputCtrl = OutputThreadCtrl::getInstance(conVec, ID_SERVER);
    outputCtrl.initCryptographi(0);
    InputThreadCtrl& inputCtrl = InputThreadCtrl::getInstance(outputCtrl, conVec, ID_SERVER);
    ConnectionThreadCtrl& connectionCtrl = ConnectionThreadCtrl::getInstance(io_context, conVec, __IP__, __PORT__, ID_SERVER);

    while(true);

} catch (boost::system::system_error& e) {
    std::cerr << "Error: " << e.what() << std::endl;
}
    return 0;
}
