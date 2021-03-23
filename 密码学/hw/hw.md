# 7-1 Advanced Engima Cracking (10 分)

## 题目要求

本题目需要模拟当年破解Enigma的过程，现在我们已经知道Plugboard, RingSetting, CipherText 和 Word

已知 PlainText 中一定包含单词 Word， 求破解 MessageKey, PlainText 和 RotorNum

由于完全破解耗时太长，输入还会给出3个齿轮中最左侧那个齿轮的编号。

### 提示

建议你实现一个加密/解密函数 与http://10.71.45.100/bhh/MyEnigma.exe 产生的结果相对比 完全一致后再考虑破解

提示：函数返回值不能是局部变量的地址。

这里有你可能用到的常量：

```c
      char reflector[]="YRUHQSLDPXNGOKMIEBFZCWVJAT"; 
      char rotor_table[5][27] = 
      {
         "EKMFLGDQVZNTOWYHXUSPAIBRCJ",
         "AJDKSIRUXBLHWTMCQGZNPYFVOE",
         "BDFHJLCPRTXVZNYEIWGAKMUSQO",
         "ESOVPZJAYQUIRHXLNFTGKDCMWB",
         "VZBRGITYUPSDNHLXAWMJQOFECK"
      };
      char step_char[5]="RFWKA"; // Royal Flags Wave Kings Above
```

### 输入格式:

输入分为5行

第一行给出Plugboard，以空格分隔的10组， 如`PO LM IU JK NH YT GB VF RE DC`

第二行给出RingSetting，如`TIP` (T为左边齿轮内部设置，P为右边齿轮内部设置)

第三行给出需要破解的密文，如`MNZUXZEZWCAYJVTODAEFBVKYXW`

第四行给出已知明文中包含的单词，如`CRYPTO`

第五行给出最左侧的齿轮编号，如`1`

密文长度不超过500，单词长度不超过20

### 输出格式:

本题保证有且仅有唯一解

假定破解出的MessageKey=XXX, PlainText=YYYYY, RotorNum=123 (1为左边齿轮编号，5为右边齿轮编号),

则程序输出格式如下:

```
MessageKey=XXX
PlainText=YYYYY
RotorNum=123
```

### 输入样例:

```in
PO LM IU JK NH YT GB VF RE DC
TIP
MNZUXZEZWCAYJVTODAEFBVKYXW
CRYPTO
1
```

### 输出样例:

```out
MessageKey=CRY
PlainText=CRYPTOISSOINTERESTINGISNIT
RotorNum=125
```

# 6-1 mydes part1 (10 分)

下载 http://10.71.45.100/bhh/mydes.c

## 1. 重写DES算法中的核心函数:

```c
    static long32 f(ulong32 r, unsigned char subkey[8]) {
       unsigned char s[4], plaintext[8], m[4], t[4];
       unsigned long int rval;
       (1) 复制r到s, 即把r当成4个字节看待(注意大小端)。
       (2) 根据plaintext_32bit_expanded_to_48bit_table这张表, 把s中的4字节共32位扩展成48位并
           保存到数组plaintext中; /* plaintext每个元素的左边2位恒为0, 右边6位用来保存数据 */
           32位转48位的过程要求使用双重循环来做,外循环8次,内循环6次,其中内循环每次只提取1位;
           注意提取某1位的时候只能从数组s中取(要计算该位是第几个字节中的第几位), 不得从参数
           r中获取; 判断某位是否为1的时候可以使用表bytebit。
       (3) 把plaintext中的8个字节与subkey中8个字节异或, 异或后的值保存到plaintext内;
            该过程使用8次循环来做。
       (4) 取出plaintext[i]查表sbox[i]得4位数, 循环8次可得8个4位数, 按从左到右顺序合并
            得32位数保存到数组m中。
       (5) 根据sbox_perm_table把m中的32位数打乱保存到数组t中, 该过程需要使用32次循环来做,
            每次循环提取1位。
       (6) 复制t到rval(注意大小端), 并返回rval。
    }
```

程序中除了可以查上述描述中提到的4张表, 即 `plaintext_32bit_expanded_to_48bit_table` `bytebit` `sbox` `sbox_perm_table` 不能查其它表，也不可以生成其它表。

### 函数接口定义：

```c++
static long32 f(ulong32 r, unsigned char subkey[8])
```

### 裁判测试程序样例：

```c++
int main()
{
   char mykey[9];
   char p[9];
   unsigned char c[9];
   int i;
   scanf("%s %s", mykey, p);
   des_init(0);
   des_set_key(mykey);
   strcpy((char *)c, p);
   des_encrypt((char *)c);
   des_done();
   for(i=0; i<8; i++)
   {
      printf("%02X ", c[i]);
   }
   des_init(0);
   des_set_key(mykey);
   des_decrypt((char *)c);
   des_done();
   puts((char *)c);
}

//其他部分参见mydes.c 你只需要提交f函数
```

### 输入样例：

**这是一个函数题，你只需要并且只能提交你修改后的`f函数`，只要改写不影响原逻辑都能在PTA得分，最终给分将由人工判题给出（判断是否符合题目要求）**

以下输入输出你无需关心，其逻辑由main函数实现：输入密钥key和明文p，输出加密得到的十六进制字符串以及解密得到明文

```in
ABCD1234 abcdefgh
```

### 输出样例：

```out
6B 4A 17 64 E9 F2 2D 4F abcdefgh
```

# 6-2 mydes part2 (10 分)

下载 http://10.71.45.100/bhh/mydes.c

## 2. 改写perm_init()函数及permute()函数

把以下两个指针定义

```c
      static char (*iperm)[16][8];
      static char (*fperm)[16][8];
```

改成:

```c
      static char (*iperm)[256][8];
      static char (*fperm)[256][8];
```

目的是把原先64位划分成16组(每组4位)改成64位划分成8组(每组8位)。注意原先每组4个位仅有16种变化，现在改成每组8个位有256种变化。

经过这样修改后, perm_init()函数会把每组8位按打乱表打乱后变成分散在8个字节中的8个位,最后把8组8字节进行或运算合并成完整的64位。

### 函数接口定义：

对应地，我们需要修改以下函数使它们符合上述要求：

```c
   static void perm_init(char perm[8][256][8], char p[64]);
   static void permute(char *inblock, char perm[8][256][8], char *outblock);
   int des_init(int mode);
```

**这是一个函数题，你需要且只能提交以上三个修改后的函数；最后给分将人工判定**

### 裁判测试程序样例：

```c++
static char (*iperm)[256][8];
static char (*fperm)[256][8];
static void perm_init(char perm[8][256][8], char p[64]);
static void permute(char *inblock, char perm[8][256][8], char *outblock);
int des_init(int mode);

//其他部分参见mydes.c

int main()
{
   char mykey[9], p[9];
   unsigned char c[9];
   int i;
   scanf("%s %s", mykey, p);
   des_init(0);
   des_set_key(mykey);
   strcpy((char *)c, p);
   des_encrypt((char *)c);
   des_done();
   for(i=0; i<8; i++)
   {
      printf("%02X ", c[i]);
   }
   des_init(0);
   des_set_key(mykey);
   des_decrypt((char *)c);
   des_done();
   puts((char *)c);
}
```

### 输入样例：

以下输入输出你无需关心，其逻辑由main函数实现：输入密钥key和明文p，输出加密得到的十六进制字符串以及解密得到明文

```in
ABCD1234 abcdefgh
```

### 输出样例：

```out
6B 4A 17 64 E9 F2 2D 4F abcdefgh
```

# 6-3 写一个函数des_cfb_encrypt()调用DES算法以CFB模式对明文进行加密 (20 分)

本题要求实现一个函数`des_cfb_encrypt()`，该函数原型如下:
`void des_cfb_encrypt(unsigned char p[], int n, unsigned char des_seed_iv[], /*=================*/ unsigned char des_iv[], unsigned char des_seed_key[]);`
其中p为明文，n为p的长度，des_iv为调用DES算法以CFB模式加密时需要的8字节初始向量。
你可以在本函数内调用以下函数实现DES加密:
`void des_encrypt(unsigned char *block, unsigned char *des_key);`
其中block在调用前指向明文，调用后指向密文，des_key指向8字节DES密钥。
同时你还可以在`des_cfb_encrypt()`内调用以下两个函数实现rc4加密或解密:
`void prepare_key(unsigned char *rc4_seed_key, int seed_key_len, rc4_key *k); void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_key *k);`
由于函数`des_cfb_encrypt()`的参数中并没有des_key，而仅有des_seed_key，故你需要 根据该函数已有的参数算出des_key。其中des_key和des_seed_key的关系如下:
`prepare_key(rc4_seed_key, strlen(rc4_seed_key), &k); strcpy(des_key, des_seed_key); rc4(des_key, strlen(des_key), &k);`
也就是说，des_key是用rc4算法加密des_seed_key得来的。`des_cfb_encrypt()`函数中的参数 des_iv和des_seed_iv的关系跟des_key和des_seed_key的关系一样，即des_iv也是用rc4算法加密 des_seed_iv得来的，并且加密des_seed_iv时使用的密钥k跟加密des_seed_key时使用的密钥相同。 为便于理解上述描述并设法解出des_key，请看函数main()的代码:

```c
main()
{
   /*
   input:
   ABCD1234 DEARSEED DEADBEEF Hello123
   output:
   A69AFFEA65AB11E3
   */
   unsigned char rc4_seed_key[0x40], des_seed_iv[0x40], des_seed_key[0x40];
   unsigned char des_iv[0x40],  des_key[0x40];
   rc4_key k;
   unsigned char p[0x100];

   scanf("%s %s %s %s", rc4_seed_key, des_seed_iv, des_seed_key, p);
   prepare_key(rc4_seed_key, strlen(rc4_seed_key), &k);
   strcpy(des_iv, des_seed_iv);
   rc4(des_iv, strlen(des_iv), &k);

   /* 注意以下3句话是注释掉的:
   prepare_key(rc4_seed_key, strlen(rc4_seed_key), &k);
   strcpy(des_key, des_seed_key);
   rc4(des_key, strlen(des_key), &k);
   */
   des_cfb_encrypt(p, strlen(p), des_seed_iv, des_iv, des_seed_key);
}
```


`main()`函数中输入的rc4_seed_key、des_seed_iv、des_seed_key长度均为8字节，
p的长度不超过0xFF。`des_cfb_encrypt()`里面要调用des_encrypt()并以十六进制格式输出密文。

### 输入样例：

```in
ABCD1234 DEARSEED DEADBEEF Hello123
```

### 输出样例：

```out
A69AFFEA65AB11E3
```

# 6-4 重写AES解密函数aes_decrypt(), 多项式乘法函数aes_polynomial_mul()等 (20 分)

下载myaes.zip(http://10.71.45.100/bhh/myaes.zip), 解压缩, 里面包含了vc6的工程 文件MyAes.dsw, 双击打开, 可以看到MyAes.c;
MyAes.c中部分函数的源代码已删除, 这些函数经编译后生成的机器码已转移到aes_lib.lib内。 这些已删除的函数包括:
`void build_sbox_inverse(void); void aes_polynomial_mul(unsigned char x[4], unsigned char y[4], unsigned char z[4]); void ByteSubInverse(unsigned char *p, int n); void ShiftRowInverse(unsigned char *p); void MixColumnInverse(unsigned char *p, unsigned char a[4], int do_mul); void aes_decrypt(unsigned char *bufin, unsigned char *bufout, unsigned char *key);`
关于这些函数的功能介绍，请查阅压缩包内的aes.txt文件。
请对本程序(MyAes.c)做以下修改:
(1) 重写上述函数
(2) 删除#pragma comment(lib, "aes_lib.lib")
(3) 在实现上述函数时可以调用myaes.c中已定义的任何函数，也可引用已定义的全局变量或数组

要求:
(1) 修改后的程序必须能正常编译运行
(2) 运行产生的输出结果与本程序一致

如果不想用VC6测试程序, 而是要在linux系统下编程, 请执行以下命令进行编译、运行:
`gcc myaes.c aes_lib.o -o myaes ./myaes`
在VC6或gcc下测试成功后，把你编写的这些函数复制到代码框内提交。

### 输入样例：

```c
A Quick BrownFox
```

### 输出样例：

```c
Encrypting by using 128-bit seed_key...
plaintext=A Quick BrownFox
seed_key=0123456789ABCDEF
ciphertext=C037F19D4AEA8E7172B176B9D4CE1D62
Decrypting...
plaintext=A Quick BrownFox
==========================================
Encrypting by using 192-bit seed_key...
plaintext=A Quick BrownFox
seed_key=0123456789ABCDEF012345678
ciphertext=F51C8DD5F7052264650126DC70D402F5
Decrypting...
plaintext=A Quick BrownFox
==========================================
Encrypting by using 256-bit seed_key...
plaintext=A Quick BrownFox
seed_key=0123456789ABCDEF0123456789ABCDEF
ciphertext=2687C0D61E803718F6D11A0384C81229
Decrypting...
plaintext=A Quick BrownFox
==========================================
```