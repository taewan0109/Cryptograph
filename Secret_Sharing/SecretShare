#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "bn-signed.h"
#include "SecretShare.h"
#include "SecretShare_MF.h"


void makePoly(Poly P, DATA key, DATA prime)
{
    DATA* Z = (DATA*)calloc(to_int(prime), sizeof(DATA));
    DATA t, m, n;
    LL i;

    if (Z == NULL)  reportError("memory allocation error");
    setPoly(P);

    t = div(inc_one(prime), from_int(2)); // t = (p + 1)/2
    m = div(dec_one(prime), from_int(-2)); // m = -(p - 1)/2
    for (i = 0; i < to_int(t); i++)   Z[i] = add(m, from_int(i));


    t = div(dec_one(prime), from_int(2)); // t = (p - 1)/2
    m = div(inc_one(prime), from_int(2)); // m = (p + 1)/2
    for (i = 0; i < to_int(t); i++)
    {
        n = add(m, from_int(i));
        Z[to_int(n)] = from_int(i + 1);
    }


    for (i = 1; i < LEN; i++)   P[i].x = Z[rand() % to_int(prime)];

    P[0].x = key;
    free(Z);
}
/*void makePoly(Poly P, DATA key, DATA prime)
{
    setPoly(P);
    P[0].x = key;
    P[1].x = from_int(166);
    P[2].x = from_int(94);
}*/


bool isPrime(DATA num)
{
    DATA len = sqrt(num), tmp;
    if (num.sig)    reportError("check Prime func input is negative");
    for (DATA i = from_int(2); cmp(i, len) == -1 || cmp(i, len) == 0; inc(i))   //i = 2; i <= len;
    {
        tmp = mod(num, i);
        if (is_zero(tmp))   return false;
    }
    return true;
}


DATA makePrime(void)
{
    UL len = 0;
    DATA i, Max = from_int(MAX);
    DATA primeArr[(MAX - MIN) / 2];

    for (i = from_int(MIN); cmp(i, Max) == -1; inc(i))
        if (isPrime(i))
        {
            primeArr[len] = i;
            len++;
        }

    return primeArr[rand() % len];
}


DATA calcPoly(Poly P, DATA n)
{
    DATA sum;
    init(sum);
    for (int i = 0; i < LEN; i++)
        sum = add(sum, mul(P[i].x, pow(n, from_int(i)) ) );
    return sum;
}


void makePoint(Poly P, POINT* point)
{
    int i, j;
    DATA num[DIST], tmp;
    bool b;
    LL t;

    for (i = 0; i < DIST; i++)
    {
        while (true)
        {
            b = true;
            t = (rand() % (PNUM * 2));
            t -= PNUM;
            num[i] = from_int(t);

            for (j = 0; j < i; j++) if (cmp(num[i], num[j]) == 0)   b = false;
            if (b == false || is_zero(num[i]))  continue;

            point[i].x = num[i];
            tmp = calcPoly(P, num[i]);
            point[i].y = tmp;
            //printf("point (%lld , %lld)\n", to_int(point[i].x), to_int(point[i].y));
            break;
        }
    }
    printf("\n");
}
/*void makePoint(Poly P, POINT* point)
{
    for (int i = 0; i < DIST; i++)
    {
        point[i].x = from_int((LL)(i + 1));
        point[i].y = calcPoly(P, point[i].x);
        printf("point (%lld , %lld)\n", to_int(point[i].x), to_int(point[i].y));
    }
    printf("\n");
}*/


void makeLagPoly(Poly L, POINT* point)
{
    Poly temp, l, p;
    setPoly(temp);
    setPoly(l);
    setPoly(p);
    setPoly(L);

    for (int i = 0; i < LEN; i++)
    {
        //printf("\n────────────────────  Laglange Poly ────────────────────\n");
        //printf("make %dth Laglange basis polynomial\n\n", i+1);
        makeLagBasis(l, point, i);
        //puts("\nLaglange basis polynomial: ");
        //printCoef(l);
        mulScalar(l, point[i].y);
        simplePoly(l);
        addPoly(l, temp, L);
        simplePoly(L);
        copyPoly(temp, L);
    }

    for (int i = 0; i < LEN; i++)
        if (cmp(L[i].y, one) != 0)
        {
            puts("\n\nError Laglange Poly:");
            printCoef(L);
            reportError("polynomial isn't has integer coef");
        }
}


void makeLagBasis(Poly l, POINT* point, int n)
{
    Poly temp, sum;
    DATA t;
    setPoly(temp);
    setPoly(sum);

    sum[0].x = one;
    for (int i = 0; i < LEN; i++)
        if (i != n)
        {
            //printf("────  x_%d ────\n", i + 1);
            temp[0].x = inv_r(point[i].x);
            simpleData(&temp[0]);
            temp[1].x = one;
            //printCoef(temp);

            //puts("\nmul poly");
            mulPoly(temp, sum, l);
            //printCoef(l);

            //printf("div poly\n");
            simplePoly(l);
            t = sub(point[n].x, point[i].x);
            divScalar(l, t);
            simplePoly(l);
            //printCoef(l);

            copyPoly(sum, l);
            //puts("\n");
        }
}


void mulPoly(Poly P, Poly Q, Poly R)
{
    FracDATA tmp_f;
    setPoly(R);

    for (int i = 0; i < LEN; i++)
        for (int j = 0; j < LEN - i; j++)
        {
            tmp_f = mulFrac(P[i], Q[j]);
            R[i + j] = addFrac(R[i + j], tmp_f);
            simpleData(&R[i + j]);
        }
}


FracDATA addFrac(FracDATA A, FracDATA B)
{
    FracDATA R;
    simpleData(&A);
    simpleData(&B);    

    R.x = add(mul(A.x, B.y), mul(A.y, B.x));
    R.y = mul(A.y, B.y);
    simpleData(&R);
    return R;
}


FracDATA subFrac(FracDATA A, FracDATA B)
{
    FracDATA R;
    DATA n, m;

    simpleData(&A);
    simpleData(&B);

    n = mul(A.x, B.y);
    m = mul(A.y, B.x);

    
    R.x = sub(n, m);
    R.y = mul(A.y, B.y);
    simpleData(&R);
    return R;
}


FracDATA mulFrac(FracDATA P, FracDATA Q)
{
    FracDATA R;
    simpleData(&P);
    simpleData(&Q);
    R.x = mul(P.x, Q.x);
    R.y = mul(P.y, Q.y);

    simpleData(&R);
    return R;
}


FracDATA invFrac(FracDATA A)
{
    FracDATA B;
    copyFrac(B, A);
    simpleData(&B);
    inv(B.x);
    return B;
}


FracDATA absFrac(FracDATA A)
{
    FracDATA B;
    copyFrac(B, A);
    simpleData(&B);
    B.x = abs(B.x);
    return B;
}


void simpleData(FracDATA* P)
{
    DATA gcd, a, b;
    FracDATA Q;


    if (is_zero(P->x))
    {
        P->y = from_int(1);
        return;
    }
    //printf("%3lld/%3lld ==> ", to_int(P->x), to_int(P->y));

    a = abs(P->x);
    b = abs(P->y);
    gcd = returnGCD(a, b);
    int t = cmp(gcd, zero);
    if (t == -1)        reportError("GCD is negative\n");
    else if (t == 0)    reportError("GCD is zero\n");

    if (cmp(gcd, one) != 0)
    {
        Q.x = div(P->x, gcd);
        Q.y = div(P->y, gcd);
    }
    else
        Q = *P;
    if (cmp(P->y, zero) == -1)
    {
        inv(Q.x);
        inv(Q.y);
    }

    *P = Q;

    //printf("%3lld/%3lld\n", to_int(P->x), to_int(P->y));
    if (cmp(P->y, zero) == -1)  reportError("Simple function result is negative");
}


DATA returnGCD(DATA p, DATA q)
{
    //printf("gcd %d, %d\n", to_int(p), to_int(q));
    if (is_zero(p) || is_zero(q))   return add(p, q);
    else if (cmp(p, q) != -1)    return returnGCD(q, mod(p, q)); //p >= q
    else    return returnGCD(p, mod(q, p));
}


void printPoly(Poly P)
{
    LL p;
    if (p = to_int(P[LEN - 1].x))
        if (p > 0)  printf(" %lldx^%d", p, LEN - 1);
        else    printf(" - %lldx^%d", -p, LEN - 1);
    else    printf(" 0");

    for (int i = LEN - 2; i > 0; i--)
        if (p = to_int(P[i].x))
            if (p < 0)
                if (p != -1)    printf(" - %lldx^%d", -p, i);
                else    printf(" - x^%d", i);
            else
                if (p != 1) printf(" + %lldx^%d", p, i);
                else   printf(" + x^%d", i);
        else    printf(" + 0");

    if (p = to_int(P[0].x))
        if (p < 0)
            if (p != -1)    printf(" - %lld", -p);
            else    printf(" - 1");
        else
            if (p != 1) printf(" + %lld", p);
            else    printf(" + 1");
    else    printf(" + 0");
    printf("\n");
}


void printCoef(Poly X)
{
    printf("┌  ");
    for (int k = LEN - 1; k >= 0; k--)  printf("%-8lld  ", to_int(X[k].x));
    printf("\n└  ");
    for (int k = LEN - 1; k >= 0; k--)  printf("%-8lld  ", to_int(X[k].y));
    printf("\n");
}


void dispersion(POINT Point[DIST], DATA key)
{
    DATA prime;
    Poly P, L;

    one = from_int(1);
    zero = from_int(0);

    prime = makePrime();
    makePoly(P, key, prime);
    //printf("┌  Prime number: %lld\n└  Polynomial  : ", to_int(prime));
    //printPoly(P);
    //printf("\n");

    makePoint(P, Point);
}


void restore(Poly L, POINT point[LEN])
{
    //printf("make lag poly start..\n");
    makeLagPoly(L, point);

    //printf("\n────────────────────  Result ────────────────────\n\n");
    printf("Restore Lagrange Poly: ");
    printPoly(L);
}
