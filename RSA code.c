#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define MIN 9500
#define MAX 10000
#define INLEN 30
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
clock_t start, end;

int main(void)
{
    DATA N, e, d, p, q;
    DATA plane_s[INLEN] = { 0, }, cipher_s[INLEN] = { 0, };
    unsigned char str[INLEN] = { 0, }, decrypted_s[INLEN] = { 0, }, decrypted_fast_s[INLEN] = { 0, };
    int i;

    printf("Program Start...\n\n");
    srand((unsigned int)time(NULL));
    makeKey(&N, &e, &d, &p, &q);


    printf("암호화할 문자열을 입력해 주세요(%d자 이내): ", INLEN);
    gets(str);

    printf("\n평문: %s\n\n", str);

    printf("10진수: ");

    for (i = 0; i < INLEN; i++)
    {
        printf("%d ", str[i]);
        //plane_s[i] = (DATA)str[i];
        cipher_s[i] = encryption(N, e, (DATA)str[i]);
        //start = clock();
        decrypted_fast_s[i] = (char)decryption_fast(N, d, cipher_s[i], p, q);
        //end = clock();
        //printf("fast: %f\n", (float)(end - start) / CLOCKS_PER_SEC);
    }
    
    printf("\n\n암호문: ");
    for (i = 0; i < INLEN; i++)
        printf("%lld ", cipher_s[i]);

    printf("\n\n복호화: ");
    for (i = 0; i < INLEN; i++)
        printf("%d ", decrypted_fast_s[i]);

    printf("\n\n복호문: %s\n", decrypted_fast_s);

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
    //*d = makeInverse(*e, phi);
    *d = extendEuclid(phi, *e);

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

    if (num == 0)   return 0;

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

    if (num == 0)   return 0;
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
    DATA* primeArr = (DATA*)malloc((MAX-MIN)/2 * sizeof(DATA));

    if (primeArr == NULL)
    {
        printf("memory allocation error: primeArr is NULL");
        exit(0);
    }

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
    free(primeArr);
}

DATA makeCoprime(DATA phi)
{
    //DATA* copArr = (DATA*)malloc(sizeof(DATA));
    DATA len = 0, p, ret, i;

    /*if (copArr == NULL)
    {
        printf("%d\n", phi);
        printf("memory allocation error: copArr is NULL");
        exit(0);
    }*/

    p = (int)pow((RDATA)phi, 1.0 / 3.0);

    for (i = 2; i <= phi; i++)
        if (returnGCD(i, phi) == 1)
        {
            //copArr = (DATA*)realloc(copArr, (len+1)*sizeof(DATA));
            //copArr[len] = i;
            len++;
            if (len == p)   break;
        }
    
    ret = i;
    //ret = copArr[rand() % p];
    //free(copArr);
    return ret;
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

DATA extendEuclid(DATA r1, DATA r2)
{
    int r, q, s, s1 = 1, s2 = 0, t, t1 = 0, t2 = 1, tmp = r1;

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
        if (t1 < 0)
            t1 += tmp;
        return t1;
    }

    printf("not inverse");
    exit(0);
}
