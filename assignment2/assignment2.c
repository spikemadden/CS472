/*
Worked with:
McKenna Jones, Zach Iverson

Code referenced for software addition and subtraction:
https://github.com/allforlove901/CPE-315-Lab-1/blob/master/CPE315_Lab1/
*/

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

long long extract_double(double value, int* sign, int* exponent) {
  unsigned long long extract = *(unsigned long long *) &value;

  // sign
  int positive_or_negative = extract >> 63;
  *sign = positive_or_negative;

  // exponent
  int exp = ((extract >> 52) & 0x7FF) - 1023;
  *exponent = exp;

  // fraction
  long long fraction = (extract << 10) & 0x3FFFFFFFFFFFFC00;
  fraction |= 0x4000000000000000;

  if (*sign == 1)
    fraction *= -1;

  return fraction;
}

double newmyadd(double first, double second) {
  int first_sign, second_sign;
  int first_exponent, second_exponent;
  int exp_difference;

  long long sum_sign, sum_exponent;
  long long first_fraction, second_fraction, sum_fraction;

  first_fraction = extract_double(first, &first_sign, &first_exponent);
  second_fraction = extract_double(second, &second_sign, &second_exponent);

  exp_difference = first_exponent - second_exponent;

  // adjust exponents
  if (exp_difference > 0) {
    second_fraction >>= exp_difference;
    second_exponent += exp_difference;
  }

  else if (exp_difference < 0) {
    exp_difference *= -1;
    first_fraction >>= exp_difference;
    first_exponent += exp_difference;
  }

  // prevent overflow
  if ((first_fraction >> 1) + (second_fraction >> 1) < 0) {
    sum_sign = 0x8000000000000000;
  }

  sum_fraction = (first_fraction >> 1) + (second_fraction >> 1);
  sum_exponent = first_exponent + 1;

  // normalize
  long long sign_compare;
  sign_compare = sum_fraction >> 1;

  if (sum_fraction != 0) {
    while(((sum_fraction ^ sign_compare) & 0x4000000000000000) == 0) {
      sum_fraction <<= 1;
      sum_exponent--;
    }
  }

  unsigned long long dub = 0;
  double result = 0;

  dub |= (sum_sign & 0x8000000000000000);

  sum_exponent += 1023;
  dub |= (sum_exponent << 52);

  sum_fraction >>= 10;
  dub |= (sum_fraction) & 0xFFFFFFFFFFFFF;

  result = *((double*)&dub);

  return result;
}

double newmysubtract(double first, double second) {
  int first_sign, second_sign;
  int first_exponent, second_exponent;
  int exp_difference;

  long long sum_sign, sum_exponent;
  long long first_fraction, second_fraction, sum_fraction;

  first_fraction = extract_double(first, &first_sign, &first_exponent);
  second_fraction = extract_double(second, &second_sign, &second_exponent);

  exp_difference = first_exponent - second_exponent;

  // adjust exponents
  if (exp_difference > 0) {
    second_fraction >>= exp_difference;
    second_exponent += exp_difference;
  }

  else if (exp_difference < 0) {
    exp_difference *= -1;
    first_fraction >>= exp_difference;
    first_exponent += exp_difference;
  }

  // prevent overflow
  if ((first_fraction >> 1) + (second_fraction >> 1) < 0) {
    sum_sign = 0x8000000000000000;
  }

  sum_fraction = (first_fraction >> 1) - (second_fraction >> 1);
  sum_exponent = first_exponent + 1;

  // normalize
  long long sign_compare;
  sign_compare = sum_fraction >> 1;

  if (sum_fraction != 0) {
    while(((sum_fraction ^ sign_compare) & 0x4000000000000000) == 0) {
      sum_fraction <<= 1;
      sum_exponent--;
    }
  }

  unsigned long long dub = 0;
  double result = 0;

  dub |= (sum_sign & 0x8000000000000000);

  sum_exponent += 1023;
  dub |= (sum_exponent << 52);

  sum_fraction >>= 10;
  dub |= (sum_fraction) & 0xFFFFFFFFFFFFF;

  result = *((double*)&dub);

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
    int signeroni;
    double real = frexp(test, &exponent);
    printf("frexp() returns %f * 2^%d\n", real, exponent);
    double my_result = myfrexp(test, &exponent);
    printf("myfrexp() returns %f * 2^%d\n\n", my_result, exponent);

    // part 2: floating point operations
    unsigned long long start, end;
    double sum, difference, product, quotient, root;
    double num1 = 8.0;
    double num2 = 1.5;
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
      sum = newmyadd(num1, num2);
    }
    end = rdtsc();

    printf("addition: %f + %f = %f\n", num1, num2, sum);
    printf("software addition took %llu cycles\n\n", (end - start)/100);

    start = rdtsc();
    for (i = 0; i < 100; i++) {
      difference = newmysubtract(num1, num2);
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
