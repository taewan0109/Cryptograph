#ifndef RSA_H
#define RSA_H

#include <stdbool.h>

#define MIN 10000	//생성하는 소수의 최소 크기
#define MAX 20000	//생성하는 소수의 최대 크기
#define INLEN 50	//입력 문자열의 길이
#define COP 1.0/2.6
#define COPC 200
#define NEXT(a, b, c) {a = b; b = c;}

typedef unsigned long long DATA;
typedef long double RDATA;
typedef unsigned long long STR[INLEN];

class RSA
{
private:
    DATA N, e, d, p, q, phi;
public:
    RSA();
    void initialize();
    void makeKey();
    DATA encryption(DATA);
    DATA decryption(DATA);
    DATA decryption_fast(DATA);
    void makePQ();
    DATA makeCoprime(DATA);
    DATA makeInverse();
    DATA extendEuclid(DATA, DATA);
    DATA returnGCD(DATA, DATA);
    bool isPrime(DATA);

    void  encryptString(STR, STR);
    void  decryptString(STR, STR);
};

#endif // RSA_H
