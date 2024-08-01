#include "GOST28147.h"

using namespace boost::multiprecision;
using namespace boost::random;



uint32_t GOSTWizard::runForwardWithSBlock(size_t round, uint32_t target, const std::vector<uint8_t>& SBlock) {
    // Выбираем блок таблицы для кодирования

    size_t lineNum = round % 8;
    auto startIT = SBlock.begin() + lineNum * 16;
    std::vector<uint8_t> tempVec(startIT, startIT + 16);

    uint32_t res = 0;

    for (size_t i = 0; i < 8; i++) {
        // Извлечение 4-битного полубайта
        uint8_t temp = (target >> (4 * (7 - i))) & 0xF; // Сдвиг и маска для выделения 4-битного полубайта
        res = (res << 4) | tempVec[temp]; // Добавление зашифрованного значения из S-блока
    }

    return res;
}

//Фиксированный размер ключа 256 бит
uint32_t GOSTWizard::getKey(boost::multiprecision::cpp_int key, size_t round)
{
    boost::multiprecision::cpp_int temp = key >> (round % 8) * 32;
    return (uint32_t)(temp & 0xFFFFFFFF);
}

std::vector<uint8_t> GOSTWizard::makeSBlock()
{
    /*Идентификатор: id-Gost28147-89-CryptoPro-A-ParamSet*/
    std::vector<uint8_t> res = {9,6,3,2,8,11,1,7,10,4,14,15,12,0,13,5,
                            3,7,14,9,8,10,15,0,5,2,6,12,11,4,13,1,
                            14,4,6,2,11,3,13,8,12,15,5,10,0,7,1,9,
                            14,7,10,12,13,1,3,9,0,2,11,4,15,8,5,6,
                            11,5,1,9,8,13,15,0,14,4,2,3,12,7,10,6,
                            3,10,13,12,1,2,0,11,7,5,9,4,8,15,14,6,
                            1,13,2,9,7,10,6,0,8,12,4,5,15,3,11,14,
                            11,10,15,5,0,12,14,8,6,2,3,9,1,7,13,4};

/*
    std::vector<uint8_t> res = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
                            0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,};
 */   
    this->SBlock = res;
    return res;
}

uint32_t GOSTWizard::cyclicShiftLeft(uint32_t value, unsigned int shift) {
    shift = shift % 32; // Циклический сдвиг на 32 бита возвращает исходное значение
    return (value << shift) | (value >> (32 - shift));
}

    
GOSTWizard::GOSTWizard()
{
    makeSBlock();
}

uint64_t GOSTWizard::encrypt(uint64_t message, boost::multiprecision::cpp_int key256b)
{
    uint32_t driveL = (message >> 32) & 0xFFFFFFFF ; // левые 32 бита
    uint32_t driveR = message & 0xFFFFFFFF; // правые 32 бита   
    for(int i = 0; i < 24; i++)
    {
        uint32_t funcRes = (driveR + getKey(key256b, i)) & 0xFFFFFFFF; //Сложение по модулю 2^32
        funcRes = runForwardWithSBlock(i, funcRes, SBlock); //Шифровка по таблице
        funcRes = cyclicShiftLeft(funcRes, 11); //сдвиг на 11 бит влево
        uint32_t temp = driveR;
        
        driveR = driveL ^ funcRes; //XOR
        driveL = temp;
    }
    for(int i = 24; i < 32; i++)
    {
        uint32_t funcRes = (driveR + getKey(key256b, 31 - i))  & 0xFFFFFFFF; //Сложение по модулю 2^32
        funcRes = runForwardWithSBlock(31 - i, funcRes, SBlock); //Шифровка по таблице
        funcRes = cyclicShiftLeft(funcRes, 11); //сдвиг на 11 бит влево
        uint32_t temp = driveR;
        
        driveR = driveL ^ funcRes; //XOR
        driveL = temp;
    }
    uint64_t res = driveL & 0xFFFFFFFF;
    res = res << 32;
    res += driveR & 0xFFFFFFFF;
    return res;
}
 
std::vector<uint8_t> GOSTWizard::encrypt(std::string message, boost::multiprecision::cpp_int key256b)
{
    std::vector<uint64_t> splitedMessage = splitter<uint64_t>(simpleStringToNumTranslate(message));
    std::vector<uint64_t> encryptedMessage;
 
    for (uint64_t block : splitedMessage) {
        encryptedMessage.push_back(encrypt(block, key256b));
    }   

    std::reverse(encryptedMessage.begin(), encryptedMessage.end());

    std::vector<uint8_t> result = uint64ToUint8(encryptedMessage);
    return result;
}

uint64_t GOSTWizard::decrypt(uint64_t message, boost::multiprecision::cpp_int key256b)
{
    uint32_t driveL = (message >> 32) & 0xFFFFFFFF; // левые 32 бита
    uint32_t driveR = message & 0xFFFFFFFF; // правые 32 бита
    uint32_t temp = driveL;
    driveL = driveR;
    driveR = temp;
   
    for(int i = 0; i < 8; i++)
    {
        uint32_t funcRes = (driveR + getKey(key256b, i)) & 0xFFFFFFFF; //Сложение по модулю 2^32
        funcRes = runForwardWithSBlock(i, funcRes, makeSBlock()); //Шифровка по таблице
        funcRes = cyclicShiftLeft(funcRes, 11); //сдвиг на 11 бит влево
        uint32_t temp = driveR;
        driveR = driveL ^ funcRes; //XOR
        driveL = temp;
    }
    for(int i = 8; i < 32; i++)
    {
        uint32_t funcRes = (driveR + getKey(key256b, 31 - i)) & 0xFFFFFFFF; //Сложение по модулю 2^32
        funcRes = runForwardWithSBlock(31 - i, funcRes, makeSBlock()); //Шифровка по таблице
        funcRes = cyclicShiftLeft(funcRes, 11); //сдвиг на 11 бит влево
        uint32_t temp = driveR;
        driveR = driveL ^ funcRes; //XOR
        driveL = temp;
    }
    uint64_t res = driveR & 0xFFFFFFFF;
    res = res << 32;
    res += driveL & 0xFFFFFFFF;
    return res;
}

std::string GOSTWizard::decrypt(std::vector<uint8_t> message, boost::multiprecision::cpp_int key256b)
{
    std::vector<uint64_t> splitedMessage = uint8ToUint64(message);
    std::vector<uint64_t> uint64DecryptedVec;
    for(auto& it : splitedMessage) {
        uint64DecryptedVec.push_back(decrypt(it, key256b));
    }

    std::vector<uint8_t> uint8tDecryptedVec = uint64ToUint8(uint64DecryptedVec);
    std::string res (uint8tDecryptedVec.begin(), uint8tDecryptedVec.end());

    std::reverse(res.begin(), res.end());
    return res;
}

typedef boost::random::independent_bits_engine<boost::random::mt19937, 256 , boost::multiprecision::cpp_int> generator_type;

boost::multiprecision::cpp_int GOSTWizard::generateSessionKey()
{
    std::time_t now = std::time(0);
    boost::random::mt19937 gen(static_cast<unsigned long>(now));
    generator_type engine(gen);
    return engine();
}