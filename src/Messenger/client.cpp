#include "client.h"

bool Client::ConnectTo(const std::string& host, const std::string& port)
{
    if (!clientConnection.isConnected())
    {
        try
        {
            clientConnection.connectTo(host, port);
            std::cout << "Client connected to server\n";

            return true;
        }
        catch (const boost::system::system_error& ec)
        {
            std::cerr << "Client : " << ec.what() << std::endl;
            return false;
        }
    }
    else {
        std::cout << "Client. Connection already exist" << std::endl;
        return false;
    }
}

bool Client::serverHandshake()
{
    if (clientConnection.isConnected())
    {
        try
        {
            clientConnection.sendMessage(Message(ID_HANDSHAKE_OPEN_KEY_REQUEST, 0, ""));

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            Message msgFromServer = clientConnection.read_message();

            if (msgFromServer.header.id == ID_HANDSHAKE_OPEN_KEY_RESPONSE)
            {
                std::string str(msgFromServer.body.begin(), msgFromServer.body.end());
                cpp_int e, n;
                std::istringstream iss(str);
                iss >> e >> n;
                rsaWizard.setOpenKeys(e, n);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(20));

            sessionKey = gostWizard.generateSessionKey();

            std::stringstream ss;
            ss << rsaWizard.encrypt(sessionKey);
            std::string sessionKeyStr = ss.str();

            clientConnection.sendMessage(Message(ID_HANDSHAKE_SESSION_KEY_RESPONSE, sessionKeyStr.size(), sessionKeyStr));
            std::cout << "RSA-secured connection is established\n";
            return true;
        }
        catch (const boost::system::system_error& ec)
        {
            std::cerr << "Client : " << ec.what() << std::endl;
            return false;
        }
    }
    else {
        std::cout << "Client. Connection failed" << std::endl;
        return false;
    }

}

bool Client::sendString(const std::string& src)
{
    if (clientConnection.isConnected())
    {
        try
        {
            std::vector<uint8_t> message = gostWizard.encrypt(src, sessionKey);
            Message res = Message(ID_SEND_MESSAGE_TO_SERVER, message.size(), message);
            clientConnection.sendMessage(res);
            return true;
        }
        catch (const boost::system::system_error& ec)
        {
            std::cerr << "Client : " << ec.what() << std::endl;
            return false;
        }
    }
    else {
        std::cout << "Client. Connection failed" << std::endl;
        return false;
    }
}

bool Client::sendUserInput()
{
    std::string userInput;
    std::getline(std::cin, userInput);

    if (userInput.size() > 0)
    {
        if (sendString(userInput))
            return true;
    }

    return false;
}

bool Client::getMessageFromServer()
{
    Message msgFromServer = clientConnection.read_message();

    if (msgFromServer.header.id == ID_SEND_MESSAGE_TO_CLIENT)
    {
        std::lock_guard<std::mutex> lock(stopMutex);

        std::string decryptedStr = gostWizard.decrypt(msgFromServer.body, sessionKey);

        std::cout << "::" << decryptedStr << std::endl;
        return true;
    }
    return false;
}

void Client::waitAndPrintMessages()
{
    while (true)
    {
        {
            std::lock_guard<std::mutex> lock(stopMutex);
            if (stopFlag) {
                break;
            }
        }

        if (clientConnection.isConnected() && clientConnection.is_have_data_on_socket())
        {
            getMessageFromServer();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Client::run()
{
    if (ConnectTo("127.0.0.1", "8888")) {
        if (serverHandshake())
        {
            std::thread inputThread = std::thread(&Client::waitAndPrintMessages, this);
            //std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            //std::this_thread::sleep_for(std::chrono::milliseconds(3000));

            while (true)
            {
                sendUserInput();
            }
        }
    }
}

int main()
{
    Client c;
    c.run();
}