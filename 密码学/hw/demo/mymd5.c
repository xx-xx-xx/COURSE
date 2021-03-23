/* MD5C.C - RSA Data Security, Inc., MD5 message-digest algorithm
 */

/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */

/* Modified & commented by Black White(iceman@zju.edu.cn) */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _MD5_CTX
{
   unsigned long  state[4]; /* 128位摘要 */
   unsigned long  count[2]; /* 已处理的报文的二进制位数,最大值=2^64-1 */
   unsigned char  data[64]; /* 64字节message块 */
} MD5_CTX;

int Init_MD5(MD5_CTX *MD5_ctx);
int Update_MD5(MD5_CTX *MD5_ctx, unsigned char *buffer, unsigned long count);
int Final_MD5(MD5_CTX *MD5_ctx);
static void Process_One_Block_MD5 (unsigned long state[4], unsigned char block[64]);
static unsigned char pad_stuff[64] = {
   0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

#define F(x, y, z) ( ((x) & (y)) | ((~x) & (z)) )
#define G(x, y, z) ( ((x) & (z)) | ((y) & (~z)) )
#define H(x, y, z) ( (x) ^ (y) ^ (z) )
#define I(x, y, z) ( (y) ^ ((x) | (~z)) )

#define ROL(x, n) ( ((x) << (n)) | ((x) >> (32-(n))) )

#define FF(a, b, c, d, x, shift, const_sin) { \
   (a) += F((b), (c), (d)) + (x) + \
   (unsigned long)(const_sin); \
   (a) = ROL((a), (shift)); \
   (a) += (b); \
   }

#define GG(a, b, c, d, x, shift, const_sin) { \
   (a) += G((b), (c), (d)) + (x) + (unsigned long)(const_sin); \
   (a) = ROL((a), (shift)); \
   (a) += (b); \
   }

#define HH(a, b, c, d, x, shift, const_sin) { \
   (a) += H((b), (c), (d)) + (x) + (unsigned long)(const_sin); \
   (a) = ROL((a), (shift)); \
   (a) += (b); \
   }

#define II(a, b, c, d, x, shift, const_sin) { \
   (a) += I((b), (c), (d)) + (x) + (unsigned long)(const_sin); \
   (a) = ROL((a), (shift)); \
   (a) += (b); \
   }

/*=============================================*/
int Init_MD5(MD5_CTX *MD5_ctx)
{
   MD5_ctx->count[0] = 0;
   MD5_ctx->count[1] = 0;
   MD5_ctx->state[0] = 0x67452301; // a0
   MD5_ctx->state[1] = 0xEFCDAB89; // b0
   MD5_ctx->state[2] = 0x98BADCFE; // c0
   MD5_ctx->state[3] = 0x10325476; // d0
   return 0;
}

/*=============================================*/
int Update_MD5(MD5_CTX *MD5_ctx, 
               unsigned char *buf, 
               unsigned long buf_len)
{
   unsigned long i, bytes_left, bytes_to_fill;
   bytes_left = (MD5_ctx->count[0] >> 3) & 0x3F; // 内部缓冲data中剩余未处理的字节数
                                                 // &0x3F 等价于 %64
   //bytes_left = (*(unsigned __int64 *)MD5_ctx->count >> 3) & 0x3F;
   *(unsigned __int64 *)MD5_ctx->count += (unsigned __int64)buf_len << 3; // 更新报文位数
   bytes_to_fill = 64 - bytes_left; // data中需要补充的字节数
   if (buf_len >= bytes_to_fill) 
   {
      memcpy(&MD5_ctx->data[bytes_left], buf, bytes_to_fill); // 现在data中已充满64字节
      Process_One_Block_MD5 (MD5_ctx->state, MD5_ctx->data); // 对data中的64字节进行计算
      bytes_left = 0; // data中剩余未处理字节数变0
      for (i=bytes_to_fill; i+63 < buf_len && i+63 >= 63; i+=64) // 从buf复制64字节到data
      {  // i=bytes_to_fill是为了跳过buf中已经补充给data的bytes_to_fill个字节的数据
         // i+63 < buf_len是为了保证buf中剩余数据至少还有64字节
         // i+63 >= 63是为了防止i+63发生溢出(注意i是unsigned long, 当它接近2^32-1时, 
         // i+63会溢出), 若i+63<63即i+63溢出, 则意味着buf中剩余字节不足64字节
         Process_One_Block_MD5(MD5_ctx->state, &buf[i]);
      }
   }
   else
      i = 0;
   memcpy(&MD5_ctx->data[bytes_left], &buf[i], buf_len-i);
   return 0;
}

/*=============================================*/
int Final_MD5(MD5_CTX *MD5_ctx)
{
   unsigned long bytes_left, pad_len;
   unsigned char total_bits[8];
   memcpy(total_bits, (unsigned char *)MD5_ctx->count, 8); // total_bits=
                                                           // 已处理的报文的二进制位数
                                                           // (含data中剩余的字节)
                                                           // 后面补充的pad_stuff及
                                                           // total_bits本身不计在内
   bytes_left = (MD5_ctx->count[0] >> 3) & 0x3F;
   pad_len = (bytes_left < 56) ? (56 - bytes_left) : 
               (64 - bytes_left + 56); // bytes_left==56时, 要补8+56=64字节
                                       // bytes_left==57时, 要补7+56=63字节
   Update_MD5(MD5_ctx, pad_stuff, pad_len); // 把pad_stuff加到data中计算
   Update_MD5(MD5_ctx, total_bits, 8); // 把total_bits加到data中计算
   return 0;
}

/*=============================================*/
static void Process_One_Block_MD5(unsigned long state[4], unsigned char block[64])
{
   unsigned long a = state[0], b = state[1], c = state[2], d = state[3];
   unsigned long *px = (unsigned long *)block;
   FF(a, b, c, d, px[0],  7,  0xD76AA478);   // step 1: a1
   FF(d, a, b, c, px[1],  12, 0xE8C7B756);   // step 2: d1
   FF(c, d, a, b, px[2],  17, 0x242070DB);   // step 3: c1
   FF(b, c, d, a, px[3],  22, 0xC1BDCEEE);   // step 4: b1
   FF(a, b, c, d, px[4],  7,  0xF57C0FAF);   // step 5: a2
   FF(d, a, b, c, px[5],  12, 0x4787C62A);   // step 6: d2
   FF(c, d, a, b, px[6],  17, 0xA8304613);   // step 7: c2
   FF(b, c, d, a, px[7],  22, 0xFD469501);   // step 8: b2
   FF(a, b, c, d, px[8],  7,  0x698098D8);   // step 9: a3
   FF(d, a, b, c, px[9],  12, 0x8B44F7AF);   // step 10:d3
   FF(c, d, a, b, px[10], 17, 0xFFFF5BB1);   // step 11:c3
   FF(b, c, d, a, px[11], 22, 0x895CD7BE);   // step 12:b3
   FF(a, b, c, d, px[12], 7,  0x6B901122);   // step 13:a4
   FF(d, a, b, c, px[13], 12, 0xFD987193);   // step 14:d4
   FF(c, d, a, b, px[14], 17, 0xA679438E);   // step 15:c4
   FF(b, c, d, a, px[15], 22, 0x49B40821);   // step 16:b4
                                        
   GG(a, b, c, d, px[1],  5,  0xF61E2562);   // step 17:a5
   GG(d, a, b, c, px[6],  9,  0xC040B340);   // step 18:d5
   GG(c, d, a, b, px[11], 14, 0x265E5A51);   // step 19:c5
   GG(b, c, d, a, px[0],  20, 0xE9B6C7AA);   // step 20:b5
   GG(a, b, c, d, px[5],  5,  0xD62F105D);   // step 21:a6
   GG(d, a, b, c, px[10], 9,  0x2441453);    // step 22:d6
   GG(c, d, a, b, px[15], 14, 0xD8A1E681);   // step 23:c6
   GG(b, c, d, a, px[4],  20, 0xE7D3FBC8);   // step 24:b6
   GG(a, b, c, d, px[9],  5,  0x21E1CDE6);   // step 25:a7
   GG(d, a, b, c, px[14], 9,  0xC33707D6);   // step 26:d7
   GG(c, d, a, b, px[3],  14, 0xF4D50D87);   // step 27:c7
   GG(b, c, d, a, px[8],  20, 0x455A14ED);   // step 28:b7
   GG(a, b, c, d, px[13], 5,  0xA9E3E905);   // step 29:a8
   GG(d, a, b, c, px[2],  9,  0xFCEFA3F8);   // step 30:d8
   GG(c, d, a, b, px[7],  14, 0x676F02D9);   // step 31:c8
   GG(b, c, d, a, px[12], 20, 0x8D2A4C8A);   // step 32:b8
                                        
   HH(a, b, c, d, px[5],  4,  0xFFFA3942);   // step 33:a9
   HH(d, a, b, c, px[8],  11, 0x8771F681);   // step 34:d9
   HH(c, d, a, b, px[11], 16, 0x6D9D6122);   // step 35:c9
   HH(b, c, d, a, px[14], 23, 0xFDE5380C);   // step 36:b9
   HH(a, b, c, d, px[1],  4,  0xA4BEEA44);   // step 37:a10
   HH(d, a, b, c, px[4],  11, 0x4BDECFA9);   // step 38:d10
   HH(c, d, a, b, px[7],  16, 0xF6BB4B60);   // step 39:c10
   HH(b, c, d, a, px[10], 23, 0xBEBFBC70);   // step 40:b10
   HH(a, b, c, d, px[13], 4,  0x289B7EC6);   // step 41:a11
   HH(d, a, b, c, px[0],  11, 0xEAA127FA);   // step 42:d11
   HH(c, d, a, b, px[3],  16, 0xD4EF3085);   // step 43:c11
   HH(b, c, d, a, px[6],  23, 0x04881D05);   // step 44:b11
   HH(a, b, c, d, px[9],  4,  0xD9D4D039);   // step 45:a12
   HH(d, a, b, c, px[12], 11, 0xE6DB99E5);   // step 46:d12
   HH(c, d, a, b, px[15], 16, 0x1FA27CF8);   // step 47:c12
   HH(b, c, d, a, px[2],  23, 0xC4AC5665);   // step 48:b12
                                        
   II(a, b, c, d, px[0],  6,  0xF4292244);   // step 49:a13
   II(d, a, b, c, px[7],  10, 0x432AFF97);   // step 50:d13
   II(c, d, a, b, px[14], 15, 0xAB9423A7);   // step 51:c13
   II(b, c, d, a, px[5],  21, 0xFC93A039);   // step 52:b13
   II(a, b, c, d, px[12], 6,  0x655B59C3);   // step 53:a14
   II(d, a, b, c, px[3],  10, 0x8F0CCC92);   // step 54:d14
   II(c, d, a, b, px[10], 15, 0xFFEFF47D);   // step 55:c14
   II(b, c, d, a, px[1],  21, 0x85845DD1);   // step 56:b14
   II(a, b, c, d, px[8],  6,  0x6FA87E4F);   // step 57:a15
   II(d, a, b, c, px[15], 10, 0xFE2CE6E0);   // step 58:d15
   II(c, d, a, b, px[6],  15, 0xA3014314);   // step 59:c15
   II(b, c, d, a, px[13], 21, 0x4E0811A1);   // step 60:b15
   II(a, b, c, d, px[4],  6,  0xF7537E82);   // step 61:a16
   II(d, a, b, c, px[11], 10, 0xBD3AF235);   // step 62:d16
   II(c, d, a, b, px[2],  15, 0x2AD7D2BB);   // step 63:c16
   II(b, c, d, a, px[9],  21, 0xEB86D391);   // step 64:b16

   state[0] += a;
   state[1] += b;
   state[2] += c;
   state[3] += d;
}
/*file_md5.cpp
file_md5.exe
file_md5 kvrt.exe*/

int main(int argc, char* argv[])
{
   MD5_CTX m;
   unsigned char s[100];
   int i;
   unsigned char *p;
   puts("Input a message:");
   gets(s);
   Init_MD5(&m);
   Update_MD5(&m, s, strlen(s));
   Final_MD5(&m);
   p = (unsigned char *)m.state;
   puts("The 128-bit md5 digest is:");
   for(i=0; i<16; i++)
      printf("%02X ", p[i]);
   getchar();
   return 0;
}

