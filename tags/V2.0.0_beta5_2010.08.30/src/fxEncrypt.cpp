#include <string.h>
#include <stdlib.h>
#include "fxEncrypt.h"
#include "fxdb.h"


enum
{
    ENCRYPT = 0,
    DECRYPT
};

/* Type:ENCRYPT, DECRYPT
 * the length of the output buffer isn't smaller
 * than ((datalen+7)/8)*8
 * input buffer and output buffer may be the same.
 * encrypt plain text by 3DES when keylen > 8
 */
bool DES(char *Out, char *In, long datalen, const char *Key, int keylen,
         bool Type);

/* initial permutation IP */
const static char IP_Table[64] =
{
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4, 62, 54, 46,
        38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8, 57, 49, 41, 33, 25,
        17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3, 61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
};

/* final permutation IP^-1 */
const static char IPR_Table[64] =
{
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31, 38, 6, 46, 14,
        54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29, 36, 4, 44, 12, 52, 20,
        60, 28, 35, 3, 43, 11, 51, 19, 59, 27, 34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
};

/* expansion operation matrix */
static const char E_Table[48] =
{
    32, 1, 2, 3, 4, 5, 4, 5, 6, 7, 8, 9, 8, 9, 10, 11, 12, 13, 12, 13, 14, 15,
        16, 17, 16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25, 24, 25, 26, 27,
        28, 29, 28, 29, 30, 31, 32, 1
};

/* 32-bit permutation function P used on the output of the S-boxes */
const static char P_Table[32] =
{
    16, 7, 20, 21, 29, 12, 28, 17, 1, 15, 23, 26, 5, 18, 31, 10, 2, 8, 24, 14,
        32, 27, 3, 9, 19, 13, 30, 6, 22, 11, 4, 25
};

/* permuted choice table (key) */
const static char PC1_Table[56] =
{
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18, 10, 2, 59, 51, 43, 35,
        27, 19, 11, 3, 60, 52, 44, 36, 63, 55, 47, 39, 31, 23, 15, 7, 62, 54,
        46, 38, 30, 22, 14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

/* permuted choice key (table) */
const static char PC2_Table[48] =
{
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4, 26, 8, 16, 7, 27,
        20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48, 44, 49, 39,
        56, 34, 53, 46, 42, 50, 36, 29, 32
};

/* number left rotations of pc1 */
const static char LOOP_Table[16] =
{
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

/* The (in)famous S-boxes */
const static char S_Box[8][4][16] =
{
    
    {// S1
	{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
	{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
	{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
	{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
    
    {// S2
	{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
	{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5}, 
	{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15}, 
	{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
	},
    
    {// S3
	{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8}, 
	{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1}, 
	{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7}, 
	{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
	},
    
    {// S4
	{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15}, 
	{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9}, 
	{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4}, 
	{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
	},
    
    {// S5
	{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9}, 
	{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6}, 
	{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14}, 
	{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
	},
    
    {// S6
	{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11}, 
	{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8}, 
	{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6}, 
	{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
	},
    
    {// S7
	{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1}, 
	{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6}, 
	{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2}, 
	{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
	},
    
    { // S8
	{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7}, 
	{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2}, 
	{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8}, 
	{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
	}
};


typedef bool(*PSubKey)[16][48];

static void Des(char Out[8], char In[8], const PSubKey pSubKey, bool Type); /*
                standard DES */
static void SetKey(const char *Key, int len);
static void SetSubKey(PSubKey pSubKey, const char Key[8]);
/* F function */
static void F_func(bool In[32], const bool Ki[48]);
/* S box function */
static void S_func(bool Out[32], const bool In[48]);
static void Transform(bool *Out, bool *In, const char *Table, int len);
static void Xor(bool *InA, const bool *InB, int len);
static void RotateL(bool *In, int len, int loop);
/* compose byte to bits */
static void ByteToBit(bool *Out, const char *In, int bits);
static void BitToByte(char *Out, const bool *In, int bits);
static bool SubKey[2][16][48];
/* triple DES */
static bool Is3DES;
static char Tmp[256], deskey[16];

bool DES(char *Out, char *In, long datalen, const char *Key, int keylen,
         bool Type)
{
    if (!(Out && In && Key && (datalen = (datalen + 7) &0xfffffff8)))
    {
        return  false;
    }
    SetKey(Key, keylen);
    if (!Is3DES)
    {
        /* DES */
        for (long i = 0, j = datalen >> 3; i < j; ++i, Out += 8, In += 8)
        {
            Des(Out, In, &SubKey[0], Type);
        }
    }
    else
    {
        /* triple DES: EDE-DED */
        for (long i = 0, j = datalen >> 3; i < j; ++i, Out += 8, In += 8)
        {
            Des(Out, In, &SubKey[0], Type);
            Des(Out, Out, &SubKey[1], !Type);
            Des(Out, Out, &SubKey[0], Type);
        }
    }
    return 0;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void SetKey(const char *Key, int len)
{
    memset(deskey, 0, 16);
    memcpy(deskey, Key, len > 16 ? 16 : len);
    SetSubKey(&SubKey[0], &deskey[0]);
    Is3DES = len > 8 ? (SetSubKey(&SubKey[1], &deskey[8]), 0):  - 1;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void Des(char Out[8], char In[8], const PSubKey pSubKey, bool Type)
{
    static bool M[64], tmp[32],  *Li = &M[0],  *Ri = &M[32];
    ByteToBit(M, In, 64);
    Transform(M, M, IP_Table, 64);
    if (Type == ENCRYPT)
    {
        for (int i = 0; i < 16; ++i)
        {
            memcpy(tmp, Ri, 32);
            F_func(Ri, (*pSubKey)[i]);
            Xor(Ri, Li, 32);
            memcpy(Li, tmp, 32);
        }
    }
    else
    {
        for (int i = 15; i >= 0; --i)
        {
            memcpy(tmp, Li, 32);
            F_func(Li, (*pSubKey)[i]);
            Xor(Li, Ri, 32);
            memcpy(Ri, tmp, 32);
        }
    }
    Transform(M, M, IPR_Table, 64);
    BitToByte(Out, M, 64);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void SetSubKey(PSubKey pSubKey, const char Key[8])
{
    static bool K[64],  *KL = &K[0],  *KR = &K[28];
    ByteToBit(K, Key, 64);
    Transform(K, K, PC1_Table, 56);
    for (int i = 0; i < 16; ++i)
    {
        RotateL(KL, 28, LOOP_Table[i]);
        RotateL(KR, 28, LOOP_Table[i]);
        Transform((*pSubKey)[i], K, PC2_Table, 48);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void F_func(bool In[32], const bool Ki[48])
{
    static bool MR[48];
    Transform(MR, In, E_Table, 48);
    Xor(MR, Ki, 48);
    S_func(In, MR);
    Transform(In, In, P_Table, 32);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void S_func(bool Out[32], const bool In[48])
{
    for (quint32 i = 0, j, k; i < 8; ++i, In += 6, Out += 4)
    {
        j = (In[0] << 1) + In[5];
        k = (In[1] << 3) + (In[2] << 2) + (In[3] << 1) + In[4];
        ByteToBit(Out, &S_Box[i][j][k], 4);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void Transform(bool *Out, bool *In, const char *Table, int len)
{
    for (int i = 0; i < len; ++i)
    {
        Tmp[i] = In[Table[i] - 1];
    }
    memcpy(Out, Tmp, len);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void Xor(bool *InA, const bool *InB, int len)
{
    for (int i = 0; i < len; ++i)
    {
        InA[i] ^= InB[i];
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void RotateL(bool *In, int len, int loop)
{
    memcpy(Tmp, In, loop);
    memcpy(In, In + loop, len - loop);
    memcpy(In + len - loop, Tmp, loop);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void ByteToBit(bool *Out, const char *In, int bits)
{
    for (int i = 0; i < bits; ++i)
    {
        Out[i] = (In[i >> 3] >> (i &7)) &1;
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

void BitToByte(char *Out, const bool *In, int bits)
{
    memset(Out, 0, bits >> 3);
    for (int i = 0; i < bits; ++i)
    {
        Out[i >> 3] |= In[i] << (i &7);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

static char base64_chars[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int base64_encode(const void *data, int size, char **str)
{
    char *s,  *p;
    int i;
    int c;
    const unsigned char *q;

    p = s = (char*)malloc(size *4 / 3+4);
    if (p == NULL)
    {
        return  - 1;
    }
    q = (const unsigned char*)data;
    i = 0;
    for (i = 0; i < size;)
    {
        c = q[i++];
        c *= 256;
        if (i < size)
        {
            c += q[i];
        }
        i++;
        c *= 256;
        if (i < size)
        {
            c += q[i];
        }
        i++;
        p[0] = base64_chars[(c &0x00fc0000) >> 18];
        p[1] = base64_chars[(c &0x0003f000) >> 12];
        p[2] = base64_chars[(c &0x00000fc0) >> 6];
        p[3] = base64_chars[(c &0x0000003f) >> 0];
        if (i > size)
        {
            p[3] = '=';
        }
        if (i > size + 1)
        {
            p[2] = '=';
        }
        p += 4;
    }
    *p = 0;
    *str = s;
    return strlen(s);
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


static int pos(char c)
{
    char *p;
    for (p = base64_chars;  *p; p++)
        if (*p == c)
        {
            return p - base64_chars;
        }
    return  - 1;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

#define DECODE_ERROR 0xffffffff

static unsigned int token_decode(const char *token)
{
    int i;
    unsigned int val = 0;
    int marker = 0;
    if (strlen(token) < 4)
    {
        return DECODE_ERROR;
    }
    for (i = 0; i < 4; i++)
    {
        val *= 64;
        if (token[i] == '=')
        {
            marker++;
        }
        else if (marker > 0)
        {
            return DECODE_ERROR;
        }
        else
        {
            val += pos(token[i]);
        }
    }
    if (marker > 2)
    {
        return DECODE_ERROR;
    }
    return (marker << 24) | val;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

int base64_decode(const char *str, void *data)
{
    const char *p;
    unsigned char *q;

    q = (unsigned char*)data;
    for (p = str;  *p && (*p == '=' || strchr(base64_chars,  *p)); p += 4)
    {
        unsigned int val = token_decode(p);
        unsigned int marker = (val >> 24) &0xff;
        if (val == DECODE_ERROR)
        {
            return  - 1;
        }
        *q++ = (val >> 16) &0xff;
        if (marker < 2)
        {
            *q++ = (val >> 8) &0xff;
        }
        if (marker < 1)
        {
            *q++ = val &0xff;
        }
    }
    return q - (unsigned char*)data;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

char *decryptXOR(const char *str, unsigned int str_len, const char *key)
{
    if (!str || !key)
    {
        return NULL;
    }

    char *data = (char*)malloc(sizeof(char) *str_len);
    if (!data)
    {
        return NULL;
    }
    //no memory

    if (base64_decode(str, data) ==  - 1)
    {
        free(data);
        return NULL;
    }

    char *res = (char*)malloc(str_len + 1);

    unsigned int i, j;
    for (i = 0; i < str_len; i++)
    {
        for (j = 0; j < strlen(key); j++)
        {
            res[i] = key[j] ^ data[i];
        }
    }
    res[str_len] = '\0';
    free(data);

    return res;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

char *encryptXOR(const char *str, unsigned int str_len, const char *key)
{
    if (!str || !key)
    {
        return NULL;
    }

    char *XOR = NULL;
    char *res = (char*)malloc(str_len);
    unsigned int i, j;
    for (i = 0; i < str_len; i++)
    {
        for (j = 0; j < strlen(key); j++)
        {
            res[i] = str[i] ^ key[j];
        }
    }
    int len = base64_encode(res, str_len, &XOR);
    XOR[len] = '\0';
    free(res);
    return XOR;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

char *fillcode(const char *str)
{
    char *base64 = NULL;
    int str_len = strlen(str);
    int len = (((str_len + 7) >> 3) << 3);
    char *res = (char*)malloc(sizeof(char) *len);
    if (!res)
    {
        return NULL;
    }

    memcpy(res, str, len);

    //fix a bug of windows...
    for (int i = len - (str_len + 1); i >= 0; i--)
    {
        res[str_len + i] = 'A' - 66;
    }

    base64_encode(res, len, &base64);

    if (res)
    {
        free(res);
    }
    return base64;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

char *resolvecode(const char *str)
{
    int i;
    int len = strlen(str);
    char *base64 = (char*)malloc(sizeof(char) *len + 1);
    if (!base64)
    {
        return NULL;
    }
    //no memory

    int base_res = base64_decode(str, base64);
    if (base_res ==  - 1)
    {
        free(base64);
        return NULL;
    }
    base64[base_res] = '\0';

    //printf("getNu is len is %d,base64 [%s]\n",len, base64);
    for (i = 0; i < len; i++)
    {
        if (base64[i] == 'A' - 66)
        {
            base64[i] = '\0';
            return base64;
        }
    }
    base64[i] = '\0';
    return base64;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

char *DES_3(const char *data, long datalen, bool isEncry)
{
    char *encry_data = (char*)data;
    char *tmp = NULL;
    char *res = NULL;

    if (!isEncry)
    {
        if (!(tmp = (char*)malloc(sizeof(char) *datalen)))
        {
            return NULL;
        }
        //no memory

        if (base64_decode(data, tmp) ==  - 1)
        {
            free(tmp);
            return NULL;
        }
        encry_data = tmp;
    }

    //int outLen = ((datalen+ 7)>>3)<<3;
    int outLen = ((strlen(encry_data) + 7) >> 3) << 3;

    char *out = (char*)malloc(sizeof(char) *outLen);
    if (!out)
    //fprintf(stderr, "out of the memory\n");
    {
        return NULL;
    }

    //DES(out, encry_data, datalen, KEY, strlen(KEY), isEncry);
    DES(out, encry_data, strlen(encry_data), KEY, strlen(KEY), isEncry);
    //out[outLen] = '\0';

    if (isEncry)
    {
        base64_encode(out, outLen, &res);
    }
    else
    {
        if (tmp)
        {
            free(tmp);
        }
        res = out;
    }
    return res;
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/

char *encryPWD(const char *data, long datalen, bool isEncry)
{
    if (isEncry)
    {
        return encryptXOR(data, (unsigned int)datalen, (const char*)KEY);
    }
    else
    {
        return decryptXOR(data, (unsigned int)datalen, (const char*)KEY);
    }
}

/**************************************************************************/
/*                                                                        */
/**************************************************************************/


#if 0
    int main()
    {
        char str[] = "你好中国   <> // // \\!!@! \" sadasd";
        //char str[] = "sdfsdfsdf";
        char *key = "www.libfetion.cn";

        char *res = encryptXOR(str, strlen(str), key);
        printf("res is %s !!!!\n", res);

        printf("decode %s \n", decryptXOR(res, strlen(res), key));
        return 0;
    }
#endif
