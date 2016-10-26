#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static __inline__ unsigned long long rdtsc(void) {
  unsigned hi, lo;
  __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
  return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}

double myfrexp(double value, int* exponent) {
  long long input, input2;
  long long mantissa;
  double result;
  int exp;
  int sign;

  memcpy(&input, &value, 8);
  memcpy(&input2, &value, 8);

  sign = input2 << 63;

  exp = ((input >> 52) & 2047) - 1022;
  *exponent = exp;

  mantissa = input & 0xFFFFFFFFFFFFF;
  result = (mantissa/pow(2,52) + 1) / 2;

  if (sign == 1)
    result *= -1;

  return result;
}

double myadd(double first, double second) {
  int exponent1;
  int exponent2;
  int exp_difference;

  double double_fraction_1;
  double double_fraction_2;
  double sum;

  uint64_t int_fraction_1;
  uint64_t int_fraction_2;
  uint64_t masked1;
  uint64_t masked2;

  double_fraction_1 = frexp(first, &exponent1);
  double_fraction_2 = frexp(second, &exponent2);

  memcpy(&int_fraction_1, &double_fraction_1, 8);
  memcpy(&int_fraction_2, &double_fraction_2, 8);

  masked1 = int_fraction_1 & 0xFFFFFFFFFFFFF;
  masked2 = int_fraction_2 & 0xFFFFFFFFFFFFF;

  masked1 = masked1 | 0x10000000000000;
  masked2 = masked2 | 0x10000000000000;

  exp_difference = exponent1 - exponent2;

  if (exp_difference >= 0) {
    masked2 = masked2 >> exp_difference;
    sum = pow(2, exponent1) * ((masked1 / pow(2, 52) / 2) + (masked2 / pow(2, 52) / 2));
  }

  else if (exp_difference < 0) {
    exp_difference *= -1;
    masked1 = masked1 >> exp_difference;
    sum = pow(2, exponent2) * ((masked1 / pow(2, 52) / 2) + (masked2 / pow(2, 52) / 2));
  }
  return sum;
}

double mysubstract(double first, double second) {
  int exponent1;
  int exponent2;
  int exp_difference;

  double double_fraction_1;
  double double_fraction_2;
  double difference;

  uint64_t int_fraction_1;
  uint64_t int_fraction_2;
  uint64_t masked1;
  uint64_t masked2;

  double_fraction_1 = frexp(first, &exponent1);
  double_fraction_2 = frexp(second, &exponent2);

  memcpy(&int_fraction_1, &double_fraction_1, 8);
  memcpy(&int_fraction_2, &double_fraction_2, 8);

  masked1 = int_fraction_1 & 0xFFFFFFFFFFFFF;
  masked2 = int_fraction_2 & 0xFFFFFFFFFFFFF;

  masked1 = masked1 | 0x10000000000000;
  masked2 = masked2 | 0x10000000000000;

  exp_difference = exponent1 - exponent2;

  if (exp_difference >= 0) {
    masked2 = masked2 >> exp_difference;
    difference = pow(2, exponent1) * ((masked1 / pow(2, 52) / 2) - (masked2 / pow(2, 52) / 2));
  }

  else if (exp_difference < 0) {
    exp_difference *= -1;
    masked1 = masked1 >> exp_difference;
    difference = pow(2, exponent2) * ((masked1 / pow(2, 52) / 2) - (masked2 / pow(2, 52) / 2));
  }
  return difference;
}

double mymultiply(double first, double second) {
  int exponent1;
  int exponent2;

  double double_fraction_1;
  double double_fraction_2;
  double product;

  double_fraction_1 = frexp(first, &exponent1);
  double_fraction_2 = frexp(second, &exponent2);

  product = double_fraction_1 * double_fraction_2 * pow(2, exponent1 + exponent2);

  return product;
}

double mydivide(double first, double second) {
  int exponent1;
  int exponent2;

  double double_fraction_1;
  double double_fraction_2;
  double quotient;

  double_fraction_1 = frexp(first, &exponent1);
  double_fraction_2 = frexp(second, &exponent2);

  quotient = double_fraction_1 / double_fraction_2 * pow(2, exponent1 - exponent2);

  return quotient;
}

double mysquareroot(double number) {
  int exponent;
  double double_fraction;
  double result;

  double_fraction = frexp(number, &exponent);

  if (exponent % 2 == 0)
    result = pow(double_fraction, 0.5) * pow(2, exponent/2);
  else
    result = pow(2 * double_fraction, 0.5) * pow(2, (exponent-1)/2);

  return result;
}

int main() {
    // part 1: frexp
    double test = 123.45;
    int exponent;
    double real = frexp(test, &exponent);
    printf("frexp() returns %f * 2^%d\n", real, exponent);
    double my_result = myfrexp(test, &exponent);
    printf("myfrexp() returns %f * 2^%d\n\n", my_result, exponent);

    // part 2: floating point operations
    unsigned long long start, end;
    double sum, difference, product, quotient, root;
    double num1 = 4.5;
    double num2 = 2.1;
    int i = 0;

    printf("%s\n", "*** Printing FPU based implementation ***");

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      sum = num1 + num2;
    }
    end = rdtsc();

    printf("addition: %f + %f = %f\n", num1, num2, sum);
    printf("hardware addition took %llu cycles\n\n", (end - start)/100);

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      difference = num1 - num2;
    }
    end = rdtsc();

    printf("subtraction: %f - %f = %f\n", num1, num2, difference);
    printf("hardware subtraction took %llu cycles\n\n", (end - start)/100);

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      product = num1 * num2;
    }
    end = rdtsc();

    printf("multiplication: %f * %f = %f\n", num1, num2, product);
    printf("hardware multiplication took %llu cycles\n\n", (end - start)/100);

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      quotient = num1 / num2;
    }
    end = rdtsc();

    printf("division: %f / %f = %f\n", num1, num2, quotient);
    printf("hardware division took %llu cycles\n\n", (end - start)/100);

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      root = sqrt(num1);
    }
    end = rdtsc();

    printf("square root: sqrt(%f) = %f\n", num1, root);
    printf("hardware square root took %llu cycles\n\n", (end - start)/100);

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      sum = num1 + num2;
    }
    end = rdtsc();

    printf("%s\n", "*** Printing my implementation  ***");

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      sum = myadd(num1, num2);
    }
    end = rdtsc();

    printf("addition: %f + %f = %f\n", num1, num2, sum);
    printf("software addition took %llu cycles\n\n", (end - start)/100);

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      difference = mysubstract(num1, num2);
    }
    end = rdtsc();

    printf("subtraction: %f - %f = %f\n", num1, num2, difference);
    printf("software subtraction took %llu cycles\n\n", (end - start)/100);

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      product = mymultiply(num1, num2);
    }
    end = rdtsc();

    printf("multiplication: %f * %f = %f\n", num1, num2, product);
    printf("software multiplication took %llu cycles\n\n", (end - start)/100);

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      quotient = mydivide(num1, num2);
    }
    end = rdtsc();

    printf("division: %f / %f = %f\n", num1, num2, quotient);
    printf("software division took %llu cycles\n\n", (end - start)/100);

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      root = mysquareroot(num1);
    }
    end = rdtsc();

    printf("square root: sqrt(%f) = %f\n", num1, root);
    printf("software square root took %llu cycles\n\n", (end - start)/100);


    // part 3: feature extraction
    const int BIT_PATTERN = 101010;
    long long input;
    int sign;
    char buffer[8];

    int a;
    double b;
    b = frexp(BIT_PATTERN, &a);

    memcpy(&input, &BIT_PATTERN, 8);
    sign = input << 63;

    printf("Feature Extraction for the following bit pattern: %d\n", BIT_PATTERN);
    printf("sign when bit pattern is treated as a double is %d\n", sign);
    printf("mantissa when bit pattern is treated as a double is %f\n", b);
    printf("exponent when bit pattern is treated as a double is %d\n", a);


    sign = !(BIT_PATTERN>0);
    printf("sign when bit pattern is treated as a long is %d\n", sign);
    printf("value when bit pattern is treated as a long is %ld\n", BIT_PATTERN);


    snprintf(buffer, 8, "%d", BIT_PATTERN);
    printf("value when the bit pattern is treated as 8 characters: %s\n", buffer);
    printf("the size of this is %d bytes\n", sizeof(buffer));

    return 0;
}
