#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "bn-signed.h"
#include "SecretShare.h"
#include "SecretShare_MF.h"


int main(void)
{
    DATA key;
    POINT Point_1[DIST], Point_2[LEN];
    Poly P, L;
    FILE* fp;
    char fname[30];

    printf("Program Start...\n\n");
    srand((unsigned int)time(NULL));
    
    key = from_int(10);

    dispersion(Point_1, key);

    for (int i = 0; i < DIST; i++)
    {
        sprintf(fname, "dist_data/dist_data %d.bin", i + 1);
        fp = fopen(fname, "wb");
        fwrite(&(Point_1[i]), sizeof(POINT), 1, fp);
        fclose(fp);
    }
    

    for (int i = 0; i < LEN; i++)   Point_2[i] = Point_1[i];
    restore(L, Point_2);

    return 0;
}
