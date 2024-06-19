#include "s21_decimal.h"

int s21_mul(s21_decimal x, s21_decimal y, s21_decimal *res) {
  int x_sign = get_binary(x.bits[3], 31);
  int y_sign = get_binary(y.bits[3], 31);
  int res_sign = 0;
  int x_scale = get_scale(x);
  int y_scale = get_scale(y);
  set_scale(res, x_scale + y_scale);
  int check = mul_decimal_mod(x, y, res);
  if ((x_sign && !y_sign) || (!x_sign && y_sign)) {
    change_to_one(&(res->bits[3]), 31);
    res_sign = 1;
  }
  set_scale(res, x_scale + y_scale);

  delete_ten(res);

  if (check == 1 && res_sign == 1) check = 2;
  if (check && (x_scale != 0 || y_scale != 0)) {
    s21_truncate(x, &x);
    s21_truncate(y, &y);
    check = s21_mul(x, y, res);
  }
  return check;
}

int mul_big_decimal_mod(s21_big_decimal x, s21_big_decimal y,
                        s21_big_decimal *res) {
  s21_big_decimal nul = {0};
  *res = nul;

  x.bits[6] = 0;
  y.bits[6] = 0;

  int yb;
  s21_big_decimal curr = *res;
  for (int j = 0; j <= 5; j++) {
    for (int i = 0; i <= 31; i++) {
      yb = get_binary(y.bits[j], i);
      if (yb) {
        curr = x;

        s21_big_decimal curcur = *res;
        int c = move_left_big(&curr, i + j * 32);
        s21_big_decimal resres = {0};
        if (c || sum_big_decimal_mod(curcur, curr, &resres)) return 1;

        *res = resres;
      }
    }
  }
  return 0;
}

int mul_decimal_mod(s21_decimal x, s21_decimal y, s21_decimal *res) {
  int res_scale = get_scale(*res);
  s21_decimal nul = {0};
  *res = nul;

  int check = 0;
  s21_big_decimal x_cp = {{0}};
  s21_big_decimal y_cp = {{0}};
  s21_big_decimal res_cp = {{0}};
  dec_to_big_dec(x, &x_cp);
  dec_to_big_dec(y, &y_cp);
  check = mul_big_decimal_mod(x_cp, y_cp, &res_cp);
  if (res_cp.bits[5])
    check = 1;
  set_scale_big(&res_cp, res_scale);

  if (!check) check = big_dec_to_dec(res_cp, res);

  return check;
}
