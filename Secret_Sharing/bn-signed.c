#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include "bn-signed.h"



/* Functions for shifting number in-place. */
static void _lshift_one_bit(struct bn* a);
static void _rshift_one_bit(struct bn* a);
static void _lshift_word(struct bn* a, int nwords);
static void _rshift_word(struct bn* a, int nwords);

/* int */
void bignum_init(struct bn* n)
{
    require(n, "n is null");
    int i;

    n->sig = false;
    for (i = 0; i < BN_ARRAY_SIZE; ++i)
    {
        n->array[i] = 0;
    }
}
void bignum_from_int(struct bn* n, SDTYPE_TMP i)
{
    require(n, "n is null");

    bignum_init(n);
    //printf("==> %lld, %d\n", i, i<0);
    if (i < 0)
    {
        n->sig = true;
        i *= -1;
        //printf("%lld\n", i);
    }
    else    n->sig = false;
    
    /* Endianness issue if machine is not little-endian? */
#ifdef WORD_SIZE
#if (WORD_SIZE == 1)
    n->array[0] = (i & 0x000000ff);
    n->array[1] = (i & 0x0000ff00) >> 8;
    n->array[2] = (i & 0x00ff0000) >> 16;
    n->array[3] = (i & 0xff000000) >> 24;
#elif (WORD_SIZE == 2)
    n->array[0] = (i & 0x0000ffff);
    n->array[1] = (i & 0xffff0000) >> 16;
#elif (WORD_SIZE == 4)
    n->array[0] = i;
    DTYPE_TMP num_32 = 32;
    DTYPE_TMP tmp = i >> num_32; /* bit-shift with U64 operands to force 64-bit results */
    n->array[1] = tmp;
#endif
#endif
}
struct bn return_from_int(SDTYPE_TMP i)
{
    struct bn n;
    bignum_from_int(&n, i);
    return n;
}
SDTYPE_TMP bignum_to_int(struct bn* n)
{
    require(n, "n is null");

    SDTYPE_TMP ret = 0;

    /* Endianness issue if machine is not little-endian? */
#if (WORD_SIZE == 1)
    ret += n->array[0];
    ret += n->array[1] << 8;
    ret += n->array[2] << 16;
    ret += n->array[3] << 24;
#elif (WORD_SIZE == 2)
    ret += n->array[0];
    ret += n->array[1] << 16;
#elif (WORD_SIZE == 4)
    ret += n->array[0];
#endif

    if (n->sig == true) ret *= -1;
    return ret;
}


/* string */
void bignum_from_string(struct bn* n, char* str, int nbytes)
{
    require(n, "n is null");
    require(str, "str is null");
    require(nbytes > 0, "nbytes must be positive");
    require((nbytes & 1) == 0, "string format must be in hex -> equal number of bytes");
    require((nbytes % (sizeof(DTYPE) * 2)) == 0, "string length must be a multiple of (sizeof(DTYPE) * 2) characters");

    bignum_init(n);

    DTYPE tmp;                        /* DTYPE is defined in bn.h - uint{8,16,32,64}_t */
    int i = nbytes - (2 * WORD_SIZE); /* index into string */
    int j = 0;                        /* index into array */

    /* reading last hex-byte "MSB" from string first -> big endian */
    /* MSB ~= most significant byte / block ? :) */
    while (i >= 0)
    {
        tmp = 0;
        sscanf(&str[i], SSCANF_FORMAT_STR, &tmp);
        n->array[j] = tmp;
        i -= (2 * WORD_SIZE); /* step WORD_SIZE hex-byte(s) back in the string. */
        j += 1;               /* step one element forward in the array. */
    }
}
struct bn return_from_string(char* str, int nbytes)
{
    struct bn n;
    bignum_from_string(&n, str, nbytes);
    return n;
}
void bignum_to_string(struct bn* n, char* str, int nbytes)
{
    require(n, "n is null");
    require(str, "str is null");
    require(nbytes > 0, "nbytes must be positive");
    require((nbytes & 1) == 0, "string format must be in hex -> equal number of bytes");

    int j = BN_ARRAY_SIZE - 1; /* index into array - reading "MSB" first -> big-endian */
    int i = 0;                 /* index into string representation. */

    /* reading last array-element "MSB" first -> big endian */
    while ((j >= 0) && (nbytes > (i + 1)))
    {
        sprintf(&str[i], SPRINTF_FORMAT_STR, n->array[j]);
        i += (2 * WORD_SIZE); /* step WORD_SIZE hex-byte(s) forward in the string. */
        j -= 1;               /* step one element back in the array. */
    }

    /* Count leading zeros: */
    j = 0;
    while (str[j] == '0')
    {
        j += 1;
    }

    /* Move string j places ahead, effectively skipping leading zeros */
    for (i = 0; i < (nbytes - j); ++i)
    {
        str[i] = str[i + j];
    }

    /* Zero-terminate string */
    str[i] = 0;
}
char* return_to_string(struct bn n)
{
    char str[5000];
    bignum_to_string(&n, str, sizeof(str));
    return str;
}


/* inc, dec */
void bignum_dec_abs(struct bn* n)
{
    require(n, "n is null");

    DTYPE tmp; /* copy of n */
    DTYPE res;
    int i;

    
    for (i = 0; i < BN_ARRAY_SIZE; ++i)
    {
        tmp = n->array[i];
        res = tmp - 1;
        n->array[i] = res;

        if (!(res > tmp))
        {
            break;
        }
    }
}
void bignum_dec(struct bn* n)
{
    simple_p(n);
    if (n->sig == false)
        if (bignum_is_zero(n))
            bignum_from_int(n, -1);
        else
            bignum_dec_abs(n);
    else
        bignum_inc_abs(n);
}
struct bn return_dec(struct bn n)
{
    struct bn r = n;
    bignum_dec(&r);
    return r;
}

void bignum_inc_abs(struct bn* n)
{
    require(n, "n is null");

    DTYPE res;
    DTYPE_TMP tmp; /* copy of n */
    int i;

    for (i = 0; i < BN_ARRAY_SIZE; ++i)
    {
        tmp = n->array[i];
        res = tmp + 1;
        n->array[i] = res;

        if (res > tmp)
            break;
    }
}
void bignum_inc(struct bn* n)
{
    simple_p(n);
    if (n->sig == true)
        if (bignum_cmp_num(*n, -1) == 0)
            bignum_init(n);
        else
            bignum_dec_abs(n);
    else
        bignum_inc_abs(n);
}
struct bn return_inc(struct bn n)
{
    struct bn r = n;
    bignum_inc(&r);
    return r;
}


/* add */
void bignum_add(struct bn* a, struct bn* b, struct bn* c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");

    if (a->sig == true || b->sig == true)   reportError("add function factor is negative");

    DTYPE_TMP tmp;
    int carry = 0;

    for (int i = 0; i < BN_ARRAY_SIZE; ++i)
    {
        tmp = (DTYPE_TMP)a->array[i] + b->array[i] + carry;
        carry = (tmp > MAX_VAL);
        c->array[i] = (tmp & MAX_VAL);
    }
    c->sig = false;
}
struct bn return_add(struct bn a, struct bn b)
{
    struct bn A, B, c;
    A = return_abs(a);
    B = return_abs(b);

    if (a.sig == false && b.sig == false)
    {
        bignum_add(&A, &B, &c);
    }
    else if (a.sig == false && b.sig == true)
    {
        bignum_sub(&A, &B, &c);
    }
    else if (a.sig == true && b.sig == false)
    {
        bignum_sub(&A, &B, &c);
        bignum_inv(&c);
    }
    else
    {
        bignum_add(&A, &B, &c);
        bignum_inv(&c);
    }
    return c;
}


/* sub */
void bignum_sub(struct bn* a, struct bn* b, struct bn* c)
{
    struct bn A, B;
    A = return_abs(*a);
    B = return_abs(*b);
    //printf("sub input: %d, %d\n", bignum_to_int(a), bignum_to_int(b));
    //printf("sub input(abs): %d, %d\n", bignum_to_int(&A), bignum_to_int(&B));

    if (a->sig == false && b->sig == false)
    {
        //puts("A");
        bignum_sub_abs(&A, &B, c);
    }
    else if (a->sig == false && b->sig == true)
    {
        //puts("B");
        bignum_add(&A, &B, c);
    }
    else if (a->sig == true && b->sig == false)
    {
        //puts("C");
        bignum_add(&A, &B, c);
        //printf("sub result: %d, sig: %d\n", bignum_to_int(c), c->sig);
        bignum_inv(c);
        //printf("sub result: %d\n", bignum_to_int(c));
    }
    else
    {
        //puts("D");
        bignum_sub_abs(&A, &B, c);
        bignum_inv(c);
    }
}
void bignum_sub_abs(struct bn* a, struct bn* b, struct bn* c)
{
    if (a->sig == true || b->sig == true)   reportError("sub-size function factor is negative");

    if (bignum_cmp(a, b) == -1)
    {
        bignum_sub_size(b, a, c);
        bignum_inv(c);
    }
    else
    {
        bignum_sub_size(a, b, c);
    }
}
void bignum_sub_size(struct bn* a, struct bn* b, struct bn* c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");

    if (a->sig == true || b->sig == true)   reportError("sub function factor is negative");
    DTYPE_TMP res;
    DTYPE_TMP tmp1;
    DTYPE_TMP tmp2;
    int borrow = 0;
    int i;
    for (i = 0; i < BN_ARRAY_SIZE; ++i)
    {
        tmp1 = (DTYPE_TMP)a->array[i] + (MAX_VAL + 1); /* + number_base */
        tmp2 = (DTYPE_TMP)b->array[i] + borrow;;
        res = (tmp1 - tmp2);
        c->array[i] = (DTYPE)(res & MAX_VAL); /* "modulo number_base" == "% (number_base - 1)" if number_base is 2^N */
        borrow = (res <= MAX_VAL);
    }
    c->sig = false;
}
struct bn return_sub(struct bn a, struct bn b)
{
    struct bn c;
    bignum_sub(&a, &b, &c);
    return c;
}


/* mul */
void bignum_mul(struct bn* a, struct bn* b, struct bn* c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");

    struct bn row;
    struct bn tmp;
    int i, j;
    DTYPE_TMP intermediate;

    bignum_init(c);

    for (i = 0; i < BN_ARRAY_SIZE; ++i)
    {
        bignum_init(&row);

        for (j = 0; j < BN_ARRAY_SIZE; ++j)
        {
            if (i + j < BN_ARRAY_SIZE)
            {
                bignum_init(&tmp);
                intermediate = ((DTYPE_TMP)a->array[i] * (DTYPE_TMP)b->array[j]);
                bignum_from_int(&tmp, intermediate);
                _lshift_word(&tmp, i + j);
                bignum_add(&tmp, &row, &row);
            }
        }
        bignum_add(c, &row, c);
    }
    c->sig = (a->sig ^ b->sig) ? true : false;
    if (bignum_is_zero(c))  c->sig = false;
}
struct bn return_mul(struct bn a, struct bn b)
{
    struct bn c;
    bignum_mul(&a, &b, &c);
    return c;
}


/* div */
void bignum_div(struct bn* a, struct bn* b, struct bn* c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");

    struct bn current;
    struct bn denom;
    struct bn tmp;

    bignum_from_int(&current, 1);               // int current = 1;
    denom = return_abs(*b);
    tmp = return_abs(*a);

    const DTYPE_TMP half_max = 1 + (DTYPE_TMP)(MAX_VAL / 2);
    bool overflow = false;
    while (bignum_cmp(&denom, a) != LARGER)     // while (denom <= a) {
    {
        if (denom.array[BN_ARRAY_SIZE - 1] >= half_max)
        {
            overflow = true;
            break;
        }
        _lshift_one_bit(&current);                //   current <<= 1;
        _lshift_one_bit(&denom);                  //   denom <<= 1;
    }
    if (!overflow)
    {
        _rshift_one_bit(&denom);                  // denom >>= 1;
        _rshift_one_bit(&current);                // current >>= 1;
    }
    bignum_init(c);                             // int answer = 0;
    while (!bignum_is_zero(&current))           // while (current != 0)
    {
        if (bignum_cmp(&tmp, &denom) != SMALLER)  //   if (dividend >= denom)
        {
            bignum_sub(&tmp, &denom, &tmp);         //     dividend -= denom;
            bignum_or(c, &current, c);              //     answer |= current;
        }
        _rshift_one_bit(&current);                //   current >>= 1;
        _rshift_one_bit(&denom);                  //   denom >>= 1;
    }
    c->sig = (a->sig ^ b->sig) ? true : false;
    if (bignum_is_zero(c))  c->sig = false;
}
struct bn return_div(struct bn a, struct bn b)
{
    struct bn c;
    bignum_div(&a, &b, &c);
    return c;
}


/* mod */
void bignum_mod(struct bn* a, struct bn* b, struct bn* c)
{
    /*
      Take divmod and throw away div part
    */
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");
    if (a->sig == true || b->sig == true)   reportError("mod function factor is negative");
    struct bn tmp;

    bignum_divmod(a, b, &tmp, c);
}
struct bn return_mod(struct bn a, struct bn b)
{
    struct bn c;
    bignum_mod(&a, &b, &c);
    return c;
}
struct bn return_mod_num(struct bn a, SDTYPE_TMP n)
{
    struct bn m;
    bignum_from_int(&m, n);
    return return_mod(a, m);
}
void bignum_divmod(struct bn* a, struct bn* b, struct bn* c, struct bn* d)
{
    /*
      Puts a%b in d
      and a/b in c

      mod(a,b) = a - ((a / b) * b)

      example:
        mod(8, 3) = 8 - ((8 / 3) * 3) = 2
    */
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");

    struct bn tmp;

    /* c = (a / b) */
    bignum_div(a, b, c);

    /* tmp = (c * b) */
    bignum_mul(c, b, &tmp);

    /* c = a - tmp */
    bignum_sub(a, &tmp, d);
}


/* bit operator */
void bignum_lshift(struct bn* a, struct bn* b, int nbits)
{
    require(a, "a is null");
    require(b, "b is null");
    require(nbits >= 0, "no negative shifts");

    bignum_assign(b, a);
    /* Handle shift in multiples of word-size */
    const int nbits_pr_word = (WORD_SIZE * 8);
    int nwords = nbits / nbits_pr_word;
    if (nwords != 0)
    {
        _lshift_word(b, nwords);
        nbits -= (nwords * nbits_pr_word);
    }

    if (nbits != 0)
    {
        int i;
        for (i = (BN_ARRAY_SIZE - 1); i > 0; --i)
        {
            b->array[i] = (b->array[i] << nbits) | (b->array[i - 1] >> ((8 * WORD_SIZE) - nbits));
        }
        b->array[i] <<= nbits;
    }
}
void bignum_rshift(struct bn* a, struct bn* b, int nbits)
{
    require(a, "a is null");
    require(b, "b is null");
    require(nbits >= 0, "no negative shifts");

    bignum_assign(b, a);
    /* Handle shift in multiples of word-size */
    const int nbits_pr_word = (WORD_SIZE * 8);
    int nwords = nbits / nbits_pr_word;
    if (nwords != 0)
    {
        _rshift_word(b, nwords);
        nbits -= (nwords * nbits_pr_word);
    }

    if (nbits != 0)
    {
        int i;
        for (i = 0; i < (BN_ARRAY_SIZE - 1); ++i)
        {
            b->array[i] = (b->array[i] >> nbits) | (b->array[i + 1] << ((8 * WORD_SIZE) - nbits));
        }
        b->array[i] >>= nbits;
    }

}
void bignum_and(struct bn* a, struct bn* b, struct bn* c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");

    int i;
    for (i = 0; i < BN_ARRAY_SIZE; ++i)
    {
        c->array[i] = (a->array[i] & b->array[i]);
    }
}
void bignum_or(struct bn* a, struct bn* b, struct bn* c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");

    int i;
    for (i = 0; i < BN_ARRAY_SIZE; ++i)
    {
        c->array[i] = (a->array[i] | b->array[i]);
    }
}
void bignum_xor(struct bn* a, struct bn* b, struct bn* c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");

    int i;
    for (i = 0; i < BN_ARRAY_SIZE; ++i)
    {
        c->array[i] = (a->array[i] ^ b->array[i]);
    }
}


/* compare */
int bignum_cmp(struct bn* a, struct bn* b)
{
    require(a, "a is null");
    require(b, "b is null");
    if ((a->sig && bignum_is_zero(a)) || (b->sig && bignum_is_zero(b)))
        reportError("compare function factor is -0");
    int i = BN_ARRAY_SIZE;

    do
    {
        i -= 1; /* Decrement first, to start with last array element */
        if (a->array[i] > b->array[i])
        {
            return LARGER;
        }
        else if (a->array[i] < b->array[i])
        {
            return SMALLER;
        }
    } while (i != 0);

    return EQUAL;
}
int compare(struct bn a, struct bn b)
{
    simple(a);
    simple(b);

    if (a.sig == false && b.sig == false)
        return bignum_cmp(&a, &b);
    else if (a.sig == false && b.sig == true)
        return LARGER;
    else if (a.sig == true && b.sig == false)
        return SMALLER;
    else
        return (-1) * bignum_cmp(&a, &b);
}
int bignum_cmp_num(struct bn a, SDTYPE_TMP n)
{
    struct bn t;
    bignum_from_int(&t, n);
    return bignum_cmp(&a, &t);
}
int bignum_is_zero(struct bn* n)
{
    require(n, "n is null");

    for (int i = 0; i < BN_ARRAY_SIZE; ++i)
        if (n->array[i])
            return 0;

    return 1;
}
int return_is_zero(struct bn n)
{
    return bignum_is_zero(&n);
}


/* pow */
void bignum_pow(struct bn* a, struct bn* b, struct bn* c)
{
    require(a, "a is null");
    require(b, "b is null");
    require(c, "c is null");

    struct bn tmp;

    bignum_init(c);
    if (b->sig) reportError("pow function's factor is negative");

    if (bignum_cmp(b, c) == EQUAL)
    {
        /* Return 1 when exponent is 0 -- n^0 = 1 */
        bignum_inc(c);
    }
    else
    {
        struct bn bcopy;
        bignum_assign(&bcopy, b);

        /* Copy a -> tmp */
        bignum_assign(&tmp, a);

        bignum_dec(&bcopy);

        /* Begin summing products: */
        while (!bignum_is_zero(&bcopy))
        {

            /* c = tmp * tmp */
            bignum_mul(&tmp, a, c);
            /* Decrement b by one */
            bignum_dec(&bcopy);

            bignum_assign(&tmp, c);
        }

        /* c = tmp */
        bignum_assign(c, &tmp);
    }
    struct bn t = return_mod_num(*b, 2);
    //printf("mod: %d\n", bignum_to_int(&t));
    if (a->sig == true && !return_is_zero(t)) c->sig = true;
    else c->sig = false;
}
struct bn return_pow(struct bn a, struct bn b)
{
    struct bn c;
    bignum_pow(&a, &b, &c);
    return c;
}


/* sqrt */
void bignum_isqrt(struct bn* a, struct bn* b)
{
    require(a, "a is null");
    require(b, "b is null");

    if (a->sig) reportError("sqrt function's factor is negative");

    struct bn low, high, mid, tmp;

    bignum_init(&low);
    bignum_assign(&high, a);
    bignum_rshift(&high, &mid, 1);
    bignum_inc(&mid);

    while (bignum_cmp(&high, &low) > 0)
    {
        bignum_mul(&mid, &mid, &tmp);
        if (bignum_cmp(&tmp, a) > 0)
        {
            bignum_assign(&high, &mid);
            bignum_dec(&high);
        }
        else
        {
            bignum_assign(&low, &mid);
        }
        bignum_sub(&high, &low, &mid);
        _rshift_one_bit(&mid);
        bignum_add(&low, &mid, &mid);
        bignum_inc(&mid);
    }
    bignum_assign(b, &low);
    b->sig = false;
}
struct bn return_isqrt(struct bn a)
{
    struct bn b;
    bignum_isqrt(&a, &b);
    return b;
}


/* copy */
void bignum_assign(struct bn* dst, struct bn* src)
{
    require(dst, "dst is null");
    require(src, "src is null");

    int i;
    for (i = 0; i < BN_ARRAY_SIZE; ++i)
        dst->array[i] = src->array[i];
    dst->sig = src->sig;
    //dst->sig = false;
}
/*void bignum_copy(struct bn* dst, struct bn* src)
{
    require(dst, "dst is null");
    require(src, "src is null");

    int i;
    for (i = 0; i < BN_ARRAY_SIZE; ++i)
        dst->array[i] = src->array[i];
    dst->sig = src->sig;
}*/


/* Private / Static functions. */
static void _rshift_word(struct bn* a, int nwords)
{
    /* Naive method: */
    require(a, "a is null");
    require(nwords >= 0, "no negative shifts");

    int i;
    if (nwords >= BN_ARRAY_SIZE)
    {
        for (i = 0; i < BN_ARRAY_SIZE; ++i)
        {
            a->array[i] = 0;
        }
        return;
    }

    for (i = 0; i < BN_ARRAY_SIZE - nwords; ++i)
    {
        a->array[i] = a->array[i + nwords];
    }
    for (; i < BN_ARRAY_SIZE; ++i)
    {
        a->array[i] = 0;
    }
}
static void _lshift_word(struct bn* a, int nwords)
{
    require(a, "a is null");
    require(nwords >= 0, "no negative shifts");

    int i;
    /* Shift whole words */
    for (i = (BN_ARRAY_SIZE - 1); i >= nwords; --i)
    {
        a->array[i] = a->array[i - nwords];
    }
    /* Zero pad shifted words. */
    for (; i >= 0; --i)
    {
        a->array[i] = 0;
    }
}
static void _lshift_one_bit(struct bn* a)
{
    require(a, "a is null");

    int i;
    for (i = (BN_ARRAY_SIZE - 1); i > 0; --i)
    {
        a->array[i] = (a->array[i] << 1) | (a->array[i - 1] >> ((8 * WORD_SIZE) - 1));
    }
    a->array[0] <<= 1;
}
static void _rshift_one_bit(struct bn* a)
{
    require(a, "a is null");

    int i;
    for (i = 0; i < (BN_ARRAY_SIZE - 1); ++i)
    {
        a->array[i] = (a->array[i] >> 1) | (a->array[i + 1] << ((8 * WORD_SIZE) - 1));
    }
    a->array[BN_ARRAY_SIZE - 1] >>= 1;
}


/* custom functions*/
struct bn return_abs(struct bn n)
{
    n.sig = false;
    return n;
}
void bignum_inv(struct bn* n)
{
    //printf("zero: %d\n", bignum_is_zero(n));
    //printf("original: %d, ", n->sig);
    if (bignum_is_zero(n))
        n->sig = false;
    else
        n->sig = !(n->sig);
    //printf("inverse: %d\n", n->sig);
}
struct bn return_inv(struct bn n)
{
    bignum_inv(&n);
    return n;
}
void reportError(char* error)
{
    printf("%s", error);
    exit(1);
}
