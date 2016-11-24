#include <stdio.h>
#include <stdint.h>

#define LITTLE_ENDIAN (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#define BIG_ENDIAN (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)

int main(int argc, char **argv) {
    short val;
    char *p_val;
    p_val = (char *) &val;

    printf("Part 4 - using compiler directives to test for endianness\n");

    #if defined(__BYTE_ORDER__) && LITTLE_ENDIAN
      p_val[0] = 0x34;
      p_val[1] = 0x12;
      printf("little endian: %x\n", val);
    #elif defined(__BYTE_ORDER__) && BIG_ENDIAN
      p_val[0] = 0x12;
      p_val[1] = 0x34;
      printf("big endian: %x\n", val);
    #endif

    printf("Part 5 - testing endianness at runtime using an union\n");

    union {
      uint32_t i;
      char p_val[2];
    } test = {0x1234};

    if (test.p_val[0] == 0x12) {
      printf("big endian: %x\n", val);
    }

    else {
      printf("little endian: %x\n", val);
    }

    return 0;
}
