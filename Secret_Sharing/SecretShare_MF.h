#include <stdio.h>
#include "bn-signed.h"

#define MIN 80
#define MAX 150
#define LEN 7 //이 인원 이상이 모이면 복원가능
#define DIST 10 //이 인원수에게 정보를 분배
#define PNUM 20 //생성하는 점의 범위 (0에서의 절댓값)

/*custumizing operations:*/
#define add(a, b)               return_add(a, b)
#define sub(a, b)               return_sub(a, b)
#define mul(a, b)               return_mul(a, b)
#define div(a, b)               return_div(a, b)
#define mod(a, b)               return_mod(a, b)
#define sqrt(a)                 return_isqrt(a)
#define pow(a, b)               return_pow(a, b)
#define inc_one(a)              return_inc(a)
#define dec_one(a)              return_dec(a)
#define abs(a)                  return_abs(a)
#define from_int(a)             return_from_int(a)

/* Initialization functions: */
#define init(A)                 bignum_init(&(A))               /* n gets zero-initialized */
#define to_int(A)               bignum_to_int(&(A))

/* Special operators and comparison */
#define cmp(a, b)               compare(a, b)                   /* Compare: returns LARGER, EQUAL or SMALLER */
#define is_zero(a)              bignum_is_zero(&(a))            /* For comparison with zero */
#define inc(a)                  bignum_inc(&(a))                /* Increment: add one to n */
#define dec(a)                  bignum_dec(&(a))                /* Decrement: subtract one from n */
#define copy(d, s)              bignum_assign(&(d), &(s))       /* Copy src into dst -- dst := src */
#define inv(a)					bignum_inv(&(a))
#define inv_r(a)				return_inv(a)

/* Polynomial operations: */
#define setPoly(A)              for (int k = 0; k < LEN; k++)   { init(A[k].x); A[k].y = from_int(1); }
#define addPoly(P, Q, R)        for (int k = 0; k < LEN; k++)   R[k] = addFrac(P[k], Q[k]);
#define mulScalar(P, n)         for (int k = 0; k < LEN; k++)   { P[k].x = mul(P[k].x, n); }
#define divScalar(P, n)         for (int k = 0; k < LEN; k++)   { P[k].y = mul(P[k].y, n); }
#define copyFrac(P, Q)          { P.x = Q.x; P.y = Q.y; }
#define copyPoly(P, Q)          for (int k = 0; k < LEN; k++)   { P[k].x = Q[k].x; P[k].y = Q[k].y; }
#define simplePoly(P)           for (int k = 0; k < LEN; k++)   simpleData(&(P[k]));

