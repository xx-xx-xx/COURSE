/* initialize a perm array */
static void perm_init(char perm[8][256][8], char p[64]) /* 64-bit, either init or final */
{
    register int b, j, k;
    int i, m;

    /* Clear the permutation array */
    memset(perm, 0, 8 * 256 * 8);
    for (i = 0; i < 8; i++) /* [%] i=SectionNum, each section contains 8 bits; */ /* each input nibble position */
    {
        for (j = 0; j < 256; j++) /* [%] j=SectionValue varying from 0 to 255; */ /* each possible input nibble */
        {
            for (k = 0; k < 64; k++) /* [%] k=TargetBit; */       /* each output bit position */
            {                                                     /* [%] check TargetBit k is concerned with SectionNum i and SectionValue j */
                b = p[k] - 1; /* [%] b=SourceBit; */              /* where does this bit come from */
                if ((b >> 3) != i)                                /* [%] b/4 != i means this bit does not lie in this section. */
                    continue; /* does it come from input posn? */ /* if not, bit k is 0 */
                if (!(j & bytebit[b & 7]))
                    continue; /* [%] This bit is not equal to 1; */
                /* [%] SourceBit b will result in setting TargetBit k to 1 */
                m = k % 8; /* [%] m = k % 8; */
                perm[i][j][k >> 3] |= bytebit[m];
            }
        }
    }
}
/* Permute inblock with perm */
static void permute(char *inblock, char perm[8][256][8], char *outblock)
{ /* result into outblock, 64 bits */
    register int i, j;
    register char *pin, *pout; /* ptr to input or output block */
    register char *p;

    if (perm == NULL)
    {
        /* No permutation, just copy */
        memcpy(outblock, inblock, 8);
        return;
    }

    /* Clear output block    */
    memset(outblock, 0, 8);

    pin = inblock;
    for (j = 0; j < 8; j++, pin++) /* [%] total 8 loops to permute 64 bits from inblock */
    {                              /* for each input nibble */
        pout = outblock;
        p = perm[j][*pin & 0xFF];
        for (i = 0; i < 8; i++) /* and each output byte */
        {
            *pout++ += *p++; /* OR the masks together*/
        }
    }
}

int des_init(int mode)
{
    if (sbox_output_perm_table != NULL)
    {
        /* Already initialized */
        return 1;
    }
    des_mode = mode;

    if ((sbox_output_perm_table = (long32(*)[64])malloc(sizeof(long32) * 8 * 64)) == NULL)
    {
        return 0;
    }
    sbox_output_perm_table_init();
    kn = (unsigned char(*)[8])malloc(sizeof(char) * 16 * 8);
    if (kn == NULL)
    {
        free((char *)sbox_output_perm_table);
        return 0;
    }
    if (mode == 1 || mode == 2) /* No permutations */
        return 1;

    iperm = (char(*)[256][8])malloc(sizeof(char) * 8 * 256 * 8);
    if (iperm == NULL)
    {
        free((char *)sbox_output_perm_table);
        free((char *)kn);
        return 0;
    }
    perm_init(iperm, ip);

    fperm = (char(*)[256][8])malloc(sizeof(char) * 8 * 256 * 8);
    if (fperm == NULL)
    {
        free((char *)sbox_output_perm_table);
        free((char *)kn);
        free((char *)iperm);
        return 0;
    }
    perm_init(fperm, fp);
    return 1;
}
