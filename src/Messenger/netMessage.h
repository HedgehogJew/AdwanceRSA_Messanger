#ifndef NET_MESSAGE_H
#define NET_MESSAGE_H

#include <memory>
#include <thread>
#include <mutex>
#include <optional>
#include <vector>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>

#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ts/internet.hpp>

#include "HeaderID.h"

typedef struct messageHeader
{
    uint8_t id;
    uint32_t size = 0;
} MessageHeader;
 

class Message
{
private:
public:

    MessageHeader header;
    std::vector<uint8_t> body;

    Message() = default;

    Message(uint8_t id, uint32_t size, const std::vector<uint64_t>& src_vec);
    Message(uint8_t id, uint32_t size, const std::vector<uint8_t>& src_vec);
    Message(uint8_t id, uint32_t size, const std::string src_string);

    size_t size() const;
    
    friend std::ostream& operator<< (std::ostream& os, const Message msg);
};

#endif