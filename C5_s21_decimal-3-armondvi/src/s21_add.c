#include "s21_decimal.h"

int s21_add(s21_decimal x, s21_decimal y, s21_decimal *res) {
  int x_sign = get_binary(x.bits[3], 31);
  int y_sign = get_binary(y.bits[3], 31);
  int x_scale = get_scale(x);
  int y_scale = get_scale(y);
  int res_scale = 0;

  s21_big_decimal x_cp = {{0}};
  s21_big_decimal y_cp = {{0}};
  dec_to_big_dec(x, &x_cp);
  dec_to_big_dec(y, &y_cp);

  int check = 0;
  if (x_scale >= y_scale) {
    res_scale = x_scale;
    s21_decimal tmp = {0};
    s21_decimal ten_scale = get_ten_pow(x_scale - y_scale);
    set_scale(&tmp, res_scale);
    check = mul_decimal_mod(y, ten_scale, &tmp);
    y = tmp;
  } else {
    res_scale = y_scale;
    s21_decimal tmp = {0};
    s21_decimal ten_scale = get_ten_pow(y_scale - x_scale);
    set_scale(&tmp, res_scale);
    check = mul_decimal_mod(x, ten_scale, &tmp);
    x = tmp;
  }
  set_scale(res, res_scale);
  if (!check) {
    if (!x_sign && !y_sign) {
      check = sum_decimal_mod(x, y, res);
    } else if (x_sign && y_sign) {
      check = sum_decimal_mod(x, y, res);
      change_to_one(&(res->bits[3]), 31);
    } else if (!x_sign && y_sign) {
      change_to_zero(&(y.bits[3]), 31);
      check = s21_sub(x, y, res);
    } else if (x_sign && !y_sign) {
      change_to_zero(&(x.bits[3]), 31);
      check = s21_sub(y, x, res);
    }
    int res_sign = get_binary(res->bits[3], 31);
    if (res_sign == 1 && check == 1) check = 2;
    set_scale(res, res_scale);
    delete_ten(res);
  }
  return check;
}

int sum_decimal_mod(s21_decimal x, s21_decimal y, s21_decimal *res) {
  int res_scale = get_scale(*res);
  s21_decimal nul = {0};
  *res = nul;
  int check = 0;
  s21_big_decimal x_cp = {{0}};
  s21_big_decimal y_cp = {{0}};
  s21_big_decimal res_cp = {{0}};
  dec_to_big_dec(x, &x_cp);
  dec_to_big_dec(y, &y_cp);
  check = sum_big_decimal_mod(x_cp, y_cp, &res_cp);
  set_scale_big(&res_cp, res_scale);
  if (!check) check = big_dec_to_dec(res_cp, res);

  return check;
}
int sum_big_decimal_mod(s21_big_decimal x, s21_big_decimal y,
                        s21_big_decimal *res) {
  s21_big_decimal nul = {0};
  *res = nul;
  x.bits[6] = 0;
  y.bits[6] = 0;
  for (int j = 0; j <= 6; j++) {
    int xb, yb, rb;
    for (int i = 0; i <= 31; i++) {
      xb = get_binary(x.bits[j], i);
      yb = get_binary(y.bits[j], i);
      rb = get_binary(res->bits[j], i);
      if (xb && yb && rb) {
        change_to_one(&(res->bits[j]), i);
        if (i == 31)
          res->bits[j + 1] = 1;
        else
          change_to_one(&(res->bits[j]), i + 1);
      } else if ((xb && yb && !rb) || (xb && rb) || (yb && rb)) {
        change_to_zero(&(res->bits[j]), i);
        if (i == 31)
          res->bits[j + 1] = 1;
        else
          change_to_one(&(res->bits[j]), i + 1);
      } else if (xb || yb) {
        change_to_one(&(res->bits[j]), i);
      }

      if (res->bits[6]) return 1;
    }
  }
  return 0;
}
