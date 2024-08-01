#ifndef MATHFUNCS_H
#define MATHFUNCS_H

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/random.hpp>
#include <iostream>

typedef boost::random::independent_bits_engine<boost::random::mt19937, 3072, boost::multiprecision::cpp_int> generator_type;

/*Алгоритм монтгоммери возведения в степень по модулю*/
boost::multiprecision::cpp_int powMod(boost::multiprecision::cpp_int a, boost::multiprecision::cpp_int e, boost::multiprecision::cpp_int m);

bool millerRabinPrimeTest(boost::multiprecision::cpp_int src, size_t rounds);

boost::multiprecision::cpp_int eulerFunc(boost::multiprecision::cpp_int p1, boost::multiprecision::cpp_int p2);

boost::multiprecision::cpp_int extended_gcd(boost::multiprecision::cpp_int a, boost::multiprecision::cpp_int b, 
                                            boost::multiprecision::cpp_int &x, boost::multiprecision::cpp_int &y);

// Функция для вычисления мультипликативного обратного числа
boost::multiprecision::cpp_int modInverse(boost::multiprecision::cpp_int e, boost::multiprecision::cpp_int phi_n);

#endif // MATHFUNCS_H
