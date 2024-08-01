#ifndef RSAWIZARD_H
#define RSAWIZARD_H

#include "mathFuncs.h"
#include "stringAndSplit.h"

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <cstdlib>
#include <utility>
#include <string>

#define OPENEXP 65537

//Раунды проверки простоты числа рабина прайма
#define MRPTROUNDS 1000

//using namespace boost::multiprecision;
//using namespace boost::random;

typedef boost::random::independent_bits_engine<boost::random::mt19937, 3072 , boost::multiprecision::cpp_int> generator_type;

class RSAWizard {
private:
    boost::multiprecision::cpp_int prime_1;
    boost::multiprecision::cpp_int prime_2;

    /*Closed key paramenters*/
    boost::multiprecision::cpp_int closedExp;

    /*Open key paramenters*/
    boost::multiprecision::cpp_int primeMlpProduct;
    boost::multiprecision::cpp_int openExp;

   
    void calculateClosedExp();

public:
    RSAWizard() : prime_1(0), prime_2(0), closedExp(0), primeMlpProduct(0), openExp(OPENEXP) {}

    void printWizardParams();
    void generatePrimeNumbers();
    void prepareKeys();
    void setOpenKeys(boost::multiprecision::cpp_int e, boost::multiprecision::cpp_int n);
    void setOpenKeys(std::pair<boost::multiprecision::cpp_int, boost::multiprecision::cpp_int> keyPair);
    std::pair<boost::multiprecision::cpp_int, boost::multiprecision::cpp_int> getOpenKeys();
    boost::multiprecision::cpp_int encrypt(boost::multiprecision::cpp_int message);
    boost::multiprecision::cpp_int decrypt(boost::multiprecision::cpp_int message);
};


#endif