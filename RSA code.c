#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define MIN 400
#define MAX 500

void makeKey(int*, int*, int*, int*, int*);
int encryption(int, int, int);
int decryption(int, int, int);
int decryption_fast(int, int, int, int, int);
void makePQ(int *, int *);
int makeCoprime(int);
int makeInverse(int, int);
int returnGCD(int, int);
bool isPrime(int);

int main(void)
{
    int N, e, d, p, q, plane, cipher, decrypted, decrypted_fast;

    srand((unsigned int)time(NULL));
    makeKey(&N, &e, &d, &p, &q);

    plane = 100;
    cipher = encryption(N, e, plane);
    decrypted = decryption(N, d, cipher);
    decrypted_fast = decryption_fast(N, d, cipher, p, q);

    printf("plane text: %d\n", plane);
    printf("cipher text: %d\n", cipher);
    printf("decrypted text: %d\n", decrypted);
    printf("decrypted text(fast): %d\n", decrypted_fast);

	return 0;
}

void makeKey(int* N, int* e, int* d, int* p, int* q)
{
    //int p, q, phi;
    int phi;

    makePQ(p, q);
    *N = (*p) * (*q);
    phi = (*p - 1) * (*q - 1);
    printf("p: %d, q: %d, N: %d, phi: %d\n", *p, *q, *N, phi);

    *e = makeCoprime(phi);
    *d = makeInverse(*e, phi);
    printf("N: %d, e: %d, d: %d\n\n", *N, *e, *d);
}

int encryption(int N, int e, int num)
{
    int prod = 1;

    for (int i = 0; i < e; i++)
    {
        prod *= num % N;
        prod %= N;
    }
    return prod;
}

int decryption(int N, int d, int num)
{
    int prod = 1;

    for (int i = 0; i < d; i++)
    {
        prod *= num % N;
        prod %= N;
    }

    return prod;
}

int decryption_fast(int N, int d, int num, int p, int q)
{
    int d_p, d_q, q_inv;
    int m_1 = 1, m_2 = 1, m_3, h, i;

    d_p = d % (p - 1);
    d_q = d % (q - 1);

    for (i = 1; i < p; i++)
        if ((q * i) % p == 1)   q_inv = i;

    for (i = 0; i < d_p; i++){
        m_1 *= num % p;
        m_1 %= p;
    }

    for (i = 0; i < d_q; i++){
        m_2 *= num % q;
        m_2 %= q;
    }

    m_3 = (m_1 - m_2);
    if (m_3 < 0)
        for (i = 1; ; i++)
            if (m_3 + i * p > 0)
            {
                m_3 += i * p;
                break;
            }
    
    h = (q_inv * m_3) % p;
    return m_2 + h * q;
}

bool isPrime(int num)
{
    for (int i = 2; i <= sqrt((double)num); i++)
        if (num % i == 0)
            return false;

    return true;
}

void makePQ(int * p, int * q)
{
    int i, len = 0;
    int primeArr[(MAX - MIN) / 2] = { 0, };

    for (i = MIN; i < MAX; i++)
        if (isPrime(i))
        {
            primeArr[len] = i;
            len++;
        }
    
    *p = primeArr[rand() % len];
    *q = primeArr[rand() % len];
}

int makeCoprime(int phi)
{
    int* copArr = (int *)malloc(phi * sizeof(int));
    int len = 0, p;

    if (copArr == NULL)
    {
        printf("memory allocation error");
        exit(0);
    }

    for (int i = 2; i <= phi; i++)
        if (returnGCD(i, phi) == 1)
        {
            copArr[len - 1] = i;
            len++;
        }

    p = (int)pow((double)phi, 1.0/4.0);
    return copArr[rand() % p];
}

int returnGCD(int p, int q)
{
    if (p == 0 || q == 0)   return p + q;
    else if (p >= q)    return returnGCD(q, p % q);
    else    return returnGCD(p, q % p);
}

int makeInverse(int e, int phi)
{
    for (int i = 1; i < phi; i++)
        if ((e * i) % phi == 1)
            return i;

    printf("Error\n");
    exit(0);
}
