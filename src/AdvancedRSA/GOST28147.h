#ifndef GOST28147_H
#define GOST28147_H

#include <cstdlib>
#include <utility>
#include <string>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>

#include "stringAndSplit.h"

class GOSTWizard
{
private:
    
    std::vector<uint8_t> SBlock;

    uint32_t runForwardWithSBlock(size_t round, uint32_t target, const std::vector<uint8_t>& SBlock);
    uint32_t getKey(boost::multiprecision::cpp_int key, size_t round);
    std::vector<uint8_t> makeSBlock();
    uint32_t cyclicShiftLeft(uint32_t value, unsigned int shift);


public:
    
    GOSTWizard();
    boost::multiprecision::cpp_int generateSessionKey();
    uint64_t encrypt(uint64_t message, boost::multiprecision::cpp_int key256b);
    std::vector<uint8_t> encrypt(std::string message, boost::multiprecision::cpp_int key256b);
    
    uint64_t decrypt(uint64_t message, boost::multiprecision::cpp_int key256b);
    std::string decrypt(std::vector<uint8_t> message, boost::multiprecision::cpp_int key256b);
    
};











#endif