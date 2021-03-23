//达成被查重的成就
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
/*
static long32 f(ulong32 r, unsigned char subkey[8]) {
        unsigned char s[4], plaintext[8], m[4], t[4];
        unsigned long int rval;
        (1) 复制r到s, 即把r当成4个字节看待(注意大小端)。
        (2) 根据plaintext_32bit_expanded_to_48bit_table这张表, 把s中的4字节共32位扩展成48位并保存到数组plaintext中; //plaintext每个元素的左边2位恒为0, 右边6位用来保存数据
        32位转48位的过程要求使用双重循环来做,外循环8次,内循环6次,其中内循环每次只提取1位;
        注意提取某1位的时候只能从数组s中取(要计算该位是第几个字节中的第几位), 不得从参数r中获取; 判断某位是否为1的时候可以使用表bytebit。
        (3) 把plaintext中的8个字节与subkey中8个字节异或, 异或后的值保存到plaintext内;该过程使用8次循环来做。
        (4) 取出plaintext[i]查表sbox[i]得4位数, 循环8次可得8个4位数, 按从左到右顺序合并得32位数保存到数组m中。
        (5) 根据sbox_perm_table把m中的32位数打乱保存到数组t中, 该过程需要使用32次循环来做,每次循环提取1位。
        (6) 复制t到rval(注意大小端), 并返回rval。
    }
*/
static long32 f(ulong32 r, unsigned char subkey[8])
{
	int i, j;
    unsigned char s[4], plaintext[8], m[4], t[4];
    unsigned long int rval = 0;
    memset(plaintext, 0, 8);
    //1
    for (i = 0; i < 4; i++) {
		s[i] = r % 256;
		r /= 256;
	}
    //2
    for (i = 0; i < 8; i++) {
		plaintext[7 - i] = 0;
		for (j = 0; j < 6; j++) {
			plaintext[7 - i] += ((s[(32 - plaintext_32bit_expanded_to_48bit_table[i * 6 + j]) / 8] &
				bytebit[7 - (32 - plaintext_32bit_expanded_to_48bit_table[i * 6 + j]) % 8]) == 0) ?
				0 : bytebit[2 + j];
		}
	}
    //3
    for (i = 0; i < 8; i++) {
		plaintext[i] ^= subkey[7 - i];
	}

    //4
    for (i = 0; i < 4; i++) m[i] = 0;
	for (i = 0; i < 4; i++) {
		m[i] |= sbox[6 - i * 2][(plaintext[i * 2 + 1] & bytebit[2]) |
			((plaintext[i * 2 + 1] & bytebit[7]) ? bytebit[3] : 0) |
			((plaintext[i * 2 + 1] >> 1) & 0x0F)];
		m[i] <<= 4;
		m[i] |= sbox[7 - i * 2][(plaintext[i * 2] & bytebit[2]) | ((plaintext[i * 2] & bytebit[7]) ? bytebit[3] : 0) |
			((plaintext[i * 2] >> 1) & 0x0F)];
	}

    //5
    for (i = 0; i < 4; i++) {
		t[i] = 0;
	}
	for (i = 0; i < 32; i++) {
		t[i / 8] |= (m[(32 - sbox_perm_table[31 - i]) / 8] & bytebit[7 - (32 - sbox_perm_table[31 - i]) % 8]) ?
			bytebit[7 - i % 8] : 0;
	}

    //6
    for (i = 0; i < 4; i++) {
		rval <<= 8;
		rval |= t[3 - i];
	}

	return rval;
}
    register ulong32 res, P;
	unsigned char ch;
	unsigned char row;
	unsigned char col;
 
    unsigned char plaintext[8];
	int sub, cou;
	res = 0;
	P = 0;

	memset(plaintext, 0, 8);
    //2！！！！！！！
	for (sub = 0; sub <= 7; sub++)//根据plaintext_32bit_expanded_to_48bit_table这张表, 把r扩展成48位并保存到以下数组内:unsigned char plaintext[8]
	{
		for (cou = 0; cou <= 5; cou++)
		{
			ch = (r >> (32 - plaintext_32bit_expanded_to_48bit_table[6 * sub + cou])) & 0x01;
			plaintext[sub] |= ch << (5 - cou);
		}
	}
    //3333333！！！！！！
	for (cou = 0; cou <= 7; cou++)//把plaintext中的各个元素与subkey中各个元素异或, 异或后的值保存到plaintext内
	{
		plaintext[cou] ^= *(subkey + cou);
	}
    //4444444444

	for (cou = 0; cou <= 7; cou++)//取出plaintext[i]查表sbox[i]得4位数, 循环8次可得8个4位数, 按从左到右顺序合并得32位数,其中查sbox的过程必须先把plaintext[i]拆成2位行号4位列号再去查表得4位结果。
	{
		row = 2 * ((plaintext[cou] >> 5) & 0x01) + (plaintext[cou] & 0x01);
		col = (plaintext[cou]>>1)&0x0F;
		ch = sbox[cou][row * 16 + col];
		P |= ch << (4 * (7 - cou));
	}
    //555555555555
	for (cou = 0; cou <= 31; cou++)//根据sbox_perm_table把步骤(3)所得32位数打乱得返回值rval, 该过程需要使用32次循环来做,每次循环提取1位。
	{
		res |= (P >> (32-sbox_perm_table[cou])) & 0x00000001;
		if (cou<=30)
		   res=res << 1;
	}

	return res;




	s[0] = r & 0xFF;
	s[1] = (r & 0xFF00)>>8;
	s[2] = (r & 0xFF0000) >> 16;
	s[3] = (r & 0xFF000000) >> 24;
	#ifdef LITTLE_ENDIAN
    s[0] = byteswap(s[0]);
    s[1] = byteswap(s[1]);
	s[2] = byteswap(s[2]);
    s[3] = byteswap(s[3]);
	#endif


    int i = 0;j = 0,temp = 0,ans=0;
	for(i=0;i<8;i++){
		ans = 0;temp = 0;
		for(j=0;j<6;j++){
			temp = plaintext_32bit_expanded_to_48bit_table[6*i+j] - 1;
			ans |= ( s[temp/8] & bytebit[7-temp%8] ) >> 2;
		}
		plaintext[i] = ans;
	}