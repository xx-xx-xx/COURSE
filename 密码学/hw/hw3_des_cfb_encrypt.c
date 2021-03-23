/* input - ABCD1234 DEARSEED DEADBEEF Hello123
   output- A69AFFEA65AB11E3 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct rc4_key
{
    unsigned char state[256];
    unsigned char x;
    unsigned char y;
} rc4_key;
unsigned char rc4_seed_key[0x40], des_seed_iv[0x40], des_seed_key[0x40]; //长度均为8字节
unsigned char des_iv[0x40], des_key[0x40];
rc4_key k;
unsigned char p[0x100]; //长度不超过0xFF

void prepare_key(unsigned char *rc4_seed_key, int seed_key_len, rc4_key *k);
void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_key *k);

/* block - 调用前指向明文，调用后指向密文
   des_key - 指向8字节DES密钥 */
void des_encrypt(unsigned char *block, unsigned char *des_key)
{
    /* rc4加密: 密钥k相同
      des_seed_key -> des_key
      des_seed_iv -> des_iv */
    prepare_key(rc4_seed_key, strlen(des_seed_key), des_seed_key);
    rc4(p, strlen(p), des_key); //???
}

/* func - 调用DES算法以CFB模式对明文进行加密
   p - 明文
   n - p的长度
   des_seed_iv - des_iv的明文
   des_iv - 调用DES算法以CFB模式加密时需要的8字节初始向量 */
void des_cfb_encrypt(unsigned char p[], int n, unsigned char des_seed_iv[], unsigned char des_iv[], unsigned char des_seed_key[])
{
    int i, j;
    int bits = 8, bytes = 1, cout = 0; //1 byte = 8 bits
    unsigned char des_key[0x40];
    unsigned char seed_iv[8], temp[0x10]; //temp plaintext
    unsigned char *c1, *c2;               //temp and final ciphertext

    c1 = (unsigned char *)malloc(sizeof(unsigned char) * 0x100); //p的长度不超过0xFF
    //注意不能重复分配地址！！！
    c2 = c1;

    //解密 des_iv 得到 key
    //加密 des_seed_key --key--> des_key
    for (i = 0; i < 64; i++)
        des_key[i] = des_seed_iv[i] ^ des_iv[i] ^ des_seed_key[i];
    //此处判定好像无影响？
    if (p == NULL || des_iv == NULL)
        return 0;
    memcpy(seed_iv, des_iv, 8);

    while (n >= bytes)
    {
        n -= bytes;
        cout += bytes;

        memcpy(temp, seed_iv, 8);
        des_encrypt(temp, des_key);
        memset(temp + 8, 0, 8);
        memcpy(temp + 8, p, bytes);

        p += bytes;

        for (j = 0; j < 8; j++)
            temp[j + 8] ^= temp[j];

        memcpy(c1, temp + 8, bytes);
        c1 += bytes;
        memcpy(temp, seed_iv, 8);

        if (bits % 8 == 0)
            memcpy(temp, temp + bits / 8, 8);
        else
            for (i = 0; i < 8; i++)
                temp[i] = temp[i + bytes] | temp[i + bytes + 1] >> 8; //temp[i] = temp[i + bits / 8] << bits % 8 | temp[i + bits / 8 + 1] >> (8 - bits % 8);

        memcpy(seed_iv, temp, 8);
        memcpy(des_iv, temp, 8);
    }

    //以十六进制格式输出密文
    for (i = 0; i < cout; i++)
        printf("%02X", c2[i]);
}

int main()
{
    scanf("%s %s %s %s", rc4_seed_key, des_seed_iv, des_seed_key, p);
    prepare_key(rc4_seed_key, strlen(rc4_seed_key), &k);
    strcpy(des_iv, des_seed_iv);
    rc4(des_iv, strlen(des_iv), &k);

    /* 注意以下3句话是注释掉的:
   prepare_key(rc4_seed_key, strlen(rc4_seed_key), &k);
   strcpy(des_key, des_seed_key);
   rc4(des_key, strlen(des_key), &k); */

    des_cfb_encrypt(p, strlen(p), des_seed_iv, des_iv, des_seed_key);
}
void prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key)
{
   unsigned char swapByte;
   unsigned char index1;
   unsigned char index2;
   unsigned char *state;
   short counter;

   state = &key->state[0];
   for(counter = 0; counter < 256; counter++)
      state[counter] = counter; /* 初始化256个密钥 */
   key->x = 0;
   key->y = 0;
   index1 = 0;
   index2 = 0;
   for(counter = 0; counter < 256; counter++)
   {   /*[%] key_data_ptr为种子密钥, 里面可以包含[1,256]个字节, 超过256字节的部分无用 */
       /*[%] 以下循环利用key_data_ptr打乱state, 注意256个state只交换, 不赋值 */
       index2 = (key_data_ptr[index1] + state[counter] + index2) % 256;
       swap_byte(&state[counter], &state[index2]);
       index1 = (index1 + 1) % key_data_len;
   }
}

void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_key *key)
{
   unsigned char x;
   unsigned char y;
   unsigned char *state;
   unsigned char xorIndex;
   short counter;

   x = key->x;
   y = key->y;

   state = &key->state[0];
   for(counter = 0; counter < buffer_len; counter ++)
   {
      x = (x + 1) % 256;
      y = (state[x] + y) % 256;
      swap_byte(&state[x], &state[y]); /*[%] 加密/解密前, 交换一对state */

      /* xorIndex = state[x] + (state[y]) % 256; */
      /*[%] 上面这条是原作者写的语句, 但可能不是他的本意, 故改成以下语句: */
      xorIndex = (state[x] + state[y]) % 256; /*[%] 计算密钥的index */
      buffer_ptr[counter] ^= state[xorIndex]; /*[%] state[xorIndex]为当前密钥 */
   }
   key->x = x; /* 保存x及y这两个state的下标, 使得下次rc4()调用与本次调用可以衔接 */
   key->y = y;
}
