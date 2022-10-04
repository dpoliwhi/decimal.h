#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#define SIGNMASK 0x80000000
#define OVERFLOW_INT 0x7FFFFFFF
#define MINUS_INFINITY 0xFF800000
#define PLUS_INFINITY 0x7F800000

typedef enum {
  s21_NORMAL_VALUE = 0,
  s21_INFINITY = 1,
  s21_NEGATIVE_INFINITY = 2,
  s21_NAN = 3
} value_type_t;

typedef struct {
  unsigned int bits[4];
  value_type_t value_type;
} s21_decimal;

typedef union {
  unsigned int ui;
  float fl;
} floatbits;

long long decimal_to_longlong(s21_decimal a);
void shift(s21_decimal *a);
void bit_on(s21_decimal *a, int position);
void bit_off(s21_decimal *a, int position);
int last_bit(s21_decimal a);
int no_bits(s21_decimal a);
int decbit(s21_decimal a, int position);
void return_infinity(s21_decimal *a);
char scale(s21_decimal a);
void to_one_scale(s21_decimal *a, s21_decimal *b);
s21_decimal minus(s21_decimal a, s21_decimal b);
int sign_minus(s21_decimal a);
int s21_is_equal(s21_decimal dec1, s21_decimal dec2);
s21_decimal s21_add(s21_decimal a, s21_decimal b);
s21_decimal s21_sub(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_less(s21_decimal dec1, s21_decimal dec2);
int is_greater_module(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2);
int s21_is_greater(s21_decimal dec1, s21_decimal dec2);
int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2);
s21_decimal errors_s21mul(s21_decimal a, s21_decimal b);
s21_decimal s21_mul(s21_decimal a, s21_decimal b);
int len_of_digit(s21_decimal dig);
int scale_decbit(s21_decimal a, int position);
s21_decimal s21_negate(s21_decimal a);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
s21_decimal plus_scale(s21_decimal a);
s21_decimal add_only_scale(s21_decimal a, int scale);
s21_decimal div_int(s21_decimal a, s21_decimal b, s21_decimal *buf);
s21_decimal not_a_number(void);
s21_decimal s21_div(s21_decimal a, s21_decimal b);
int float_errors(float src, s21_decimal *dst);
void print(s21_decimal a);
void printfloat(float fff);
char getbinexp(float src);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
double powtwo(int i);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
s21_decimal minus_scale(s21_decimal a);
s21_decimal plus(s21_decimal a, s21_decimal b);
s21_decimal s21_floor(s21_decimal a);
int detect_minus_result(s21_decimal *a, s21_decimal *b);
s21_decimal s21_truncate(s21_decimal a);
s21_decimal s21_mod(s21_decimal a, s21_decimal b);
s21_decimal s21_round(s21_decimal a);

#endif  // SRC_S21_DECIMAL_H_
