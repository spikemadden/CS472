#include <stdio.h>

#define LITTLE_ENDIAN (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define BIG_ENDIAN (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)

int main(int argc, char **argv)
{
        short val;
        char *p_val;
        p_val = (char *) &val;

        #if defined(__BYTE_ORDER__) && LITTLE_ENDIAN
        p_val[0] = 0x34;
        p_val[1] = 0x12;
        printf("little endian: %x\n", val);
        #elif defined(__BYTE_ORDER__) && BIG_ENDIAN
        p_val[0] = 0x12;
        p_val[1] = 0x34;
        printf("big endian: %x\n", val);
        #endif

        return 0;
}
