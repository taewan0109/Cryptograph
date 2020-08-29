#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define MIN 900
#define MAX 1000
#define INLEN 20

typedef unsigned long long DATA;
typedef long double RDATA;

void makeKey(DATA*, DATA*, DATA*, DATA*, DATA*);
DATA encryption(DATA, DATA, DATA);
DATA decryption(DATA, DATA, DATA);
DATA decryption_fast(DATA, DATA, DATA, DATA, DATA);
void makePQ(DATA*, DATA*);
DATA makeCoprime(DATA);
DATA makeInverse(DATA, DATA);
DATA returnGCD(DATA, DATA);
bool isPrime(DATA);

int main(void)
{
    DATA N, e, d, p, q, plane, cipher, decrypted, decrypted_fast;
    DATA plane_s[INLEN] = { 0, }, cipher_s[INLEN] = { 0, };
    unsigned char str[INLEN] = { 0, }, decrypted_s[INLEN] = { 0, }, decrypted_fast_s[INLEN] = { 0, };
    int i;

    srand((unsigned int)time(NULL));
    makeKey(&N, &e, &d, &p, &q);

    printf("암호화할 문자열을 입력해 주세요(%d자 이내): ", INLEN);
    gets(str);

    printf("\n평문: %s\n\n", str);

    printf("10진수: ");
    for (i = 0; i < INLEN; i++)
    {
        printf("%d ", str[i]);
        plane_s[i] = (DATA)str[i];
        cipher_s[i] = encryption(N, e, plane_s[i]);
        decrypted_s[i] = (char)decryption(N, d, cipher_s[i]);
        decrypted_fast_s[i] = (char)decryption_fast(N, d, cipher_s[i], p, q);
    }
    
    printf("\n\n암호문: ");
    for (i = 0; i < INLEN; i++)
        printf("%lld ", cipher_s[i]);

    printf("\n\n복호문: %s\n", decrypted_fast_s);
    
    /*plane = 100000;
    cipher = encryption(N, e, plane);
    decrypted = decryption(N, d, cipher);
    decrypted_fast = decryption_fast(N, d, cipher, p, q);

    printf("plane text: %lld\n", plane);
    printf("cipher text: %lld\n", cipher);
    printf("decrypted text: %lld\n", decrypted);
    printf("decrypted text(fast): %lld\n", decrypted_fast);*/

    return 0;
}

void makeKey(DATA* N, DATA* e, DATA* d, DATA* p, DATA* q)
{
    DATA phi;

    makePQ(p, q);
    *N = (*p) * (*q);
    phi = (*p - 1) * (*q - 1);
    printf("p: %lld, q: %lld, N: %lld, phi: %lld\n", *p, *q, *N, phi);

    *e = makeCoprime(phi);
    *d = makeInverse(*e, phi);
    printf("N: %lld, e: %lld, d: %lld\n\n", *N, *e, *d);
}

DATA encryption(DATA N, DATA e, DATA num)
{
    DATA prod = 1;

    for (DATA i = 0; i < e; i++)
    {
        prod *= num % N;
        prod %= N;
    }
    return prod;
}

DATA decryption(DATA N, DATA d, DATA num)
{
    DATA prod = 1;

    for (DATA i = 0; i < d; i++)
    {
        prod *= num % N;
        prod %= N;
    }

    return prod;
}

DATA decryption_fast(DATA N, DATA d, DATA num, DATA p, DATA q)
{
    DATA d_p, d_q, q_inv;
    DATA m_1 = 1, m_2 = 1, m_3, h, i;

    d_p = d % (p - 1);
    d_q = d % (q - 1);

    for (i = 1; i < p; i++)
        if ((q * i) % p == 1)
        {
            q_inv = i;
            break;
        }
    if (q_inv != i) exit(0);


    for (i = 0; i < d_p; i++) {
        m_1 *= num % p;
        m_1 %= p;
    }

    for (i = 0; i < d_q; i++) {
        m_2 *= num % q;
        m_2 %= q;
    }

    m_3 = (m_1 - m_2);
    if (m_1 < m_2)
        for (i = 1; ; i++)
            if ((m_1 + i * p) +m_2 > 0)
            {
                m_3 += i * p;
                break;
            }

    h = (q_inv * m_3) % p;
    return m_2 + h * q;
}

bool isPrime(DATA num)
{
    for (DATA i = 2; i <= sqrt((RDATA)num); i++)
        if (num % i == 0)
            return false;

    return true;
}

void makePQ(DATA* p, DATA* q)
{
    DATA i, len = 0, r_1, r_2;
    DATA primeArr[(MAX - MIN) / 2] = { 0, };

    for (i = MIN; i < MAX; i++)
        if (isPrime(i))
        {
            primeArr[len] = i;
            len++;
        }

    r_1 = rand() % len;
    *p = primeArr[r_1];
    while (1) {
        r_2 = rand() % len;
        if (r_1 != r_2) {
            *q = primeArr[r_2];
            break;
        }
    }

}

DATA makeCoprime(DATA phi)
{
    DATA* copArr = (DATA*)malloc(phi * sizeof(DATA));
    DATA len = 0, p;

    if (copArr == NULL)
    {
        printf("memory allocation error");
        exit(0);
    }

    for (DATA i = 2; i <= phi; i++)
        if (returnGCD(i, phi) == 1)
        {
            copArr[len - 1] = i;
            len++;
        }

    p = (DATA)pow((RDATA)phi, 1.0 / 4.0);
    return copArr[rand() % p];
}

DATA returnGCD(DATA p, DATA q)
{
    if (p == 0 || q == 0)   return p + q;
    else if (p >= q)    return returnGCD(q, p % q);
    else    return returnGCD(p, q % p);
}

DATA makeInverse(DATA e, DATA phi)
{
    for (DATA i = 1; i < phi; i++)
        if ((e * i) % phi == 1)
            return i;

    printf("Error\n");
    exit(0);
}
