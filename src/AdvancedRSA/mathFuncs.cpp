#include "mathFuncs.h"

using namespace boost::multiprecision;

boost::multiprecision::cpp_int powMod(boost::multiprecision::cpp_int a, boost::multiprecision::cpp_int e, boost::multiprecision::cpp_int m) {
    cpp_int r = 1;

    while (e > 0) {
        if ((e & 1) == 0) {
            e = e >> 1;
            a = (a * a) % m;
        } else {
            e = e - 1;
            r = (r * a) % m;
        }
    }

    return r;
}

bool millerRabinPrimeTest(boost::multiprecision::cpp_int src, size_t rounds) {
    if (src < 2) {
        return false;
    }
    if (src != 2 && src % 2 == 0) {
        return false;
    }

    generator_type gen;

    for (size_t i = 0; i < rounds; i++) {
        cpp_int temp = gen() % (src - 2) + 2;

        cpp_int t = 1;
        cpp_int s = 0;
        cpp_int decompositionTemp = src - 1;

        while (decompositionTemp % 2 == 0) {
            s++;
            decompositionTemp /= 2;
        }
        t = decompositionTemp;

        cpp_int x = powMod(temp, t, src);

        if (x == 1 || x == src - 1) {
            for (size_t j = 0; j < s - 1; j++) {
                x = powMod(x, 2, src);
                if (x == 1) {
                    return false;
                }
                if (x == src - 1) {
                    break;
                }
            }
        } else {
            return false;
        }
    }
    return true;
}

boost::multiprecision::cpp_int eulerFunc(boost::multiprecision::cpp_int p1, boost::multiprecision::cpp_int p2) {
    return (p1 - 1) * (p2 - 1);
}

boost::multiprecision::cpp_int extended_gcd(boost::multiprecision::cpp_int a, boost::multiprecision::cpp_int b, 
                                            boost::multiprecision::cpp_int &x, boost::multiprecision::cpp_int &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }

    cpp_int x1, y1;
    cpp_int gcd = extended_gcd(b % a, a, x1, y1);

    x = y1 - (b / a) * x1;
    y = x1;

    return gcd;
}

boost::multiprecision::cpp_int modInverse(boost::multiprecision::cpp_int e, boost::multiprecision::cpp_int phi_n) {
    cpp_int x, y;
    cpp_int g = extended_gcd(e, phi_n, x, y);
    if (g != 1) {
        std::cout << "Inverse does not exist." << std::endl;
        return 0;
    } else {
        return (x % phi_n + phi_n) % phi_n;
    }
}
