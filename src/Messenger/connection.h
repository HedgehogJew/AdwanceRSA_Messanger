#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <thread>
#include <mutex>
#include <optional>
#include <vector>
#include <iostream>
#include <chrono>
#include <cstdint>
#include <boost/asio.hpp>
#include <boost/asio/ts/buffer.hpp>
#include <boost/asio/ts/internet.hpp>
#include <boost/multiprecision/cpp_int.hpp>

#include "netMessage.h"

class Connection
{
private:

    boost::asio::io_context& context_;
    boost::asio::ip::tcp::socket socket_;
    boost::multiprecision::cpp_int sessionKeyGOST;
    bool connected_ = false;

public:
    Connection(boost::asio::io_context& context);
    ~Connection();

    // Конструктор перемещения
    Connection(Connection&& other) noexcept;
    // Оператор перемещения
    Connection& operator=(Connection&& other) noexcept;

    void setSessionKey(boost::multiprecision::cpp_int const key);
    boost::multiprecision::cpp_int getSessionKey() const;
    boost::asio::ip::tcp::socket& socket();

    bool connectTo(const std::string& host, const std::string& port);

    //Метод ожидания подключения к сокету
    bool acceptAs(const std::string& host, const std::string& port);
    bool disconnect();
    bool isConnected();

    bool sendMessage(const Message& message);

    Message read_message();

    //Проверка наличия новых данных на сокете
    bool is_have_data_on_socket();

    friend bool operator==(const Connection& lhs, const Connection& rhs);
    friend bool operator!=(const Connection& lhs, const Connection& rhs);
};

#endif