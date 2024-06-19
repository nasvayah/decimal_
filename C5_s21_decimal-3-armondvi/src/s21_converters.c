#include "s21_decimal.h"

int s21_from_int_to_decimal(int x, s21_decimal *res) {
  s21_decimal nul = {0};
  *res = nul;

  if (x < 0) {
    change_to_one(&res->bits[3], 31);
    x *= -1;
  }
  res->bits[0] = (unsigned int)x;
  return 0;
}

int s21_from_float_to_decimal(float x, s21_decimal *res) {
  s21_decimal nul = {0};
  *res = nul;

  int check = 0;
  int sign = 0;
  s21_decimal ten = {{10, 0, 0, 0}};

  if (x < 0) {
    sign = 1;
    x *= -1;
  }
  int d = 0;
  char float_str[64] = {'\0'};
  sprintf(float_str, "%f", x);
  char *s = float_str;
  while (*s != '\0' && *s != '.') {
    unsigned int c = (int)*s - 48;
    s21_decimal tmp = {{c, 0, 0, 0}};
    if (s21_mul(*res, ten, res) || s21_add(*res, tmp, res)) check = 1;
    s++;
  }
  s21_decimal res_after_dot = {0};
  if (*s == '.') {
    s++;
    while (*s != '\0') {
      if (check_zeros(s)) {
        d++;
        unsigned int c = (int)*s - 48;
        s21_decimal tmp = {{c, 0, 0, 0}};
        if (s21_mul(res_after_dot, ten, &res_after_dot) ||
            s21_add(res_after_dot, tmp, &res_after_dot))
          check = 1;
      }
      s++;
    }
  }
  if (res_after_dot.bits[0] || res_after_dot.bits[0] || res_after_dot.bits[0]) {
    s21_decimal ten_pow = get_ten_pow(d);
    if (s21_mul(*res, ten_pow, res) || s21_add(res_after_dot, *res, res))
      check = 1;
  }
  set_scale(res, d);
  if (sign){ change_to_one(&res->bits[3], 31);}
  return check;
}

int s21_from_decimal_to_int(s21_decimal x, int *res) {
  *res = 0;

  int check = 0;
  s21_truncate(x, &x);
  if (x.bits[2] || x.bits[1] || get_binary(x.bits[0], 31)) check = 1;

  *res = (int)x.bits[0];

  if (get_binary(x.bits[3], 31)) *res = *res * (-1);

  return check;
}

int s21_from_decimal_to_float(s21_decimal x, float *res) {
  int res_int = 0;
  *res = 0;
  int scale = get_scale(x);
  set_scale(&x, 0);
  int check = s21_from_decimal_to_int(x, &res_int);


  *res = (float)res_int;
  *res = (*res) / pow(10, scale);

  return check;
}

int check_zeros(char *s) {
  int check = 0;
  while (*s != '\0') {
    if (*s != '0') check = 1;
    s++;
  }
  return check;
}
