#include "netMessage.h"

Message::Message(uint8_t id, uint32_t size, const std::vector<uint64_t>& src_vec) {
    header.id = id;
    header.size = size;

    size_t total_bytes = src_vec.size() * sizeof(uint64_t);
    body.resize(total_bytes);
    memcpy(body.data(), src_vec.data(), total_bytes);
}

Message::Message(uint8_t id, uint32_t size, const std::vector<uint8_t>& src_vec) {
    header.id = id;
    header.size = size;
    body = src_vec;
}

Message::Message(uint8_t id, uint32_t size, const std::string src_string) {
    header.id = id;
    header.size = size;
    std::vector<uint8_t> vec(src_string.size());
    memcpy(vec.data(), src_string.data(), src_string.size());
    body = vec;
}

size_t Message::size() const {
    return sizeof(header) + body.size();
}

std::ostream& operator<<(std::ostream& os, const Message msg) {
    os << "ID:" << int(msg.header.id) << " Size: " << msg.size() << std::endl;
    for (auto it : msg.body) {
        os << (char)it;
    }
    return os;
}
