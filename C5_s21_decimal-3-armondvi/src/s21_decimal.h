#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

typedef struct {
  unsigned int bits[4];
} s21_decimal;
typedef struct {
  unsigned int bits[7];
} s21_big_decimal;

int get_bit(unsigned int src, int pos);
void set_bit(unsigned int *dst, int pos, int value);
void set_sign(s21_decimal *dst, int sign);
int get_sign(s21_decimal src);
int get_pow(s21_decimal src) ;
int set_pow(s21_decimal *dst, int value);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void print_binary(int x);
void print_decimal(s21_decimal x);
int get_binary(unsigned int x, unsigned int i);
void change_to_one(unsigned int *res, int i);
void change_to_zero(unsigned int *res, int i);
int sum_binary(int x, int y);
int sub_decimal_mod(s21_decimal x, s21_decimal y, s21_decimal *res);
int sum_decimal_mod(s21_decimal x, s21_decimal y, s21_decimal *res);
int is_bigger_mod(s21_decimal x, s21_decimal y);
int is_bigger_mod_big(s21_big_decimal x, s21_big_decimal y);
int s21_sub(s21_decimal x, s21_decimal y, s21_decimal *res);
int move_left(s21_decimal *x, int d);
int move_left_big(s21_big_decimal *x, int d);
void set_scale(s21_decimal *x, int scale);
int get_scale(s21_decimal x);
int s21_add(s21_decimal x, s21_decimal y, s21_decimal *res);
int mul_decimal_mod(s21_decimal x, s21_decimal y, s21_decimal *res);
int s21_mul(s21_decimal x, s21_decimal y, s21_decimal *res);
s21_decimal get_ten_pow(int x);
s21_decimal div_decimal_mod_wo(s21_decimal x, s21_decimal y, s21_decimal *res);
int div_decimal_mod(s21_decimal x, s21_decimal y, s21_decimal *res);
void delete_ten(s21_decimal *res);
int s21_div(s21_decimal x, s21_decimal y, s21_decimal *res);
int s21_is_less(s21_decimal x, s21_decimal y);
int s21_is_less_or_equal(s21_decimal x, s21_decimal y);
int s21_is_greater(s21_decimal x, s21_decimal y);
int s21_is_greater_or_equal(s21_decimal x, s21_decimal y);
int s21_is_equal(s21_decimal x, s21_decimal y);
int s21_is_not_equal(s21_decimal x, s21_decimal y);
int s21_negate(s21_decimal x, s21_decimal *res);
int s21_truncate(s21_decimal x, s21_decimal *res);
int s21_round(s21_decimal x, s21_decimal *res);
int s21_floor(s21_decimal x, s21_decimal *res);
int s21_from_int_to_decimal(int x, s21_decimal *res);
int s21_from_float_to_decimal(float x, s21_decimal *res);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal x, float *res);
int check_zeros(char *s);
int iz_zero(s21_decimal x);

int get_scale_big(s21_big_decimal x);
void set_scale_big(s21_big_decimal *x, int scale);
void print_big_decimal(s21_big_decimal x);
int sum_big_decimal_mod(s21_big_decimal x, s21_big_decimal y,
                        s21_big_decimal *res);
int sub_big_decimal_mod(s21_big_decimal x, s21_big_decimal y,
                        s21_big_decimal *res);
int div_big_decimal_mod_wo(s21_big_decimal x, s21_big_decimal y,
                           s21_big_decimal *res);
int mul_big_decimal_mod(s21_big_decimal x, s21_big_decimal y,
                        s21_big_decimal *res);

void dec_to_big_dec(s21_decimal x, s21_big_decimal *res);
int big_dec_to_dec(s21_big_decimal x, s21_decimal *res);



void div_decimal_mod_wo_ok(s21_decimal x, s21_decimal y, s21_decimal *res, s21_decimal *left);
#endif  // S21_DECIMAL_H
