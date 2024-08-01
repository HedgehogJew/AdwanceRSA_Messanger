#ifndef STRINGANDSPLIT_H
#define STRINGANDSPLIT_H

#include <boost/multiprecision/cpp_int.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <limits>

boost::multiprecision::cpp_int simpleStringToNumTranslate( std::string src);
std::string simpleNumToStringTranslate( boost::multiprecision::cpp_int src);

template <typename T>
std::vector<T> splitter(boost::multiprecision::cpp_int message);

std::string uint8tVectorToCString(const std::vector<uint8_t>& data);
std::vector<uint8_t> CStringToUint8tVector(const std::string& str_data);

std::vector<uint64_t> uint8ToUint64(const std::vector<uint8_t>& input);
std::vector<uint8_t> uint64ToUint8(const std::vector<uint64_t>& input);

#endif // STRINGANDSPLIT_H
