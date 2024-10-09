/*
 * bitXor - x ^ y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 7
 *   Difficulty: 1
 */
int bitXor(int x, int y) {
    return ~(x & y) & ~(~x & ~y);
}

/*
 * samesign - Determines if two integers have the same sign.
 *   0 is not positive, nor negative
 *   Example: samesign(0, 1) = 0, samesign(0, 0) = 1
 *            samesign(-4, -5) = 1, samesign(-4, 5) = 0
 *   Legal ops: >> << ! ^ && if else &
 *   Max ops: 12
 *   Difficulty: 2
 *
 * Parameters:
 *   x - The first integer.
 *   y - The second integer.
 *
 * Returns:
 *   1 if x and y have the same sign , 0 otherwise.
 */
int samesign(int x, int y) {
    // Check if both x and y are zero
    if (!x && !y)
        return 1;
    // Check if either x or y is zero
    if (!(x && y))
        return 0;
    // Check if x and y have the same sign
    return !((x ^ y) >> 31);
}

/*
 * logtwo - Calculate the base-2 logarithm of a positive integer using bit
 *   shifting. (Think about bitCount)
 *   Example: logtwo(32) = 5 (v > 0)
 *   Legal ops: > < >> << |
 *   Max ops: 25
 *   Difficulty: 4
 */
int logtwo(int v) {
    int result = 0;

    // 使用位移与或运算来判断不同的区间，并更新 result
    result = (v > 0xFFFF) << 4;  // 如果数字大于 16 位，则 result 先累积 16
    v >>= result;                // 如果大于 16 位，则右移 16 位

    int shift = (v > 0xFF) << 3;  // 如果大于 8 位，则再累积 8
    result |= shift;
    v >>= shift;

    shift = (v > 0xF) << 2;  // 如果大于 4 位，则再累积 4
    result |= shift;
    v >>= shift;

    shift = (v > 0x3) << 1;  // 如果大于 2 位，则再累积 2
    result |= shift;
    v >>= shift;

    result |= (v >> 1);  // 最后检查最高的 1 位

    return result;
}

/*
 *  byteSwap - swaps the nth byte and the mth byte
 *    Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *              byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *    Note: You may assume that 0 <= n <= 3, 0 <= m <= 3
 *    Legal ops: ! ~ & ^ | + << >>
 *    Max ops: 17
 *    Difficulty: 2
 */
int byteSwap(int x, int n, int m) {
    // Step 1: Shift the n and m positions to extract the bytes
    int n_shift = n << 3;  // n * 8 to get the bit position of nth byte
    int m_shift = m << 3;  // m * 8 to get the bit position of mth byte

    // Step 2: Extract the nth and mth bytes
    int nth_byte = (x >> n_shift) & 0xFF;
    int mth_byte = (x >> m_shift) & 0xFF;

    // Step 3: Clear the nth and mth bytes in the original number
    int mask = (0xFF << n_shift) | (0xFF << m_shift);
    x = x & ~mask;

    // Step 4: Insert the bytes in swapped positions
    x = x | (nth_byte << m_shift) | (mth_byte << n_shift);

    return x;
}

/*
 * reverse - Reverse the bit order of a 32-bit unsigned integer.
 *   Example: reverse(0xFFFF0000) = 0x0000FFFF reverse(0x80000000)=0x1 reverse(0xA0000000)=0x5
 *   Note: You may assume that an unsigned integer is 32 bits long.
 *   Legal ops: << | & - + >> for while ! ~ (You can define unsigned in this function)
 *   Max ops: 30
 *   Difficulty: 3
 */
unsigned reverse(unsigned v) {
    // Swap odd and even bits
    v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1);

    // Swap consecutive pairs
    v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);

    // Swap nibbles (4 bits)
    v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);

    // Swap bytes
    v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);

    // Swap 16-bit halves
    v = (v >> 16) | (v << 16);

    return v;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Note: You can assume that 0 <= n <= 31
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Difficulty: 3
 */
int logicalShift(int x, int n) {
    return (x >> n) & ~(1 << 31 >> n << 1);
}

/*
 * leftBitCount - returns count of number of consective 1's in left-hand (most) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12,
 *             leftBitCount(0xFE00FF0F) = 7
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Difficulty: 4
 */
int leftBitCount(int x) {
    int cnt = 0;
    int off = 1&(!(~x));
    cnt += (!!(~(x>>16)))<<4;
    cnt += (!!(~(x>>(cnt+8))))<<3;
    cnt += (!!(~(x>>(cnt+4))))<<2;
    cnt += (!!(~(x>>(cnt+2))))<<1;
    cnt += (!!(~(x>>(cnt+1))));
    return 32+~cnt+off;
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but it is to be interpreted as
 *   the bit-level representation of a single-precision floating point values.
 *   Legal ops: if else while for & | ~ + - >> << < > ! ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned float_i2f(int x) {
    unsigned sign = x & (1 << 31);
    unsigned exp = 0;
    unsigned frac = 0;
    unsigned round = 0;
    unsigned absX = sign ? (~x + 1) : x;
    unsigned tmp = absX;
    while ((tmp = tmp >> 1))
        ++exp;
    frac = absX << (31 - exp) << 1;
    round = frac << 23 >> 23;
    frac = frac >> 9;
    if (round > 0xFF + 1) round = 1;
    else if (round < 0xFF + 1) round = 0;
    else round = frac & 1;
    return x ? (sign | ((exp + 0x7F) << 23) | frac) + round : 0;
}

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: & >> << | if > < >= <= ! ~ else + ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatScale2(unsigned uf) {
    int exp = (uf & 0x7f800000) >> 23;
    int sign = uf & (1 << 31);
    if (exp == 0)
        return uf << 1 | sign;
    if (exp == 255)
        return uf;
    exp++;
    if (exp == 255)
        return 0x7f800000 | sign;
    return (exp << 23) | (uf & 0x807fffff);
}

/*
 * float64_f2i - Convert a 64-bit IEEE 754 floating-point number to a 32-bit signed integer.
 *   The conversion rounds towards zero.
 *   Note: Assumes IEEE 754 representation and standard two's complement integer format.
 *   Parameters:
 *     uf1 - The lower 32 bits of the 64-bit floating-point number.
 *     uf2 - The higher 32 bits of the 64-bit floating-point number.
 *   Returns:
 *     The converted integer value, or 0x80000000 on overflow, or 0 on underflow.
 *   Legal ops: >> << | & ~ ! + - > < >= <= if else
 *   Max ops: 60
 *   Difficulty: 3
 */
int float64_f2i(unsigned uf1, unsigned uf2) {
    int s = uf2 & 0x80000000;  // Sign
    int exp = (uf2 >> 20) & 0x7FF;  // Exponent
    int M1 = uf2 & 0xFFFFF;  // Higher part of the fraction
    int M2 = uf1 >> 12;  // Lower part of the fraction

    // // Check for special cases
    // if (exp == 0x7FF) return 0x80000000;  // NaN or Inf
    // if (exp == 0) return 0;  // Zero

    // Calculate E and M
    int E = exp - 1023;  // Adjust exponent
    int M = (M1 << 12) | M2;  // Combine M1 and M2
    if (E < 0) return 0;  // Underflow

    // Handle overflow
    if (E >= 32) return 0x80000000;  // Overflow

    // Shift M to the right position and apply the sign
    M |= s;
    return M >> (E - 31);  // Align to integer
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: < > <= >= << >> + - & | ~ ! if else &&
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatPower2(int x) {
        unsigned expo;
    unsigned frac;

    if (x < -149) {
        return 0;
    }
    // denormalizado
    if (x < -126 && x >= -149) {
        int corrimiento = (-x - 126);
        frac = 1 << (23 - corrimiento);
        return frac;
    }
    if (x >= -126 && x <= 127) {
        expo = (x + 127) << 23;
        return expo;
    }
    if (x > 127) {
        // infinito
        return 0xFF << 23;
        ;
    }
    return 0;
}
