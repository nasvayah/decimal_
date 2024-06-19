#include "s21_decimal.h"

int s21_negate(s21_decimal x, s21_decimal *res) {
  s21_decimal nul = {0};
  *res = nul;
  int sign = get_binary(x.bits[3], 31);
  if (sign == 1)
    change_to_zero(&x.bits[3], 31);
  else
    change_to_one(&x.bits[3], 31);
  *res = x;
  return 0;
}

int s21_truncate(s21_decimal x, s21_decimal *res) {
  s21_decimal nul = {0};
  *res = nul;
  int sign = get_binary(x.bits[3], 31);
  delete_ten(&x);
  int x_scale = get_scale(x);
  s21_decimal ten_pow = get_ten_pow(x_scale);
  s21_decimal left = {{}};
  div_decimal_mod_wo_ok(x, ten_pow, res, &left);
  if (sign == 1) change_to_one(&res->bits[3], 31);
  if (left.bits[3]) return 1;
  return 0;
}

int s21_round(s21_decimal x, s21_decimal *res) {
  s21_decimal nul = {0};
  *res = nul;
  delete_ten(&x);
  int x_scale = get_scale(x);
  int x_sign = get_binary(x.bits[3], 31);
  int check = 0;
  s21_decimal one = {{1, 0, 0, 0}};
  *res = x;
  if (x_scale) {
    s21_decimal ten_pow = get_ten_pow(x_scale - 1);
    s21_decimal left = {{}};
    div_decimal_mod_wo_ok(x, ten_pow, res, &left);
    if (left.bits[3]) check = 1;
    ten_pow = get_ten_pow(1);
    div_decimal_mod_wo_ok(*res, ten_pow, res, &left);
    if (x_sign) change_to_one(&res->bits[3], 31);
    if (left.bits[3]) check = 1;
    if (left.bits[0] >= 5 && !x_sign) {
      if (s21_add(*res, one, res)) check = 1;
    }
    if (left.bits[0] >= 5 && x_sign) {
      if (s21_sub(*res, one, res)) check = 1;
    }
  }
  return check;
}

int s21_floor(s21_decimal x, s21_decimal *res) {
  s21_decimal nul = {0};
  *res = nul;
  delete_ten(&x);
  int x_scale = get_scale(x);
  int x_sign = get_binary(x.bits[3], 31);
  int check = 0;
  s21_decimal one = {{1, 0, 0, 0}};
  *res = x;
  if (x_scale) {
    s21_decimal ten_pow = get_ten_pow(x_scale - 1);
    s21_decimal left = div_decimal_mod_wo(x, ten_pow, res);
    if (left.bits[3]) check = 1;
    ten_pow = get_ten_pow(1);
    left = div_decimal_mod_wo(*res, ten_pow, res);
    if (x_sign) change_to_one(&res->bits[3], 31);
    if (left.bits[3]) check = 1;

    if (x_sign) {
      if (s21_sub(*res, one, res)) check = 1;
    }
  }
  return check;
}
