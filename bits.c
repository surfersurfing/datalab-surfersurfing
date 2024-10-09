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
    // 当x、y都为0时，输出1
    if (!x && !y)
        return 1;
    // 当x、y仅有一个为0时，输出0
    if (!(x && y))
        return 0;
    // 当x、y都不为0时，异或后，最高位为1则符号不同，最高位为0则符号相同
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

    // 检查高位并累加到结果中
    result = (v > 0xFFFF) << 4;  // 如果 v 大于 16 位
    v >>= result;                // 相应右移

    result |= (v > 0xFF) << 3;  // 如果 v 大于 8 位
    v >>= (v > 0xFF) << 3;      // 相应右移

    result |= (v > 0xF) << 2;  // 如果 v 大于 4 位
    v >>= (v > 0xF) << 2;      // 相应右移

    result |= (v > 0x3) << 1;  // 如果 v 大于 2 位
    v >>= (v > 0x3) << 1;      // 相应右移

    return result | (v >> 1);  // 最后检查最高位，加入到结果中
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
    int n_shift = n << 3;  // n * 8 得到第 n 个字节的位位置
    int m_shift = m << 3;  // m * 8 得到第 m 个字节的位位置

    int mask = (0xFF << n_shift) | (0xFF << m_shift);
    int bytes = ((x >> n_shift) & 0xFF) << m_shift | ((x >> m_shift) & 0xFF) << n_shift;

    return (x & ~mask) | bytes;
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
    // 奇偶位交换
    v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1); 
    // 每2位一组交换
    v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);
    // 每4位一组交换
    v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);
    // 每8位一组交换
    v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);
    // 每16位一组交换
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
    int off = 1 & (!(~x));
    cnt += (!!(~(x >> 16))) << 4;
    cnt += (!!(~(x >> (cnt + 8)))) << 3;
    cnt += (!!(~(x >> (cnt + 4)))) << 2;
    cnt += (!!(~(x >> (cnt + 2)))) << 1;
    cnt += (!!(~(x >> (cnt + 1))));
    return 32 + ~cnt + off;
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
    if (round > 0xFF + 1)
        round = 1;
    else if (round < 0xFF + 1)
        round = 0;
    else
        round = frac & 1;
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
    int exp = (uf >> 23) & 0xFF;               // 提取指数部分
    if (exp == 0)                              // 非规格化数或0
        return (uf << 1) | (uf & 0x80000000);  // 左移一位并保留符号位
    if (exp == 255)                            // 无穷大或NaN
        return uf;
    exp++;                                      // 指数加1（乘以2）
    if (exp == 255)                             // 溢出到无穷大
        return 0x7f800000 | (uf & 0x80000000);  // 返回无穷大并保留符号位
    return (uf & 0x807fffff) | (exp << 23);     // 返回新浮点数：更新后的指数加原符号和小数部分
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
    int s = uf2 & 0x80000000;                          // 符号位
    int exp = (uf2 >> 20) & 0x7FF;                     // 指数部分
    int frac = ((uf2 & 0xFFFFF) << 12) | (uf1 >> 20);  // 合并尾数

    int E = exp - 1023;  // 调整指数
    if (E < 0)
        return 0;  // 指数小于0，表示值小于1，返回0（下溢）

    if (E >= 31)
        return 0x80000000;  // 指数过大，溢出，返回最大值

    frac = (frac | 0x100000) >> (20 - E);  // 恢复隐含的1位，并根据E调整尾数

    return s ? -frac : frac;  // 根据符号返回最终结果
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
    if (x < -149) {
        return 0;  // 结果太小，返回 0
    }
    if (x < -126) {
        // 非规格化数的情况
        return 1 << (149 + x);  // 计算出尾数的二进制表示
    }
    if (x <= 127) {
        // 正常情况，计算指数部分
        return (x + 127) << 23;  // 指数转换为浮点格式
    }
    // 结果太大，返回正无穷大
    return 0x7F800000;  // +INF 的浮点表示
}
