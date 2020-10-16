#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

//#define WORD_SIZE 1
/* This macro defines the word size in bytes of the array that constitues the big-number data structure. */
#ifndef WORD_SIZE
#define WORD_SIZE 4
#endif

/* Size of big-numbers in bytes */
#define BN_ARRAY_SIZE    (128 / WORD_SIZE)


/* Here comes the compile-time specialization for how large the underlying array size should be. */
/* The choices are 1, 2 and 4 bytes in size with uint32, uint64 for WORD_SIZE==4, as temporary. */
#ifndef WORD_SIZE
#error Must define WORD_SIZE to be 1, 2, 4

#elif (WORD_SIZE == 1)
  /* Data type of array in structure */
#define DTYPE                    uint8_t
/* bitmask for getting MSB */
#define DTYPE_MSB                ((DTYPE_TMP)(0x80))
/* Data-type larger than DTYPE, for holding intermediate results of calculations */
#define DTYPE_TMP                uint32_t
#define SDTYPE_TMP               int64_t
/* sprintf format string */
#define SPRINTF_FORMAT_STR       "%.02x"
#define SSCANF_FORMAT_STR        "%2hhx"
/* Max value of integer type */
#define MAX_VAL                  ((DTYPE_TMP)0xFF)

#elif (WORD_SIZE == 2)
#define DTYPE                    uint16_t
#define DTYPE_TMP                uint32_t
#define DTYPE_MSB                ((DTYPE_TMP)(0x8000))
#define SPRINTF_FORMAT_STR       "%.04x"
#define SSCANF_FORMAT_STR        "%4hx"
#define MAX_VAL                  ((DTYPE_TMP)0xFFFF)

#elif (WORD_SIZE == 4)
#define DTYPE                    uint32_t
#define DTYPE_TMP                uint64_t
#define SDTYPE_TMP               int64_t
#define DTYPE_MSB                ((DTYPE_TMP)(0x80000000))
#define SPRINTF_FORMAT_STR       "%.08x"
#define SSCANF_FORMAT_STR        "%8x"
#define MAX_VAL                  ((DTYPE_TMP)0xFFFFFFFF)
#endif
#ifndef DTYPE
#error DTYPE must be defined to uint8_t, uint16_t uint32_t or whatever
#endif



#define require(p, msg)	assert(p && msg)
#define put(A)			puts("A")
#define simple(a)		if (a.sig && bignum_is_zero(&a)) a.sig = false; else;
#define simple_p(a)		if (a->sig && bignum_is_zero(a)) a->sig = false; else;


struct bn
{
	bool sig;
	DTYPE array[BN_ARRAY_SIZE];
};
enum { SMALLER = -1, EQUAL = 0, LARGER = 1 };


/* Initialization functions: */
void bignum_init(struct bn* n);
void bignum_from_int(struct bn* n, SDTYPE_TMP i);
SDTYPE_TMP  bignum_to_int(struct bn* n);
void bignum_from_string(struct bn* n, char* str, int nbytes);
void bignum_to_string(struct bn* n, char* str, int maxsize);

/* Basic arithmetic operations: */
void bignum_add(struct bn* a, struct bn* b, struct bn* c);
void bignum_sub(struct bn* a, struct bn* b, struct bn* c);
void bignum_mul(struct bn* a, struct bn* b, struct bn* c);
void bignum_div(struct bn* a, struct bn* b, struct bn* c);
void bignum_mod(struct bn* a, struct bn* b, struct bn* c);
void bignum_divmod(struct bn* a, struct bn* b, struct bn* c, struct bn* d);

/* Bitwise operations: */
void bignum_and(struct bn* a, struct bn* b, struct bn* c);
void bignum_or(struct bn* a, struct bn* b, struct bn* c);
void bignum_xor(struct bn* a, struct bn* b, struct bn* c);
void bignum_lshift(struct bn* a, struct bn* b, int nbits);
void bignum_rshift(struct bn* a, struct bn* b, int nbits);

/* Special operators and comparison */
int  bignum_cmp(struct bn* a, struct bn* b);
int  bignum_is_zero(struct bn* n);
void bignum_inc(struct bn* n);
void bignum_inc_abs(struct bn* n);
void bignum_dec(struct bn* n);
void bignum_dec_abs(struct bn* n);
void bignum_pow(struct bn* a, struct bn* b, struct bn* c);
void bignum_isqrt(struct bn* a, struct bn* b);
void bignum_assign(struct bn* dst, struct bn* src);

/* customizing operations */
struct bn return_add(struct bn a, struct bn b);
struct bn return_sub(struct bn a, struct bn b);
struct bn return_mul(struct bn a, struct bn b);
struct bn return_div(struct bn a, struct bn b);
struct bn return_mod(struct bn a, struct bn b);
struct bn return_pow(struct bn a, struct bn b);
struct bn return_isqrt(struct bn a);
struct bn return_inc(struct bn n);
struct bn return_dec(struct bn n);
struct bn return_mod_num(struct bn a, SDTYPE_TMP n);
struct bn return_abs(struct bn n);
struct bn return_inv(struct bn n);
void bignum_sub_abs(struct bn* a, struct bn* b, struct bn* c);
void bignum_sub_size(struct bn* a, struct bn* b, struct bn* c);
void bignum_inv(struct bn* n);

struct bn return_from_int(SDTYPE_TMP i);
char* return_to_string(struct bn n);
struct bn return_from_string(char* str, int nbytes);

int bignum_cmp_num(struct bn a, SDTYPE_TMP n);
int compare(struct bn a, struct bn b);
int return_is_zero(struct bn n);
void reportError(char*);
#endif /* #ifndef __BIGNUM_H__ */
