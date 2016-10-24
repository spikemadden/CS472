#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

double myfrexp(double value, int* exponent) {
  long long input;
  long long mantissa;
  double result;
  int exp;

  memcpy(&input, &value, 8);

  exp = ((input >> 52) & 2047) - 1022;
  *exponent = exp;

  mantissa = input & 0xFFFFFFFFFFFFF;
  result = (mantissa/pow(2,52) + 1) / 2;

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

int main() {

    // part 1: frexp
    double test = 123.45;
    int exponent;
    double real = frexp(test, &exponent);
    printf("frexp() returns %f * 2^%d\n", real, exponent);
    double my_result = myfrexp(test, &exponent);
    printf("myfrexp() returns %f * 2^%d\n", my_result, exponent);


    // part 2: floating point operations
    double num1 = 4.5;
    double num2 = 2.1;

    double sum = myadd(num1, num2);
    double difference = mysubstract(num1, num2);
    double product = mymultiply(num1, num2);
    double quotient = mydivide(num1, num2);

    printf("addition: %f + %f = %f\n", num1, num2, sum);
    printf("subtraction: %f - %f = %f\n", num1, num2, difference);
    printf("multiplication: %f * %f = %f\n", num1, num2, product);
    printf("division: %f / %f = %f\n", num1, num2, quotient);

    return 0;
}
