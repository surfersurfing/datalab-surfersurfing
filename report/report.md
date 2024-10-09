# datalab 报告

姓名：李纪仪

学号：2022201543

| 总分 | bitXor | logtwo | byteSwap | reverse | logical |  |  |  |  |  |  |
| ---- | ------ | ------ | -------- | ------- | ------- | - | - | - | - | - | - |
| 0    | 1      | 0.00   | 0.00     | 0.00    | ···  |  |  |  |  |  |  |

test 截图：

![图片描述](imgs/image.png "test 截图")

<!-- TODO: 用一个通过的截图，本地图片，放到 imgs 文件夹下，不要用这个 github，pandoc 解析可能有问题 -->

## 解题报告

### 亮点

<!-- 告诉助教哪些函数是你实现得最优秀的，比如你可以排序。不需要展开，展开请放到后文中。 -->

### 1. bitXor

```c
int bitXor(int x, int y) {
    return ~(x & y) & ~(~x & ~y);
}
```

思路：

- 观察真值表：

| x | y | Xor |
| - | - | --- |
| 0 | 0 | 0   |
| 0 | 1 | 1   |
| 1 | 0 | 1   |
| 1 | 1 | 0   |

- 异或可以表达为：

$$
\begin{align}
x\wedge y 
&= \sim((x\&y)|(\sim x\&\sim y))\\
&=  \sim(x\&y)\&\sim(\sim x\&\sim y) 
\end{align}
$$

### 2. samesign

```c
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
```

思路：

- c语言整数大小为4byte，符号位为最高位
- 使用异或，通过最高位判断符号是否相同： `!((x^y)>>31)`
- 考虑异常情况：
  - 0既不是正数也不是负数
  - 当x，y都为0时，输出1
  - 当x，y仅有一个为0时，输出0

### 3. logtwo

```c
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
```

思路：

- 题目给出 v 是一个正数，则答案的整数部分即为 v 中最高位 1 的位置 -1
- 使用二分的思路来寻找答案：
  - result = 0
  - 当 v 大于16位时，整数部分至少为16，否则整数部分必小于16，故：``result += （v > 0xFFFF) << 4``
  - 若大于16位，右移16位：``v >>= result``
  - 同理依次考虑8、4、2、1位的情况

### 4. byteSwap

```c
int byteSwap(int x, int n, int m) {
    int n_shift = n << 3;  // n * 8 得到第 n 个字节的位位置
    int m_shift = m << 3;  // m * 8 得到第 m 个字节的位位置

    int mask = (0xFF << n_shift) | (0xFF << m_shift);
    int bytes = ((x >> n_shift) & 0xFF) << m_shift | ((x >> m_shift) & 0xFF) << n_shift;

    return (x & ~mask) | bytes;
}
```

思路：

- 提取第n、m字节
- 将原始数字x的n、m位置字节置为0
- 用或运算将字节插入交换后的位置

### 5. reverse

```c
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
```

思路：

分五步进行：

- 交换相邻奇、偶位比特
- 以每2位比特为一组，交换相邻两组
- 以每4位比特为一组，交换相邻两组
- 以每8位比特为一组，交换相邻两组
- 以每16位比特为一组，交换相邻两组

对相邻两组交换，通过位运算和掩码实现，下面以每四位一组交换为例说明：

- 掩码为：0x0F0F 0F0F
- 实例：对 v = 0x2220 1543
  - v先右移4位后将原先位置的字节移动到要交换的位置：``(v >> 4)``，得到：0x0222 0154
  - 再使用掩码将原先位置的字节抹零：``(v >> 4) & 0x0F0F0F0F)``，得到：0x0202 0104
  - 对称的左移，抹零：``((v & 0x0F0F0F0F) << 4)``，得到：0x2000 5030
  - 再通过或运算，最终实现相邻两组的交换：``v = ((v >> 4) & 0x0F0F0F0F) | ((v & 0x0F0F0F0F) << 4)``，得到：0x2202 5134

### 6. logicalShift

```c
int logicalShift(int x, int n) {
    return (x >> n) & ~(1 << 31 >> n << 1);
}
```

思路：

- x 右移 n 位后，用 1 00...00 1111 (注：有 n 个 0 ) 去把算术右移的的符号位扩展置为 0

### 7. leftBitCount

```c
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
```

思路：

1

### 8. float_i2f

```c
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
```

思路：

1

### 9. floatScale2

```c
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
```

思路：
1

### 10. float64_f2i

```c
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
```

思路：

1

### 11. floatPower2

```c
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
```

思路：

1

## 反馈/收获/感悟/总结

<!-- 这一节，你可以简单描述你在这个 lab 上花费的时间/你认为的难度/你认为不合理的地方/你认为有趣的地方 -->

<!-- 或者是收获/感悟/总结 -->

<!-- 200 字以内，可以不写 -->

## 参考的重要资料

<!-- 有哪些文章/论文/PPT/课本对你的实现有重要启发或者帮助，或者是你直接引用了某个方法 -->

<!-- 请附上文章标题和可访问的网页路径 -->
