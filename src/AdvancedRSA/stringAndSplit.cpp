#include "stringAndSplit.h"

using namespace boost::multiprecision;


boost::multiprecision::cpp_int simpleStringToNumTranslate(std::string src) 
{
    cpp_int res;
    for(size_t i = 0; i < src.length(); i++)
    {
        res *= 256;
        res += (uint8_t)src[i];
    }

    return res;
}

std::string simpleNumToStringTranslate(boost::multiprecision::cpp_int src)
{
    std::string res; 
    cpp_int num = src;

    char temp = 0;

    while(num != 0)
    {
        temp = (char) (num % 256);
        res.insert(res.begin(),temp);
            
        num /= 256;
    }
    return res;
}


/*
std::string simpleNumToStringTranslate(boost::multiprecision::cpp_int src) {
    std::stringstream ss;
    ss << src;
    return ss.str();
}

boost::multiprecision::cpp_int simpleStringToNumTranslate(std::string src) {
    boost::multiprecision::cpp_int result;
    
    try {
        result = boost::multiprecision::cpp_int(src);
    } catch (const std::runtime_error& e) {
        throw;
    }
    
    return result;
}
*/
template <typename T>
std::vector<T> splitter(boost::multiprecision::cpp_int message) {
    // Проверяем, что размер T не превышает размер uint64_t
    if (sizeof(T) > sizeof(uint64_t)) {
        std::cout << "Bad input\n";
        return std::vector<T>{0};
    }

    std::vector<T> res;
    cpp_int temp(message);

    uint64_t mask = std::numeric_limits<T>::max();
    
    while (temp > 0) {
        res.push_back(static_cast<T>(temp & mask));
        temp >>= (sizeof(T) * 8);
    }
    // Переворачиваем вектор, чтобы сохранить порядок байтов, как в исходном числе
    std::reverse(res.begin(), res.end());
    return res;
}


// Explicit instantiation of template functions
template std::vector<uint8_t> splitter<uint8_t>(boost::multiprecision::cpp_int message);
template std::vector<uint16_t> splitter<uint16_t>(boost::multiprecision::cpp_int message);
template std::vector<uint32_t> splitter<uint32_t>(boost::multiprecision::cpp_int message);
template std::vector<uint64_t> splitter<uint64_t>(boost::multiprecision::cpp_int message);

std::string uint8tVectorToCString(const std::vector<uint8_t>& data) {
    size_t size = data.size() * sizeof(uint8_t);

    std::string str_data;
    str_data.resize(size);

    memcpy(&str_data[0], data.data(), size);

    return str_data;
}

std::vector<uint8_t> CStringToUint8tVector(const std::string& str_data) {
   
    size_t size = str_data.size() / sizeof(uint8_t);

    std::vector<uint8_t> data;
    data.resize(size);

    memcpy(data.data(), str_data.data(), size * sizeof(uint8_t));

    return data;
}


///////////////////////////////////////////////////

std::vector<uint64_t> uint8ToUint64(const std::vector<uint8_t>& input) {
    std::vector<uint64_t> output;
    uint64_t temp = 0;
    size_t numBlocks = input.size() / sizeof(uint64_t);
    size_t remainder = input.size() % sizeof(uint64_t);

    for (size_t i = 0; i < numBlocks; ++i) {
        std::memcpy(&temp, &input[i * sizeof(uint64_t)], sizeof(uint64_t));
        output.push_back(temp);
    }

    if (remainder > 0) {
        temp = 0;
        std::memcpy(&temp, &input[numBlocks * sizeof(uint64_t)], remainder);
        output.push_back(temp);
    }

    return output;
}

std::vector<uint8_t> uint64ToUint8(const std::vector<uint64_t>& input) {
    std::vector<uint8_t> res;
    res.resize(input.size()*sizeof(uint64_t));

    memcpy(&res[0], &input[0], res.size());
    return res;
}