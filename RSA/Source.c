#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "RSA.h"


int main(void)
{
    DATA N, e, d, p, q;
    DATA plane_s[INLEN] = { 0, }, cipher_s[INLEN] = { 0, };
    unsigned char str[INLEN] = { 0, }, decrypted_s[INLEN] = { 0, }, decrypted_fast_s[INLEN] = { 0, };
    int i, inputlen;


    printf("Program Start...\n\n");
    srand((unsigned int)time(NULL));
    makeKey(&N, &e, &d, &p, &q);


    printf("암호화할 문자열을 입력해 주세요(%d자 이내): ", INLEN);
    gets_s(str, sizeof(str));
    inputlen = strlen(str);

    printf("\n공개키: (%llu, %llu)\n", N, e);

    printf("\n평문  : %s\n\n", str);


    printf("10진수: ");
    for (i = 0; i < inputlen; i++)
    {
        printf("%d ", str[i]);
        //plane_s[i] = (DATA)str[i];
        cipher_s[i] = encryption(N, e, (DATA)str[i]);
        decrypted_fast_s[i] = (char)decryption_fast(N, d, cipher_s[i], p, q);
    }
    printf("\n\n암호문: ");
    for (i = 0; i < inputlen; i++) printf("%llx ", cipher_s[i]);

    printf("\n\n================================================\n");
    printf("개인키: (%llu, %llu)\n", N, d);
    printf("\n\n암호문: ");
    for (i = 0; i < inputlen; i++) printf("%llx ", cipher_s[i]);

    printf("\n\n복호화: ");
    for (i = 0; i < inputlen; i++) printf("%d ", decrypted_fast_s[i]);
    printf("\n\n복호문: %s\n", decrypted_fast_s);

    return 0;
}

