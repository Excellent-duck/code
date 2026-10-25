#ifndef CRYPTO_H
#define CRYPTO_H

#include<stdint.h>


/*
流密码加密解密函数
key：密钥
key_len：密钥长度
monce：随机数（或者叫种子，保证每一次加密结果不一样）
nonce_len:随机数长度
in：输入明文或者密文
len：数据长度
out：输出的明文或密文
*/

void stream_crypt(const uint8_t *key,int key_len,const uint8_t *nonce,int nonce_len,const uint8_t *in,int len,uint8_t *out);

#endif