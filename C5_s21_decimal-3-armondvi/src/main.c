// #include "s21_decimal.h"

// int main(){
//   s21_decimal dec1 = {0};

//   dec1.bits[0] = 0b00000000000000000000000001101101;
//   dec1.bits[1] = 0b00000000000011000110010101011011;
//   dec1.bits[2] = 0b00000000000000000011000000111001;
//   dec1.bits[3] = 0b00000000000011100000000000000000;
//   s21_decimal result = {0};
//   s21_decimal ten = get_ten_pow(14);
//   result.bits[0] = 0b10000111101111000001011000011110;
//   result.bits[1] = 0b00000000000000000000000000000000;
//   result.bits[2] = 0b00000000000000000000000000000000;
//   result.bits[3] = 0b00000000000000000000000000000000;
//   s21_decimal res1 = {0};
//   //s21_truncate(dec1, &res1);
//   print_decimal(dec1);
//   print_decimal(ten);
//   div_decimal_mod_wo(dec1, ten, &res1);
// //   print_decimal(result);
//   print_decimal(res1);

// }