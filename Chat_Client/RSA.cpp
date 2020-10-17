#include <QDebug>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "RSA.h"

RSA::RSA()
{
    initialize();
}

void RSA::initialize()
{
    srand((unsigned int)time(NULL));
    makeKey();
    qDebug("p: %llu, q: %llu, N: %llu, phi: %llu\n", p, q, N, phi);
    qDebug("N: %llu, e: %llu, d: %llu\n\n", N, e, d);
}

void RSA::makeKey()
{
    makePQ();
    N = p * q;
    phi = (p - 1) * (q - 1);
    //printf("p: %llu, q: %llu, N: %llu, phi: %llu\n", p, q, N, phi);


    e = makeCoprime(phi);
    d = extendEuclid(phi, e);

    //printf("N: %llu, e: %llu, d: %llu\n\n", N, e, d);
}

DATA RSA::encryption(DATA num)
{
    DATA prod = 1;

    for (DATA i = 0; i < e; i++)
    {
        prod *= num % N;
        prod %= N;
    }
    return prod;
}

DATA RSA::decryption(DATA num)
{
    DATA prod = 1;

    if (num == 0)   return 0;

    for (DATA i = 0; i < d; i++)
    {
        prod *= num % N;
        prod %= N;
    }

    return prod;
}

DATA RSA::decryption_fast(DATA num)
{
    DATA d_p, d_q, q_inv = 0;
    DATA m_1 = 1, m_2 = 1, m_3, h, i;

    if (num == 0)   return 0;
    d_p = d % (p - 1);
    d_q = d % (q - 1);

    for (i = 1; i < p; i++)
        if ((q * i) % p == 1)
        {
            q_inv = i;
            break;
        }

    if(q_inv == 0 || q_inv != i){
        qDebug("q_inv is zero\n");
        exit(1);
    }


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
            if ((m_1 + i * p) + m_2 > 0)
            {
                m_3 += i * p;
                break;
            }

    h = (q_inv * m_3) % p;
    return m_2 + h * q;
}

bool RSA::isPrime(DATA num)
{
    for (DATA i = 2; i <= sqrt((RDATA)num); i++)
        if (num % i == 0)   return false;
    return true;
}

void RSA::makePQ()
{
    DATA i, len = 0, r_1, r_2;
    //DATA* primeArr = (DATA*)malloc((MAX - MIN) / 2 * sizeof(DATA));
    DATA* primeArr = new DATA[(MAX - MIN) / 2];

    if (primeArr == NULL)
    {
        //printf("memory allocation error: primeArr is NULL");
        exit(1);
    }

    for (i = MIN; i < MAX; i++)
        if (isPrime(i))
        {
            primeArr[len] = i;
            len++;
        }

    r_1 = rand() % len;
    p = primeArr[r_1];
    while (1)
    {
        r_2 = rand() % len;
        if (r_1 != r_2)
        {
            q = primeArr[r_2];
            break;
        }
    }
    //free(primeArr);
    delete []primeArr;
}

DATA RSA::makeCoprime(DATA phi)
{
    DATA len = 0, t, i;

    //t = (int)pow((RDATA)phi, COP);
    t = COPC;

    for (i = 2; i <= phi; i++)
        if (returnGCD(i, phi) == 1 && ++len == t)
            break;

    return i;
}

DATA RSA::returnGCD(DATA p, DATA q)
{
    if (p == 0 || q == 0)   return p + q;
    else if (p >= q)    return returnGCD(q, p % q);
    else    return returnGCD(p, q % p);
}

DATA RSA::makeInverse()
{
    for (DATA i = 1; i < phi; i++)
        if ((e * i) % phi == 1) return i;
    //printf("Error\n");
    exit(0);
}

DATA RSA::extendEuclid(DATA r1, DATA r2)
{
    DATA r, q, s, s1 = 1, s2 = 0, t, t1 = 0, t2 = 1, tmp = r1;

    while (r2)
    {
        q = r1 / r2;
        r = r1 % r2;
        s = s1 - q * s2;
        t = t1 - q * t2;

        NEXT(r1, r2, r);
        NEXT(s1, s2, s);
        NEXT(t1, t2, t);
    }

    if (r1 == 1) //역원이 있음
    {
        if (t1 < 0) t1 += tmp;
        return t1;
    }

    //printf("not inverse");
    exit(0);
}

void RSA::encryptString(STR plane_s, STR cipher_s)
{
    for (int i = 0; i < INLEN; i++)
        cipher_s[i] = encryption(plane_s[i]);
}

void RSA::decryptString(STR cipher_s, STR plane_s)
{
    for (int i = 0; i < INLEN; i++)
        plane_s[i] = (char)decryption_fast(cipher_s[i]);
}

















