#include "s21_decimal.h"

int is_bigger_mod(s21_decimal x, s21_decimal y) {
  int xb, yb, res;
  for (int j = 2; j >= 0; j--) {
    for (int i = 31; i >= 0; i--) {
      xb = get_binary(x.bits[j], i);
      yb = get_binary(y.bits[j], i);
      res = xb - yb;
      if (res != 0) break;
    }
    if (res != 0) break;
  }
  return res;
}

int is_bigger_mod_big(s21_big_decimal x, s21_big_decimal y) {
  for (int j = 5; j >= 0; j--) {
    for (int i = 31; i >= 0; i--) {
      if (x.bits[j] > y.bits[j]) return 1;
      if (x.bits[j] < y.bits[j]) return -1;
    }
  }
  return 0;
}

int s21_is_equal(s21_decimal x, s21_decimal y) {
  if (x.bits[0] == 0 && x.bits[1] == 0 && x.bits[2] == 0)
    change_to_zero(&x.bits[3], 31);
  if (y.bits[0] == 0 && y.bits[1] == 0 && y.bits[2] == 0)
    change_to_zero(&y.bits[3], 31);
  delete_ten(&x);
  delete_ten(&y);
  int x_sign = get_binary(x.bits[3], 31);
  int y_sign = get_binary(y.bits[3], 31);
  int x_scale = get_scale(x);
  int y_scale = get_scale(y);
  int res = 0;
  if (x_sign == y_sign && x_scale == y_scale && is_bigger_mod(x, y) == 0)
    res = 1;
  return res;
}
int s21_is_not_equal(s21_decimal x, s21_decimal y) {
  if (x.bits[0] == 0 && x.bits[1] == 0 && x.bits[2] == 0)
    change_to_zero(&x.bits[3], 31);
  if (y.bits[0] == 0 && y.bits[1] == 0 && y.bits[2] == 0)
    change_to_zero(&y.bits[3], 31);
  delete_ten(&x);
  delete_ten(&y);
  int x_sign = get_binary(x.bits[3], 31);
  int y_sign = get_binary(y.bits[3], 31);
  int x_scale = get_scale(x);
  int y_scale = get_scale(y);
  int res = 1;
  if (x_sign == y_sign && x_scale == y_scale && is_bigger_mod(x, y) == 0)
    res = 0;
  return res;
}

int s21_is_greater(s21_decimal x, s21_decimal y) {
  if (x.bits[0] == 0 && x.bits[1] == 0 && x.bits[2] == 0)
    change_to_zero(&x.bits[3], 31);
  if (y.bits[0] == 0 && y.bits[1] == 0 && y.bits[2] == 0)
    change_to_zero(&y.bits[3], 31);
  delete_ten(&x);
  delete_ten(&y);
  int x_sign = get_binary(x.bits[3], 31);
  int y_sign = get_binary(y.bits[3], 31);
  int x_scale = get_scale(x);
  int y_scale = get_scale(y);
  int res = 0;
  if (!x_sign && !y_sign) {
    if (x_scale < y_scale)
      res = 1;
    else if (x_scale == y_scale) {
      if (is_bigger_mod(x, y) == 1) res = 1;
    }

  } else if (x_sign && y_sign) {
    if (x_scale > y_scale)
      res = 1;
    else if (x_scale == y_scale) {
      if (is_bigger_mod(x, y) == -1) res = 1;
    }
  } else {
    if (x_sign < y_sign) res = 1;
  }
  return res;
}

int s21_is_less(s21_decimal x, s21_decimal y) {
  if (x.bits[0] == 0 && x.bits[1] == 0 && x.bits[2] == 0)
    change_to_zero(&x.bits[3], 31);
  if (y.bits[0] == 0 && y.bits[1] == 0 && y.bits[2] == 0)
    change_to_zero(&y.bits[3], 31);
  int y_sign = get_binary(y.bits[3], 31);
  int x_sign = get_binary(x.bits[3], 31);
  delete_ten(&x);
  delete_ten(&y);
  int x_scale = get_scale(x);
  int y_scale = get_scale(y);
  int res = 0;
  if (!x_sign && !y_sign) {
    if (x_scale > y_scale)
      res = 1;
    else if (x_scale == y_scale) {
      if (is_bigger_mod(x, y) == -1) res = 1;
    }

  } else if (x_sign && y_sign) {
    if (x_scale < y_scale)
      res = 1;
    else if (x_scale == y_scale) {
      if (is_bigger_mod(x, y) == 1) res = 1;
    }
  } else {
    if (x_sign > y_sign) res = 1;
  }
  return res;
}

int s21_is_greater_or_equal(s21_decimal x, s21_decimal y) {
  return (s21_is_equal(x, y) || s21_is_greater(x, y));
}

int s21_is_less_or_equal(s21_decimal x, s21_decimal y) {
  return (s21_is_equal(x, y) || s21_is_less(x, y));
}
