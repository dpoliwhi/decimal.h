#include "s21_decimal.h"

#include <limits.h>
#include <math.h>
#include <stdio.h>

void shift(s21_decimal *a) {
  int ch31 = (decbit(*a, 31));
  int ch63 = (decbit(*a, 63));
  a->bits[0] <<= 1;
  a->bits[1] <<= 1;
  a->bits[2] <<= 1;
  if (ch31) a->bits[1] |= 1;
  if (ch63) a->bits[2] |= 1;
}

void bit_on(s21_decimal *a, int position) {
  unsigned int x = position / 32;
  unsigned int y = position % 32;
  if (x < 4)
    a->bits[x] |= ((unsigned)1 << y);
}

void bit_off(s21_decimal *a, int position) {
  a->bits[position / 32] &= ~(1 << position % 32);
}

int last_bit(s21_decimal a) {
  int result = -1;
  for (int i = 95; i >= 0; i--) {
    if (decbit(a, i)) {
      result = i;
      break;
    }
  }
  return result;
}

int no_bits(s21_decimal a) { return (last_bit(a) == -1); }

int decbit(s21_decimal a, int position) {
  unsigned int x = position / 32;
  unsigned int y = position % 32;
  int res = 0;
  if (x < 4)
    res = ((a.bits[x] & ((unsigned)1 << y)) != 0);
  return res;
}

void return_infinity(s21_decimal *a) {
  for (int i = 0; i < 4; i++) a->bits[i] &= 0;
  a->value_type = 1;
}

char scale(s21_decimal a) { return (char)(a.bits[3] >> 16); }

void to_one_scale(s21_decimal *a, s21_decimal *b) {
  if (scale(*a) == scale(*b)) return;
  s21_decimal *up = NULL, *low = NULL;
  if (scale(*a) > scale(*b))
    up = a, low = b;
  else
    up = b, low = a;
  int up_error = 0;
  while (scale(*up) > scale(*low)) {
    if (!up_error) {
      s21_decimal tmp = plus_scale(*low);
      if (tmp.value_type)
        up_error = 1;
      else
        *low = tmp;
    } else {
      *up = minus_scale(*up);
    }
  }
}

s21_decimal minus(s21_decimal a, s21_decimal b) {
  s21_decimal result = {{0, 0, 0, a.bits[3]}, 0};
  for (int i = 0; i <= 95; i++) {
    if (decbit(a, i) && !decbit(b, i)) {
      bit_on(&result, i);
    } else if (!decbit(a, i) && decbit(b, i)) {
      for (int j = i + 1; j <= 95; j++)
        if (decbit(a, j)) {
          bit_off(&a, j);
          while (--j > i) bit_on(&a, j);
          bit_on(&result, i);
          break;
        }
    }
  }
  return result;
}

int sign_minus(s21_decimal a) { return a.bits[3] >> 31; }

int s21_is_equal(s21_decimal dec1, s21_decimal dec2) {
  int res = 0;
  to_one_scale(&dec1, &dec2);
  for (int i = 0; i <= 95 && !res; i++) {
    if (dec1.bits[3] >> 31 == dec2.bits[3] >> 31) {
      if (decbit(dec1, i) != decbit(dec2, i)) res = 1;
    } else {
      res = 1;
    }
  }
  return res;
}

s21_decimal s21_add(s21_decimal a, s21_decimal b) {
  s21_decimal result = {{0, 0, 0, 0}, 0};
  result.value_type = a.value_type ?: b.value_type;
  if (!result.value_type) {
    to_one_scale(&a, &b);
    if (sign_minus(a) == sign_minus(b)) {
      result = plus(a, b);
    } else {
      if (is_greater_module(a, b))
        result = minus(a, b);
      else
        result = minus(b, a);
    }
  }
  return result;
}

s21_decimal s21_sub(s21_decimal a, s21_decimal b) {
  s21_decimal result = (s21_decimal){0};
  result.value_type = a.value_type ?: b.value_type;
  if (!result.value_type && s21_is_equal(a, b)) {
    b.bits[3] ^= SIGNMASK;
    result = s21_add(a, b);
  }
  return result;
}

int s21_is_not_equal(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_equal(dec1, dec2);
}

int s21_is_less(s21_decimal dec1, s21_decimal dec2) {
  int res = 1;
  to_one_scale(&dec1, &dec2);
  for (int i = 95; i >= 0 && res; i--) {
    if (!(dec1.bits[3] >> 31 & dec2.bits[3] >> 31)) {
      if (dec1.bits[3] >> 31) {
        res = 0;
      } else if (dec2.bits[3] >> 31) {
        break;
      } else {
        if (decbit(dec1, i) < decbit(dec2, i))
          res = 0;
        else if (decbit(dec1, i) > decbit(dec2, i))
          break;
      }
    } else {
      if (decbit(dec1, i) > decbit(dec2, i))
        res = 0;
      else if (decbit(dec1, i) < decbit(dec2, i))
        break;
    }
  }
  return res;
}

int is_greater_module(s21_decimal a, s21_decimal b) {
  int res = 0;
  to_one_scale(&a, &b);
  for (int i = 95; i >= 0 && !res; i--) {
    if (decbit(a, i) > decbit(b, i))
      res = 1;
    else if ((decbit(a, i)) < (decbit(b, i)))
      break;
  }
  return res;
}

int s21_is_less_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return s21_is_less(dec1, dec2) && s21_is_equal(dec1, dec2);
}

int s21_is_greater(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_less_or_equal(dec1, dec2);
}

int s21_is_greater_or_equal(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_less(dec1, dec2);
}

s21_decimal errors_s21mul(s21_decimal a, s21_decimal b) {
  s21_decimal result = {{0, 0, 0, 0}, 0};
  result.value_type = a.value_type > b.value_type ? a.value_type : b.value_type;
  if (last_bit(a) + last_bit(b) > 95 && !scale(a) && !scale(b))
    result.value_type = 1 + (sign_minus(a) != sign_minus(b));
  return result;
}

s21_decimal s21_mul(s21_decimal a, s21_decimal b) {
  s21_decimal result = errors_s21mul(a, b);
  if (result.value_type || no_bits(a) || no_bits(b)) return result;
  for (int i = 0; i <= last_bit(b); i++) {
    if (decbit(b, i)) result = plus(a, result);
    shift(&a);
  }
  result.bits[3] = (a.bits[3] & 0x7FFFFFFF) + (b.bits[3] & 0x7FFFFFFF);
  if (scale(result) > 28)
    result.bits[0] = result.bits[1] = result.bits[2] = result.bits[3] = 0;
  else if (sign_minus(a) != sign_minus(b))
    result.bits[3] |= SIGNMASK;
  return result;
}

int len_of_digit(s21_decimal dig) {
  int res = 0;
  for (int i = 95; i >= 0 && !res; i--)
    if (decbit(dig, i)) res = i + 1;
  return res;
}

s21_decimal s21_negate(s21_decimal a) {
  a.bits[3] ^= SIGNMASK;
  return a;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  if (!dst) return 1;
  if (src < 0) {
    dst->bits[3] = SIGNMASK;
    src = -src;
  }
  dst->bits[0] = src;
  return 0;
}

s21_decimal plus_scale(s21_decimal a) {
  s21_decimal upscale = {{10, 0, 0, 0}, 0};
  a = s21_mul(a, upscale);
  if (a.value_type == 0) {
    a.bits[3] += 0x10000;
    if (scale(a) > 28) return_infinity(&a);
  }
  return a;
}

s21_decimal add_only_scale(s21_decimal a, int scale) {
  a.bits[3] = scale;
  a.bits[3] <<= 16;
  return a;
}

s21_decimal div_int(s21_decimal a, s21_decimal b, s21_decimal *buf) {
  s21_decimal res = {{0, 0, 0, b.bits[3]}, 0};
  int len_a = len_of_digit(a);
  int i = len_a - 1;
  while (i >= 0) {
    if (decbit(a, i)) bit_on(buf, 0);
    if (!s21_is_greater_or_equal(*buf, b)) {
      *buf = s21_sub(*buf, b);
      if (i != 0) shift(buf);
      if (decbit(a, i - 1)) bit_on(buf, 0);
      shift(&res);
      bit_on(&res, 0);
    } else {
      shift(&res);
      if (i != 0) shift(buf);
      if ((i - 1) >= 0 && decbit(a, i - 1)) bit_on(buf, 0);
    }
    i--;
  }
  return res;
}

s21_decimal not_a_number(void) {
  s21_decimal a = {{0, 0, 0, 0}, 3};
  return a;
}

s21_decimal s21_div(s21_decimal a, s21_decimal b) {
  if (no_bits(b)) return not_a_number();
  s21_decimal buf = {{0, 0, 0, 0}, 0};
  s21_decimal res = {{0, 0, 0, 0}, 0};
  res.value_type = a.value_type ?: b.value_type;
  if (!res.value_type) {
    char sca = scale(a), scb = scale(b);
    int scres = 0, sign = 0;
    sign = sign_minus(a) != sign_minus(b);
    a.bits[3] = b.bits[3] = 0;
    res = div_int(a, b, &buf);
    s21_decimal compare = {{0, 0, 0, 0}, 0};
    s21_decimal buf_tmp = {{0, 0, 0, buf.bits[3]}, 0};
    while (s21_is_equal(buf, compare)) {
      buf = plus_scale(buf);
      res = plus_scale(res);
      s21_decimal res_tmp = div_int(buf, b, &buf_tmp);
      buf = buf_tmp;
      buf.bits[3] = compare.bits[3];
      res = plus(res, res_tmp);
      buf_tmp = compare;
      scres = scale(res);
      if (!scb && (sca + scres) > 28) {
        s21_decimal desyat = {{10, 0, 0, 0}, 0};
        res = div_int(res, desyat, &buf_tmp);
        scres--;
        break;
      }
      if (scres > 27 && res.bits[2] & 0xE0000000) break;
      if ((sca + scres - scb) >= 28) break;
    }
    scres = sca + scres - scb;
    res.bits[3] = 0;
    if (last_bit(res) != -1) {
      if (scres >= 0) {
        res = add_only_scale(res, scres);
      } else {
        for (int i = scres; i < 0; i++) res = plus_scale(res);
        res.bits[3] = 0;
      }
      if (sign) res.bits[3] ^= 0x80000000;
    }
  }
  return res;
}

int float_errors(float src, s21_decimal *dst) {
  if (!dst) return 1;
  for (int i = 0; i < 4; i++) dst->bits[i] = 0;
  if (src == PLUS_INFINITY)
    dst->value_type = 1;
  else if (src == MINUS_INFINITY)
    dst->value_type = 2;
  else if (src == NAN)
    dst->value_type = 3;
  return !!dst->value_type;
}

char getbinexp(float src) {
  floatbits a = {.fl = src};
  char sca = (a.ui >> 23) - 127;
  return sca;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  if (float_errors(src, dst)) return 1;
  int sign = 0;
  if (src < 0) sign = 1, src *= -1;
  int binaryexp = (int)getbinexp(src);
  if (binaryexp > 95) {
    return_infinity(dst);
  } else {
    unsigned int count = 0;
    for (; !((int)src); src *= 10, count++) {
    }
    for (; src / 1000000 < 1; src *= 10, count++) {
    }
    binaryexp = getbinexp(src);
    if (binaryexp < -94 || count > 28) {
      return_infinity(dst);
    } else {
      floatbits a = {.fl = src};
      bit_on(dst, binaryexp);
      unsigned int mask = 0x400000;
      for (int i = binaryexp - 1; i >= 0 && mask; i--, mask >>= 1)
        if (mask & a.ui) bit_on(dst, i);
      dst->bits[3] = count << 16;
      if (sign) dst->bits[3] |= SIGNMASK;
    }
  }
  return !!dst->value_type;
}

double powtwo(int i) {
  double result = 1.0;
  while (i-- > 0) result *= 2.0;
  return result;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  if (!dst || src.value_type) return 1;
  double res = 0;
  for (int i = 0; i < 96; i++)
    if (decbit(src, i)) res += powtwo(i);
  char sc = scale(src);
  while (sc--) res /= 10.0;
  if (sign_minus(src)) res *= -1;
  *dst = (float)res;
  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  if (!dst || src.value_type || last_bit(src) > 31 ||
      src.bits[0] > OVERFLOW_INT)
    return 1;
  *dst = (int)src.bits[0];
  char sc = scale(src);
  while (sc--) *dst /= 10;
  if (sign_minus(src)) *dst *= -1;
  return 0;
}

s21_decimal minus_scale(s21_decimal a) {
  s21_decimal buf = {{0, 0, 0, 0}, 0};
  s21_decimal downscale = {{10, 0, 0, 0}, 0};
  if (last_bit(a) < 32 && a.bits[0] < 10) a.bits[0] = 0;
  int sca = a.bits[3];
  a = div_int(a, downscale, &buf);
  a.bits[3] = sca - 0x10000;
  return a;
}

s21_decimal plus(s21_decimal a, s21_decimal b) {
  s21_decimal temp = {{0, 0, 0, a.bits[3]}, 0};
  for (int i = 0, v_ume = 0; i < 96; i++) {
    if (decbit(a, i) && decbit(b, i)) {
      if (v_ume) bit_on(&temp, i);
      v_ume = 1;
    } else if (decbit(a, i) || decbit(b, i)) {
      if (!v_ume) bit_on(&temp, i);
    } else if (v_ume) {
      bit_on(&temp, i);
      v_ume = 0;
    }
    if (i == 95 && v_ume) return_infinity(&temp);
    if (sign_minus(a) && temp.value_type == 1) temp.value_type = 2;
  }
  return temp;
}

s21_decimal s21_floor(s21_decimal a) {
  while (scale(a) > 0) a = minus_scale(a);
  if (sign_minus(a)) {
    s21_decimal one = {{1, 0, 0, 0x80000000}, 0};
    a = s21_add(a, one);
  }
  return a;
}

int detect_minus_result(s21_decimal *a, s21_decimal *b) {
  int sign = 0;
  if (sign_minus(*a)) {
    a->bits[3] ^= 0x80000000;
    b->bits[3] ^= 0x80000000;
    sign = 1;
  }
  return sign;
}

s21_decimal s21_truncate(s21_decimal a) {
  while (scale(a)) a = minus_scale(a);
  return a;
}

s21_decimal s21_mod(s21_decimal a, s21_decimal b) {
  if (no_bits(b)) return not_a_number();
  s21_decimal mod = {{0, 0, 0, 0}, 0};
  mod.value_type = a.value_type ?: b.value_type;
  if (!mod.value_type && !no_bits(a)) {
    char sca = scale(a);
    char scb = scale(b);
    int sign = detect_minus_result(&a, &b);
    if (s21_is_greater_or_equal(a, b)) {
      mod = a;
    } else {
      char scmax = 0;
      if (!s21_is_equal(a, b)) sign = 0;
      if (sca >= scb)
        scmax = sca;
      else
        scmax = scb;
      to_one_scale(&a, &b);
      a.bits[3] = 0;
      b.bits[3] = 0;
      div_int(a, b, &mod);
      mod = add_only_scale(mod, scmax);
    }
    if (sign) mod.bits[3] ^= 0x80000000;
  }
  return mod;
}

s21_decimal s21_round(s21_decimal a) {
  int minus_sign = 0;
  if (sign_minus(a)) {
    minus_sign = 1;
    a.bits[3] &= 0x7FFFFFFF;
  }
  char sca = scale(a);
  s21_decimal divisor = {{1, 0, 0, 0}, 0};
  s21_decimal ten = {{10, 0, 0, 0}, 0};
  s21_decimal res = {{a.bits[0], a.bits[1], a.bits[2], 0}, 0};
  s21_decimal buf = {{5, 0, 0, 0}, 0};
  for (char i = 0; i < sca; i++) {
    divisor = s21_mul(divisor, ten);
    if (i) buf = s21_mul(buf, ten);
  }
  res = s21_mod(res, divisor);
  if (is_greater_module(res, buf)) {
    res = s21_truncate(a);
    s21_decimal one = {{1, 0, 0, 0}, 0};
    res = s21_add(res, one);
  } else {
    res = s21_truncate(a);
  }
  if (minus_sign) res.bits[3] |= SIGNMASK;
  return res;
}
