#include <stdbool.h>

#define MIN 30000	//생성하는 소수의 최소 크기
#define MAX 40000	//생성하는 소수의 최대 크기
#define INLEN 50	//입력 문자열의 길이
#define COP 1.0/3
#define COPF 50000
#define COPC 300
#define NEXT(a, b, c) {a = b; b = c;}

typedef unsigned long long DATA;
typedef long double RDATA;

void makeKey(DATA*, DATA*, DATA*, DATA*, DATA*);
DATA encryption(DATA, DATA, DATA);
DATA decryption(DATA, DATA, DATA);
DATA decryption_fast(DATA, DATA, DATA, DATA, DATA);
void makePQ(DATA*, DATA*);
DATA makeCoprime(DATA);
DATA makeInverse(DATA, DATA);
DATA extendEuclid(DATA, DATA);
DATA returnGCD(DATA, DATA);
bool isPrime(DATA);
