/*
  Author:  Pate Williams (c) 1997

  Secure hash algorithm updated version.
  See "Handbook of Applied Cryptography"
  by Alfred J. Menezes et al editors,
  9.53 Algorithm page 348.
*/

/* 
   The following functions are rewritten by Black White(iceman@zju.edu.cn):
      void SHA1_Init(SHA1_CTX *psha);
      void SHA1_Update(SHA1_CTX *psha, uchar *buf, ulong buf_len);
      void SHA1_Final(SHA1_CTX *psha);
   And the comments marked by [%] are added for more readability.
 */   

#include <stdio.h>
#include <string.h>
#define LITTLE_ENDIAN 1 /* [%] Intel的CPU均遵循此数据存放规则 */

typedef unsigned char uchar;
typedef unsigned long ulong;
typedef struct _SHA1_CTX 
{
   ulong state[5]; /* [%] 4*5=20字节即160位hash值 */
   struct {ulong hi, lo;} length; /* [%] length相当于一个64位整数, 存放报文的二进制位数 */
                                  /* 注意length的值复制到data中时必须以Big-Endian规则存放:
                                     高32位在前,低32位在后,并且32位中的高8位在前低8位在后.
                                  */                                    
   uchar data[64]; /* [%] 64字节message块 */
} SHA1_CTX;
void SHA1_Init(SHA1_CTX *psha);
void SHA1_Update(SHA1_CTX *psha, uchar *buf, ulong buf_len);
void SHA1_Final(uchar *digest, SHA1_CTX *psha);

static void  Process_One_Block_SHA1(ulong state[5], uchar block[64]);
static void  Round(int j_min, ulong y, ulong *X, 
                   ulong block_state[5],
                   ulong (*z)(ulong, ulong, ulong));
static ulong f(ulong u, ulong v, ulong w);
static ulong g(ulong u, ulong v, ulong w);
static ulong h(ulong u, ulong v, ulong w);
static ulong ROL(ulong x, int number);
#ifdef LITTLE_ENDIAN
static void  BigEndian(uchar *buf, int buf_len);
#endif

static ulong f(ulong u, ulong v, ulong w)
{
   return (u & v) | (~u & w);
}

static ulong g(ulong u, ulong v, ulong w)
{
   return (u & v) | (u & w) | (v & w);
}

static ulong h(ulong u, ulong v, ulong w)
{
   return u ^ v ^ w;
}

#ifdef LITTLE_ENDIAN
static void BigEndian(uchar *buf, int buf_len)
{  /* [%]把buf中包含的buf_len/4个long全部转化成Big-Endian格式存放 */
   /* buf_len是buf中存放的字节数, buf_len/4是buf中存放的long型整数个数 */
   int i;
   uchar *cp = (uchar *)buf, t;
   for (i = 0; i < buf_len/4; i++) 
   {
      t = cp[0];
      cp[0] = cp[3];
      cp[3] = t;
      t = cp[1];
      cp[1] = cp[2];
      cp[2] = t;
      cp += 4;
   }
}
#endif

static ulong ROL(ulong x, int number) /* left circular shift number bits */
{
   return (x << number) | (x >> (32 - number));
}

static void Round(int j_min, ulong y, ulong *X,
            ulong block_state[5],
            ulong (*z)(ulong, ulong, ulong))
{
   int j;
   ulong t;
   for (j = j_min; j < j_min + 20; j++) 
   {
      t = ROL(block_state[0], 5) + 
          (*z)(block_state[1], block_state[2], block_state[3]) + 
          block_state[4] + X[j] + y;
      block_state[4] = block_state[3];
      block_state[3] = block_state[2];
      block_state[2] = ROL(block_state[1], 30);
      block_state[1] = block_state[0];
      block_state[0] = t;
   }
}

void SHA1_Init(SHA1_CTX *psha)
{
   psha->state[0] = 0x67452301UL;
   psha->state[1] = 0xEFCDAB89UL;
   psha->state[2] = 0x98BADCFEUL;
   psha->state[3] = 0x10325476UL;
   psha->state[4] = 0xC3D2E1F0UL;
   psha->length.hi = 0;
   psha->length.lo = 0;
}

void SHA1_Update(SHA1_CTX *psha, uchar *buf, ulong buf_len)
{
   ulong i, bytes_left, bytes_to_fill;
   bytes_left = (psha->length.lo >> 3) & 0x3F; /*[%] 内部缓冲data中剩余未处理的字节数
                                                     &0x3F 等价于 %64
                                                */
   psha->length.lo += buf_len << 3;
   if(psha->length.lo < (buf_len << 3)) /*[%] 若上一行语句做加法运算时产生进位, */
      psha->length.hi++;                /*[%] 则进位加到length.hi中. */
   psha->length.hi += buf_len >> 29;    /*[%] 加上buf_len << 3后移出左端的3位 */
   bytes_to_fill = 64 - bytes_left;     /* data中需要补充的字节数 */
   if (buf_len >= bytes_to_fill) 
   {
      memcpy(&psha->data[bytes_left], buf, bytes_to_fill); /* 现在data中已充满64字节 */
      Process_One_Block_SHA1(psha->state, psha->data); /* 对data中的64字节进行计算 */
      bytes_left = 0; /* data中剩余未处理字节数变0 */
      for (i=bytes_to_fill; i+63 < buf_len && i+63 >= 63; i+=64) /* 从buf复制64字节到data */
      {  /* i=bytes_to_fill是为了跳过buf中已经补充给data的bytes_to_fill个字节的数据
            i+63 < buf_len是为了保证buf中剩余数据至少还有64字节
            i+63 >= 63是为了防止i+63发生溢出(注意i是unsigned long, 当它接近2^32-1时, 
            i+63会溢出), 若i+63<63即i+63溢出, 则意味着buf中剩余字节不足64字节
          */
         Process_One_Block_SHA1(psha->state, &buf[i]);
      }
   }
   else
      i = 0;
   memcpy(&psha->data[bytes_left], &buf[i], buf_len-i);
}

static void Process_One_Block_SHA1(ulong state[5], uchar block[64])
{
   static ulong y1 = 0x5A827999UL, y2 = 0x6ED9EBA1UL,
                y3 = 0x8F1BBCDCUL, y4 = 0xCA62C1D6UL;
   ulong block_state[5];
   ulong X[80];
   int i;
   #ifdef LITTLE_ENDIAN
   BigEndian(block, 64); /*[%] block中的每个ulong必须为BigEndian格式 */
   #endif
   memcpy(X, block, 64); /*[%] 复制block中的16个ulong到X中 */
   memcpy(block_state, state, sizeof(ulong)*5);
   for(i = 16; i < 80; i++) /* 把X中的16个ulong扩充成80个ulong */
   {
      X[i] = ROL(X[i - 3] ^ X[i - 8] 
           ^ X[i - 14] ^ X[i - 16], 1);
   }
   Round( 0, y1, X, block_state, f);
   Round(20, y2, X, block_state, h);
   Round(40, y3, X, block_state, g);
   Round(60, y4, X, block_state, h);
   for(i=0; i<5; i++)
      state[i] += block_state[i];
}

void SHA1_Final(uchar *digest, SHA1_CTX *psha)
{
   ulong bytes_left, pad_len;
   uchar total_bits[8];
   static unsigned char pad_stuff[64] = 
   {
   0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
   };
   memcpy(total_bits, (uchar *)&psha->length, 8); /* total_bits=
                                                    已处理的报文的二进制位数
                                                    (含data中剩余的字节)
                                                    后面补充的pad_stuff及
                                                    total_bits本身不计在内
                                                  */
   bytes_left = (psha->length.lo >> 3) & 0x3F;
   pad_len = (bytes_left < 56) ? (56 - bytes_left) : 
             (64 - bytes_left + 56); /* bytes_left==56时, 要补8+56=64字节
                                        bytes_left==57时, 要补7+56=63字节
                                      */
   SHA1_Update(psha, pad_stuff, pad_len); /* 把pad_stuff加到data中计算 */
   #ifdef LITTLE_ENDIAN
   BigEndian(total_bits, 8);  /* total_bits必须转成Big-Endian格式,
                                 即32位的ulong必须高字节在前, 低字节在后
                               */
   #endif
   SHA1_Update(psha, total_bits, 8); /* 把total_bits加到data中计算 */
   memcpy(digest, psha->state, 20);  /* 把5个state复制到digest */
   #ifdef LITTLE_ENDIAN
   BigEndian(digest, 20); 
   /*[%] 注意SHA1的hash值共5个state, 每个state为ulong类型, 
         输出的时候,不可以把5个state当作连续的20个字节并以
         字节为单位按从左到右顺序输出, 而应该以ulong为单位
         分5次输出. 这是因为在Little-Endian的机器中, ulong
         是按低字节在前高字节在后的顺序存放在内存中的 , 若
         以字节为单位输出ulong , 则从左到右输出的4个字节与
         直接输出ulong所得的4字节顺序刚好是反的.
         这里为了达到以字节为单位按从左到右顺序输出的目的,
         特地调用BigEndian()这个函数对每个ulong所包含的4个
         字节颠倒顺序, 这样一来,  digest中包含的20字节摘要
         就可以按从左到右顺序输出了.
   */
   #endif
}

void do_buffer(uchar *buffer)
{
   SHA1_CTX sha;
   int i;
   uchar digest[20];
   SHA1_Init(&sha);
   SHA1_Update(&sha, buffer, strlen((char *)buffer));
   SHA1_Final(digest, &sha);
   printf("%s\n", buffer);
   for(i = 0; i < 20; i++)
      printf("%02X", digest[i]);
   printf("\n");
}

int main(void)
{
   uchar buffer1[32] = "";
   uchar buffer2[32] = "a";
   uchar buffer3[32] = "abc";
   uchar buffer4[32] = "abcdefghijklmnopqrstuvwxyz";

   do_buffer(buffer1);
   do_buffer(buffer2);
   do_buffer(buffer3);
   do_buffer(buffer4);
   return 0;
}