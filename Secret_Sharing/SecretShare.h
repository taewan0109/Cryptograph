#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "bn-signed.h"
#include "SecretShare_MF.h"

/* typedefs */
typedef struct bn DATA;
typedef long long LL;
typedef unsigned long long UL;
#pragma pack(push, 1)
typedef struct {
    DATA x;
    DATA y;
} POINT;
#pragma pack(pop)
typedef struct frac {
    DATA x;
    DATA y;
}FracDATA;
typedef struct frac Poly[LEN];

/* functions */
void makePoly(Poly, DATA, DATA);
void printPoly(Poly);
void printCoef(Poly);
DATA makePrime(void);
DATA calcPoly(Poly, DATA);
void makePoint(Poly, POINT*);
void makeLagPoly(Poly, POINT*);
void makeLagBasis(Poly, POINT*, int);
void mulPoly(Poly, Poly, Poly);
FracDATA addFrac(FracDATA, FracDATA);
FracDATA subFrac(FracDATA, FracDATA);
FracDATA mulFrac(FracDATA, FracDATA);
FracDATA invFrac(FracDATA);
FracDATA absFrac(FracDATA);
//void divScalar(Poly, DATA);
DATA returnGCD(DATA, DATA);
void simpleData(FracDATA*);
bool isPrime(DATA);

void dispersion(POINT [DIST], DATA);
void restore(Poly, POINT*);

DATA one, zero;
