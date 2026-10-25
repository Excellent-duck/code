#include "crypto.h"
#include<stdint.h>


void stream_crypt(const uint8_t *key,int key_len,const uint8_t *nonce,int nonce_len,const uint8_t *in,int len,uint8_t *out)
{
    uint32_t state =2166136261;
    //1.先把key和nonce混合进state
    for(int i=0;i<key_len;i++)
    {
       state = (state^key[i])*16777619;
    }
    for(int i = 0;i<nonce_len;i++)
    {
        state = (state^nonce[i])*16777619;
    }
    for(int i = 0;i<len;i++)
    {
        state = (state^ (uint32_t )i)*16777619;
       uint8_t statelow8 = (uint8_t)(state&0x000000FF);//取低8位作为当前字节的密钥流
       out[i] = in[i] ^ (uint8_t)statelow8;
    }
}