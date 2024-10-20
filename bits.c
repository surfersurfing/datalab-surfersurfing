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
    if (!x && !y)  // 当x、y都为0时，输出1
        return 1;
    if (!(x && y))  // 当x、y仅有一个为0时，输出0
        return 0;
    return !((x ^ y) >> 31);  // 当x、y都不为0时，异或后，最高位为1则符号不同，最高位为0则符号相同
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
    int result = 0;              // 检查高位并累加到结果中
    result = (v > 0xFFFF) << 4;  // 如果 v 大于 16 位
    v >>= result;                // 相应右移

    int tmp = (v > 0xFF) << 3;
    result |= tmp;              // 如果 v 大于 8 位
    v >>= tmp;                  // 相应右移

    tmp = (v > 0xF) << 2;
    result |= tmp;  // 如果 v 大于 4 位
    v >>= tmp;      // 相应右移

    tmp = (v > 0x3) << 1;
    result |= tmp;  // 如果 v 大于 2 位
    v >>= tmp;      // 相应右移

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
    v = (v >> 16) | (v << 16);                              // 每16位一组交换
    v = ((v >> 8) & 0x00FF00FF) | ((v & 0x00FF00FF) << 8);  // 每8位一组交换
    v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4);  // 每4位一组交换
    v = ((v >> 2) & 0x33333333) | ((v & 0x33333333) << 2);  // 每2位一组交换
    v = ((v >> 1) & 0x55555555) | ((v & 0x55555555) << 1);  // 奇偶位交换
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
    int cnt = 0;  // 初始化计数器 cnt，用于记录左边连续 1 的数量。

    // off 用于处理全 1 的特殊情况。如果 x 全是 1 (~x 全是 0)，off = 1，否则 off = 0。
    // (~x) 是 x 按位取反，!(~x) 是取反后的值再进行逻辑非运算，结果为 1 表示 x 全是 1，否则为 0。
    int off = 1 & (!(~x));

    // 检查 x 最高 16 位是否存在 0，如果存在 0，说明前 16 位不是全 1，则向右移 16 位并更新计数器。
    // (!!(~(x >> 16))) 用于判断 x 右移 16 位后的补码是否存在 0。若存在 0，结果为 1；否则为 0。
    // 结果乘以 16（即左移 4 位）以更新计数器 cnt。
    cnt += (!!(~(x >> 16))) << 4;

    // 接下来根据 cnt + 8 检查接下来的 8 位是否存在 0。
    // (!!(~(x >> (cnt + 8)))) 判断在 cnt 位置右移 8 位后的 x 是否存在 0。
    cnt += (!!(~(x >> (cnt + 8)))) << 3;

    // 再根据 cnt + 4 检查接下来的 4 位是否存在 0。
    cnt += (!!(~(x >> (cnt + 4)))) << 2;

    // 根据 cnt + 2 检查接下来的 2 位是否存在 0。
    cnt += (!!(~(x >> (cnt + 2)))) << 1;

    // 最后根据 cnt + 1 检查接下来的一位是否存在 0。
    cnt += (!!(~(x >> (cnt + 1))));

    // 返回结果是 32 减去 cnt 加上 off。off 的作用是处理全 1 的情况，如果 x 全是 1，结果会多加 1。
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
    if (x == 0)
        return 0;

    unsigned sign = x & (1 << 31);        // 获取符号位，x 的最高位用于表示正负
    unsigned exp = 0;                     // 指数部分，后续通过移位确定
    unsigned frac = 0;                    // 尾数部分，将绝对值表示为尾数
    unsigned round = 0;                   // 处理舍入用的中间变量
    unsigned absX = sign ? (~x + 1) : x;  // 取 x 的绝对值，如果 x 是负数则取反加 1
    unsigned tmp = absX;                  // 临时变量用于计算指数

    // 计算指数：每次右移 absX，直到 tmp 变为 0
    // 统计移位次数，即求出 absX 是几位数，存储到 exp
    while ((tmp = tmp >> 1))
        ++exp;

    // 用cnt实现exp计算，会超运算符使用
    // int cnt = ((tmp >> 16)) << 4;
    // exp += cnt;
    // cnt = ((tmp >> (8 + cnt)) != 0) << 3;
    // exp += cnt;
    // cnt = ((tmp >> (4 + cnt)) != 0) << 2;
    // exp += cnt;
    // cnt = ((tmp >> (2 + cnt)) != 0) << 1;
    // exp += cnt;
    // cnt = ((tmp >> (1 + cnt)) != 0);
    // exp += cnt;
    

    // 计算尾数部分：将 absX 左移以得到尾数
    // 31 - exp 是为了移到最高位，并且再左移 1 位用于精度保留
    frac = absX << (31 - exp) << 1;

    // 提取需要用来舍入的部分：尾数的低位部分，用于决定是否舍入
    round = frac & 0x1FF;

    // 将尾数右移 9 位，保留高 23 位的有效部分
    frac = frac >> 9;

    // 舍入规则：如果舍入部分大于 0xFF + 1 = 0x100，则进 1，否则根据舍入部分大小决定是否进位
    // 当舍入部分恰好等于 0xFF + 1 时，采用尾数的最低位决定是否进位
    round = (round > 0x100) + ((round == 0x100) & (frac & 1));

    // 返回浮点数格式：符号位 | 指数部分 | 尾数部分，最后加上舍入值
    return (sign | ((exp + 0x7F) << 23) | frac) + round;
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
    int exp = uf & 0x7f800000;     // 提取指数部分
    int sign = (uf & 0x80000000);  // 提取符号位
    if (exp == 0)                  // 非规格化数或0
        return (uf << 1) | sign;   // 左移一位并保留符号位
    if (exp == 0x7f800000)         // 无穷大或NaN
        return uf;
    exp += 0x800000;                 // 指数加1（乘以2）
    if (exp == 0x7f800000)           // 溢出到无穷大
        return 0x7f800000 | sign;    // 返回无穷大并保留符号位
    return (uf & 0x807fffff) | exp;  // 返回新浮点数：更新后的指数加原符号和小数部分
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
    int s = uf2 & 0x80000000;                                    // 符号位
    int exp = (uf2 >> 20) & 0x7FF;                               // 指数部分
    int frac = ((uf2 & 0xFFFFF) << 12) | ((uf1 >> 20) & 0xFFF);  // 合并尾数

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
    if (x < -149)  // 结果太小，返回 0
        return 0;
    if (x < -126)                // 非规格化数的情况
        return 1 << (149 + x);   // 计算出尾数的二进制表示
    if (x < 128)                 // 正常情况，计算指数部分
        return (x + 127) << 23;  // 指数转换为浮点格式
    return 0x7F800000;           // 结果太大，返回正无穷大
}
