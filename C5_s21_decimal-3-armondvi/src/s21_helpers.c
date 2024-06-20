#include "s21_decimal.h"

void print_binary(int x) {
  for (int i = 31; i >= 0; i--) printf("%d", (x >> i) & 1);
}

void print_decimal(s21_decimal x) {
  for (int j = 3; j >= 0; j--) {
    for (int i = 31; i >= 0; i--) printf("%d", (x.bits[j] >> i) & 1);
    printf(" ");
  }
  printf("\n");
}

void print_big_decimal(s21_big_decimal x) {
  for (int j = 6; j >= 0; j--) {
    for (int i = 31; i >= 0; i--) printf("%d", (x.bits[j] >> i) & 1);
    printf(" ");
  }
  printf("\n");
}

int get_binary(unsigned int x, int i) {
  if ((x >> i) & 1) return 1;
  return 0;
}

void change_to_one(unsigned int *res, int i) { *res |= (1 << i); }

void change_to_zero(unsigned int *res, int i) { *res &= ~(1 << i); }

void dec_to_big_dec(s21_decimal x, s21_big_decimal *res) {
  for (int j = 3; j >= 0; j--) {
    for (int i = 31; i >= 0; i--) {
      if (get_binary(x.bits[j], i)) {
        if (j != 3)
          change_to_one(&res->bits[j], i);
        else
          change_to_one(&res->bits[7], i);
      }
    }
  }
}

int big_dec_to_dec(s21_big_decimal x, s21_decimal *res) {
  res->bits[3] = x.bits[6];
  s21_big_decimal ten = {{10, 0, 0, 0, 0, 0, 0}};
  int scale = get_scale(*res);
  while ((x.bits[5] || x.bits[4] || x.bits[3]) && scale != 0) {
    scale--;
    set_scale(res, scale);
    div_big_decimal_mod_wo(x, ten, &x);
  }
  if (x.bits[5] || x.bits[4] || x.bits[3]) return 1;
  for (int j = 2; j >= 0; j--) {
    res->bits[j] = x.bits[j];
  }

  return 0;
}

int move_left(s21_decimal *x, int d) {
  if (d) {
    if (x->bits[2] & (1 << (31 - (d - 1)))) return 1;
    if (d >= 32 && d < 64) {
      x->bits[2] = x->bits[1];
      x->bits[1] = x->bits[0];
      x->bits[0] = 0;
      d = d - 32;
    } else if (d >= 64) {
      x->bits[2] = x->bits[0];
      x->bits[1] = 0;
      x->bits[0] = 0;
      d = d - 64;
    }
    for (int i = 2; i >= 1; i--) {
      x->bits[i] = x->bits[i] << d;
      x->bits[i] |= x->bits[i - 1] >> (32 - d);
    }
    x->bits[0] = x->bits[0] << d;
  }
  return 0;
}

int move_left_big(s21_big_decimal *x, int d) {
  if (x->bits[5] & (1 << (31 - (d - 1)))) return 1;
  if (d >= 32) {
    int move = d / 32;
    d = d - move * 32;
    // printf("%d\n", d);
    for (int j = 5; j >= move; j--) {
      x->bits[j] = x->bits[j - move];
    }
    for (int j = 0; j < move; j++) {
      x->bits[j] = 0;
    }
  }
  if (d) {
    for (int i = 5; i >= 1; i--) {
      x->bits[i] = x->bits[i] << d;
      x->bits[i] |= x->bits[i - 1] >> (32 - d);
    }
    x->bits[0] = x->bits[0] << d;
  }
  return 0;
}

void set_scale(s21_decimal *x, int scale) {
  int sign = get_binary(x->bits[3], 31);
  x->bits[3] = 0;
  x->bits[3] |= (scale << 16);
  if (sign) change_to_one(&(x->bits[3]), 31);
}

void set_scale_big(s21_big_decimal *x, int scale) {
  int sign = get_binary(x->bits[6], 31);
  x->bits[6] = 0;
  x->bits[6] |= (scale << 16);
  if (sign) change_to_one(&(x->bits[6]), 31);
}
int get_scale(s21_decimal x) {
  change_to_zero(&(x.bits[3]), 31);
  int res = x.bits[3] >> 16;
  if (res > 28) res = 0;
  return res;
}

s21_decimal get_ten_pow(int x) {
  s21_decimal tmp = {{1, 0, 0, 0}};
  s21_decimal ten = {{10, 0, 0, 0}};
  for (int i = 0; i < x; i++) {
    s21_decimal res = {0};
    mul_decimal_mod(tmp, ten, &res);
    tmp = res;
  }
  return tmp;
}

void delete_ten(s21_decimal *res) {
  s21_decimal res_cp = *res;
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_decimal left = {0};
  div_decimal_mod_wo_ok(res_cp, ten, &res_cp, &left);
  int scale = get_scale(*res);
  while (scale > 0 && !(left.bits[0] || left.bits[1] || left.bits[2])) {
    *res = res_cp;
    scale--;
    set_scale(res, scale);
    div_decimal_mod_wo_ok(res_cp, ten, &res_cp, &left);
  }
}

int iz_zero(s21_decimal x){
  int res = 1;
  if (x.bits[0] || x.bits[1] || x.bits[2])
    res = 0;
  return res;
}
