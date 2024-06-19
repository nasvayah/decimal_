#include "s21_decimal.h"

int s21_sub(s21_decimal x, s21_decimal y, s21_decimal *res) {
  int comp = is_bigger_mod(x, y);
  int x_sign = get_binary(x.bits[3], 31);
  int y_sign = get_binary(y.bits[3], 31);
  int x_scale = get_scale(x);
  int y_scale = get_scale(y);
  int res_scale = 0;
  int check = 0;
  if (x_scale >= y_scale) {
    res_scale = x_scale;
    s21_decimal tmp = {0};
    s21_decimal ten_scale = get_ten_pow(x_scale - y_scale);
    mul_decimal_mod(y, ten_scale, &tmp);
    y = tmp;
  } else {
    res_scale = y_scale;
    s21_decimal tmp = {0};
    s21_decimal ten_scale = get_ten_pow(y_scale - x_scale);
    mul_decimal_mod(x, ten_scale, &tmp);
    x = tmp;
  }
  if (comp < 0) {
    s21_decimal t = x;
    x = y;
    y = t;
  }
  if (!x_sign && !y_sign) {
    check = sub_decimal_mod(x, y, res);
    if (comp < 0) change_to_one(&(res->bits[3]), 31);
  } else if (x_sign && y_sign) {
    check = sub_decimal_mod(x, y, res);
    if (comp > 0) change_to_one(&(res->bits[3]), 31);
  } else if (!x_sign && y_sign) {
    check = sum_decimal_mod(x, y, res);
  } else if (!y_sign && x_sign) {
    check = sum_decimal_mod(x, y, res);
    change_to_one(&(res->bits[3]), 31);
  }
  int res_sign = get_binary(res->bits[3], 31);
  if (res_sign == 1 && check == 1) check = 2;
  set_scale(res, res_scale);
  delete_ten(res);
  return check;
}

int sub_decimal_mod(s21_decimal x, s21_decimal y, s21_decimal *res) {
  s21_decimal nul = {0};
  *res = nul;

  x.bits[3] = 0;
  y.bits[3] = 0;

  for (int j = 0; j <= 2; j++) {
    int xb, yb;
    for (int i = 0; i <= 31; i++) {
      xb = get_binary(x.bits[j], i);
      yb = get_binary(y.bits[j], i);
      if ((!xb && !yb) || (xb && yb))
        change_to_zero(&(res->bits[j]), i);
      else if (xb && !yb)
        change_to_one(&(res->bits[j]), i);
      else if (yb && !xb) {
        //change_to_one(&x.bits[j], i);
        int q = i+1;
        int g = j;
        if (q == 32){
          q = 0;
          g = j+1;
        }
        while (!get_binary(x.bits[g], q) ){
          change_to_one(&x.bits[g], q);
          q++;
           if (q == 32){
            q = 0;
            g = g+1;
          }
        }
        change_to_zero(&x.bits[g], q);
        change_to_one(&(res->bits[j]), i);
      }

      if (res->bits[3]) return 1;
    }
  }
  return 0;
}

int sub_big_decimal_mod(s21_big_decimal x, s21_big_decimal y,
                        s21_big_decimal *res) {
  s21_big_decimal nul = {0};
  *res = nul;

  x.bits[6] = 0;
  y.bits[6] = 0;

  for (int j = 0; j <= 5; j++) {
    int xb, yb;
    for (int i = 0; i <= 31; i++) {
      xb = get_binary(x.bits[j], i);
      yb = get_binary(y.bits[j], i);
      if ((!xb && !yb) || (xb && yb))
        change_to_zero(&(res->bits[j]), i);
      else if (xb && !yb)
        change_to_one(&(res->bits[j]), i);
      else if (yb && !xb) {
        int q = i + 1;
        int g = j;
        while (!get_binary(x.bits[g], q)) {
          change_to_one(&(x.bits[g]), q);
          q++;
          if (q == 32) {
            g++;
            q = 0;
          }
        }
        change_to_zero(&(x.bits[j]), q);
        change_to_one(&(res->bits[j]), i);
      }

      if (res->bits[6]) return 1;
    }
  }
  return 0;
}
