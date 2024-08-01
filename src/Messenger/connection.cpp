#include "connection.h"

using namespace boost::multiprecision;
using namespace boost; 

Connection::Connection(boost::asio::io_context& context)
    : context_(context), socket_(context) {}

Connection::~Connection()
{
    try {
        if (socket_.is_open()) {
            socket_.close();
        }
    } catch (const boost::system::system_error& e) {
        std::cerr << "Error closing socket: " << e.what() << std::endl;
    }
}

Connection::Connection(Connection&& other) noexcept
    : context_(other.context_), socket_(std::move(other.socket_)), sessionKeyGOST(std::move(other.sessionKeyGOST)), connected_(other.connected_) {
    other.connected_ = false;
}

    // Оператор перемещения
Connection& Connection::operator=(Connection&& other) noexcept {
    if (this != &other) {
        socket_ = std::move(other.socket_);
        sessionKeyGOST = std::move(other.sessionKeyGOST);
        connected_ = other.connected_;
        other.connected_ = false;
    }
    return *this;
}

boost::asio::ip::tcp::socket& Connection::socket() {
    return socket_;
}

void Connection::setSessionKey(boost::multiprecision::cpp_int const key) {
    sessionKeyGOST = key;
}

boost::multiprecision::cpp_int Connection::getSessionKey() const {
    return sessionKeyGOST;
}

bool Connection::connectTo(const std::string& host, const std::string& port)
{
    try {
        asio::ip::tcp::resolver resolver(context_);
        asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, port);

        asio::connect(socket_, endpoints);
        connected_ = true;
        return true;
    } catch (const boost::system::system_error& e) {
        std::cerr << "Error connecting to server: " << e.what() << std::endl;
        connected_ = false;
        return false;
    }
}

bool Connection::acceptAs(const std::string& host, const std::string& port)
{
    try {
        asio::ip::tcp::acceptor acceptor(context_, asio::ip::tcp::endpoint(asio::ip::make_address(host), std::stoi(port)));

        std::cout << "Start listening on " << host << ":" << port << "." << std::endl;

        acceptor.accept(socket_);

        std::cout << "Accepted a connection from: " << socket_.remote_endpoint() << std::endl;
        connected_ = true;
        return true;
    } catch (const boost::system::system_error& e) {
        std::cerr << "Error accepting connection: " << e.what() << std::endl;
        connected_ = false;
        return false;
    }
}

bool Connection::disconnect()
{
    if (isConnected()) {
        try {
            socket_.shutdown(asio::ip::tcp::socket::shutdown_both);
            socket_.close();
            connected_ = false;
            return true;
        } catch (const boost::system::system_error& e) {
            std::cerr << "Error disconnecting: " << e.what() << std::endl;
            return false;
        }
    }
    return true;
}

bool Connection::isConnected()
{
    return socket_.is_open() && connected_;
}

bool Connection::sendMessage(const Message& message)
{
    if (!isConnected()) {
        std::cerr << "Error: Not connected to server." << std::endl;
        return false;
    }

    std::vector<boost::asio::const_buffer> buffers;
    buffers.push_back(boost::asio::buffer(&message.header, sizeof(message.header)));
    buffers.push_back(boost::asio::buffer(message.body));

    boost::system::error_code ec;
    asio::write(socket_, buffers, ec);

    if (ec) {
        std::cerr << "Error sending message: " << ec.message() << std::endl;
        connected_ = false; // Обновить состояние подключения
        return false;
    } else {
        return true;
    }
}


Message Connection::read_message() {
    try {
        MessageHeader header;

        asio::read(socket_, boost::asio::buffer(&header, sizeof(header)));

        std::vector<uint8_t> body(header.size);
        asio::read(socket_, boost::asio::buffer(body));
        
        Message received_message(header.id, header.size, body);

        return received_message;

    } catch (boost::system::system_error& e) {
        std::cerr << "Error reading message: " << e.what() << std::endl;
        return Message(ID_ERROR, 0, std::vector<uint8_t>());
    }
}

bool Connection::is_have_data_on_socket() {
    auto start_time = std::chrono::steady_clock::now();
    int timeout_seconds = 1;

    while (true) {
        boost::system::error_code ec;
        std::size_t bytes_available = socket_.available(ec);

        if (!ec) {
            if(bytes_available > 0)
                return true;
        } else {
            std::cerr << "Error waiting for data: " << ec.message() << std::endl;
            break;
        }

        auto current_time = std::chrono::steady_clock::now();
        if (std::chrono::duration_cast<std::chrono::seconds>(current_time - start_time).count() >= timeout_seconds) {
            break;
        }
    }
    return false;
}

bool operator==(const Connection& lhs, const Connection& rhs) {
    return lhs.sessionKeyGOST == rhs.sessionKeyGOST;
}


bool operator!=(const Connection& lhs, const Connection& rhs) {
    return !(lhs == rhs);
}