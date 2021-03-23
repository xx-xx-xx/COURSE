/*
From Mercury!comsec.com!owner-cp-lite Mon Sep 12 18:43:59 1994
Return-Path: <Mercury!comsec.com!owner-cp-lite>
Received: by chinet (/\==/\ Smail3.1.28.1 #28.1{chinet})
      id <m0qkL2U-0002GEa@chinet>; Mon, 12 Sep 94 18:43 CDT
Received: by mercury.mcs.com (/\==/\ Smail3.1.28.1 #28.1)
      id <m0qkKw5-000BaRa@mercury.mcs.com>; Mon, 12 Sep 94 18:37
CDT
Received: by nntp.crl.com with UUCP id AA29979
  (5.65c/IDA-1.5); Mon, 12 Sep 1994 15:59:24 -0700
Received: from localhost (eb@localhost) by comsec.com
(8.6.5/8.6.5) id PAA06980 for cp-lite-real; Mon, 12 Sep 1994
15:39:09 -0700
Received: from localhost (eb@localhost) by comsec.com
(8.6.5/8.6.5) id PAA06976; Mon, 12 Sep 1994 15:39:09 -0700
Message-Id: <199409122239.PAA06976@comsec.com>
To: Cypherpunks Lite <cp-lite@comsec.com>
Sender: owner-cp-lite@comsec.com
Date: Fri, 9 Sep 1994 22:11:49 -0500
From: nobody@jpunix.com
Subject: Thank you Bob Anderson
Remailed-By: remailer@jpunix.com
Complaints-To: postmaster@jpunix.com
Precedence: bulk
Status: RO

SUBJECT:  RC4 Source Code

I've tested this.  It is compatible with the RC4 object module
that comes in the various RSA toolkits.
*/

/* The comments marked by [%] are added by Black White(iceman@zju.edu.cn) */

#include <stdio.h>
#include <string.h>

typedef struct rc4_key
{
     unsigned char state[256];
     unsigned char x;
     unsigned char y;
} rc4_key;

void prepare_key(unsigned char *key_data_ptr, int key_data_len, rc4_key *key);
void rc4(unsigned char *buffer_ptr, int buffer_len, rc4_key *key);
static void swap_byte(unsigned char *a, unsigned char *b);

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

static void swap_byte(unsigned char *a, unsigned char *b)
{
   unsigned char swapByte;
   swapByte = *a;
   *a = *b;
   *b = swapByte;
}

main()
{
   rc4_key k;
   char seed_key[] = "This is RC4 seed key.";
   char plaintext[100] = "A quick brown fox jumps over the lazy dog.";
   unsigned char buf[100];
   int i, n;
   prepare_key(seed_key, strlen(seed_key), &k);
   printf("plaintext=%s\n", plaintext);
   strcpy(buf, plaintext);
   n = strlen(buf);
   rc4(buf, n, &k);   /*[%] 加密 */
   printf("ciphertext=");
   for(i=0; i<n; i++) /*[%] 因密文中可能包含不可显示的字符, 故用十六进制输出 */
      printf("%02X", buf[i]);
   putchar('\n');

   prepare_key(seed_key, strlen(seed_key), &k);
   rc4(buf, n, &k);   /*[%] 解密 */
   buf[n] = '\0';
   printf("plaintext=%s\n", buf);
}