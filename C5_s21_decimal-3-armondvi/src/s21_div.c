#include "s21_decimal.h"

int s21_div(s21_decimal x, s21_decimal y, s21_decimal *res) {
  int x_sign = get_binary(x.bits[3], 31);
  int y_sign = get_binary(y.bits[3], 31);
  int x_scale = get_scale(x);
  int y_scale = get_scale(y);
  int res_sign = 0;
  if ((x_sign && !y_sign) || (!x_sign && y_sign)) {
    change_to_one(&(res->bits[3]), 31);
    res_sign = 1;
  }
  if (x_scale > y_scale) {
    s21_decimal ten_pow = get_ten_pow(x_scale - y_scale);
    mul_decimal_mod(y, ten_pow, &y);
  } else if (y_scale > x_scale) {
    s21_decimal ten_pow = get_ten_pow(y_scale - x_scale);
    mul_decimal_mod(x, ten_pow, &x);
  }
  int check = div_decimal_mod(x, y, res);
  if (check == 1 && res_sign == 1) check = 2;

  delete_ten(res);
  return check;
}

// int div_big_decimal_mod_wo(s21_big_decimal x, s21_big_decimal y,
//                            s21_big_decimal *res) {
//   s21_big_decimal nul = {0};
//   *res = nul;
//   x.bits[6] = 0;
//   y.bits[6] = 0;

//   s21_big_decimal left = {0};
//   int check = 0;

//   while (is_bigger_mod_big(x, y) != -1) {
//     s21_big_decimal tmp = y;
//     int comp = is_bigger_mod_big(x, tmp), d = 0;

//     while (comp != -1 && d < 193) {
//       d++;
//       if (move_left_big(&tmp, 1)) break;
//       comp = is_bigger_mod_big(x, tmp);
//     }
//     tmp = y;
//     move_left_big(&tmp, d - 1);
//     sub_big_decimal_mod(x, tmp, &left);
//     s21_big_decimal counter = {{1, 0, 0, 0, 0, 0, 0}};
//     move_left_big(&counter, d - 1);
//     if (sum_big_decimal_mod(*res, counter, res)) check = 1;
//     x = left;
//   }
//   return check;
// }

int div_big_decimal_mod_wo(s21_big_decimal x_big, s21_big_decimal y_big,
                           s21_big_decimal *res_big) {
  int check = 0;
  while (is_bigger_mod_big(x_big, y_big) != -1){
      s21_big_decimal tmp = y_big;
      int d = 0;
      while (is_bigger_mod_big(x_big, tmp) != -1){
        move_left_big(&tmp, 1);
        d++;
      }
      tmp = y_big;
      move_left_big(&tmp, d-1);
      s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
      move_left_big(&one, d-1);
      check = sum_big_decimal_mod(*res_big, one, res_big);
      sub_big_decimal_mod(x_big, tmp, &x_big);
    }
    return check;
}

s21_decimal div_decimal_mod_wo(s21_decimal x, s21_decimal y, s21_decimal *res) {
  s21_decimal nul = {0};
  *res = nul;
  x.bits[3] = 0;
  y.bits[3] = 0;

  s21_decimal left = {0};
  int check = 0;

  s21_decimal x_copy = x;
  s21_decimal y_copy = y;

  while (is_bigger_mod(x, y) != -1) {
    s21_decimal tmp = y;
    int comp = is_bigger_mod(x, tmp), d = 0;

    while (comp != -1 && d < 97) {
      d++;
      if (move_left(&tmp, 1)) break;
      comp = is_bigger_mod(x, tmp);
    }
    tmp = y;
    move_left(&tmp, d - 1);
    sub_decimal_mod(x, tmp, &left);

    s21_decimal counter = {{1, 0, 0, 0}};
    move_left(&counter, d - 1);
    if (sum_decimal_mod(*res, counter, res)) check = 1;
    x = left;
  }
  if (is_bigger_mod(x_copy, y_copy) == -1) left = x_copy;
  if (check) left.bits[3] = 1;
  return left;
}

// int div_decimal_mod(s21_decimal x, s21_decimal y, s21_decimal *res) {
//   s21_decimal nul = {0};
//   *res = nul;
//   if (!(y.bits[0] || y.bits[1] || y.bits[2])) return 3;
//   int scale = 0;
//   s21_decimal tmp_res = {0};
//   s21_decimal left = div_decimal_mod_wo(x, y, &tmp_res);
//   s21_decimal ten = {{10, 0, 0, 0}};
//   s21_decimal res_copy = {0};
//   s21_decimal left_mul_ten_cp = left;
//   s21_decimal left_mul_ten = {0};
//   if (left.bits[0] || left.bits[1] || left.bits[2]) {
//     while (is_bigger_mod(left_mul_ten, y) == -1) {
//       mul_decimal_mod(left_mul_ten_cp, ten, &left_mul_ten);
//       left_mul_ten_cp = left_mul_ten;
//       scale += 1;
//     }
//   }
//   s21_decimal left_next = div_decimal_mod_wo(left_mul_ten, y, &res_copy);

//   while ((left.bits[0] || left.bits[1] || left.bits[2]) && !left_next.bits[3] &&
//          scale < 28) {
//     x = left_mul_ten;
//     mul_decimal_mod(*res, ten, res);
//     sum_decimal_mod(*res, tmp_res, res);
//     left = div_decimal_mod_wo(x, y, &tmp_res);
//     left_mul_ten_cp = left;
//     while (is_bigger_mod(left_mul_ten, y) == -1) {
//       mul_decimal_mod(left_mul_ten_cp, ten, &left_mul_ten);
//       left_mul_ten_cp = left_mul_ten;
//       scale += 1;
//     }
//     left_next = div_decimal_mod_wo(left_mul_ten, y, &res_copy);
//   }
//   if (left_next.bits[3] || left.bits[3]) return 1;
//   mul_decimal_mod(*res, ten, res);
//   sum_decimal_mod(*res, tmp_res, res);

//   set_scale(res, scale);
//   return 0;
// }




void div_decimal_mod_wo_ok(s21_decimal x, s21_decimal y, s21_decimal *res, s21_decimal *left){
  s21_big_decimal x_big = {{}};
  s21_big_decimal y_big = {{}};
  s21_big_decimal res_big = {{}};
  s21_big_decimal left_big = {{}};
  dec_to_big_dec(x, &x_big);
  dec_to_big_dec(y, &y_big);
  while (is_bigger_mod_big(x_big, y_big) != -1){
    s21_big_decimal tmp = y_big;
    int d = 0;
    while (is_bigger_mod_big(x_big, tmp) != -1){
      move_left_big(&tmp, 1);
      d++;
    }
    tmp = y_big;
    move_left_big(&tmp, d-1);
    s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
    move_left_big(&one, d-1);
    sum_big_decimal_mod(res_big, one, &res_big);
    sub_big_decimal_mod(x_big, tmp, &x_big);
  }
  left_big = x_big;
  big_dec_to_dec(res_big, res);
  big_dec_to_dec(left_big, left);

}


// void div_decimal_mod_wo_ok(s21_decimal x, s21_decimal y, s21_decimal *res, s21_decimal *left){

//   while (is_bigger_mod(x, y) != -1){
//     s21_decimal tmp = y;
//     int d = 0;
//     while (is_bigger_mod(x, tmp) != -1){
//       move_left(&tmp, 1);
//       d++;
//     }
//     tmp = y;
//     move_left(&tmp, d-1);
//     s21_decimal one = {{1, 0, 0, 0}};
//     move_left(&one, d-1);
//     sum_decimal_mod(*res, one, res);
//     print_decimal(x);
//     sub_decimal_mod(x, tmp, &x);
//     //print_decimal(x);
//   }
//   *left = x;


// }


int div_decimal_mod(s21_decimal x, s21_decimal y, s21_decimal *res){
  if (iz_zero(y))
    return 3;
  s21_decimal left = {{}};
  div_decimal_mod_wo_ok(x, y, res, &left);
  int scale = 0;
  s21_decimal ten = {{10, 0, 0, 0}};
  s21_big_decimal ten_big = {{10, 0, 0, 0, 0, 0, 0}};
  int flag = 0;
  while (!iz_zero(left) && !flag && scale < 28){
    flag = mul_decimal_mod(left, ten, &x);
    s21_decimal res_tmp = {{}};
    div_decimal_mod_wo_ok(x, y, &res_tmp, &left);
    s21_big_decimal res_tmp_big = {{}};
    dec_to_big_dec(res_tmp, &res_tmp_big);

    s21_big_decimal res_big = {{}};
    dec_to_big_dec(*res, &res_big);
    mul_big_decimal_mod(res_big, ten_big, &res_big);
    sum_big_decimal_mod(res_big, res_tmp_big, &res_big);
    if (res_big.bits[3]) {flag = 1;}
    big_dec_to_dec(res_big, res);
    scale++;
    flag = mul_decimal_mod(left, ten, &x);
  }
   set_scale(res, scale);

  return 0;
}