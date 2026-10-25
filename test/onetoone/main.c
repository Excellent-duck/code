#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include "frame.h"
#include "crypto.h"



int main()
{
//========================================================================
//测试打包
//========================================================================
    frame_t tx_frame;
    frame_t re_frame;
    uint8_t buffer[512];


    //开始填充结构体
    tx_frame.magic = FRAME_MAGIC;
    tx_frame.msg_type = MSG_TYPE_DATA;
    tx_frame.seq = 0x01;
    strcpy((char*)tx_frame.payload,"hello");
    tx_frame.payload_len = strlen((char*)tx_frame.payload);

    printf("---send-----");
    printf("source data：msg_type=%d,seq=%d,payload=%s(长度&d)\n",tx_frame.msg_type,tx_frame.seq,tx_frame.payload,tx_frame.payload_len);


    int encoded_len = frame_encode(&tx_frame,buffer,sizeof(buffer));
    if(encoded_len < 0)
    {
        printf("pack failed\n");
        return -1;
    }
    printf("pack ok len%d\n",encoded_len);
    printf("byte_stream(hex):");
    for(int i=0;i<encoded_len;i++)
    {
        printf("%02X",buffer[i]);

    }
      printf("\n\n");
//----------------------------------------------------------------------------------------------------
//打包data测试加密
     encoded_len = frame_encode(&tx_frame,buffer,sizeof(buffer));
     uint8_t key[]={0x11,0x22,0x33};
     uint8_t nonce[]={0xAA,0xBB};
     uint8_t encrypted_buffer[512];

     stream_crypt(key,3,nonce,2,buffer,encoded_len,encrypted_buffer);
     printf("encrypted data:");
     for(int i=0;i<encoded_len;i++)
     {
        printf("%02X",encrypted_buffer[i]);
     }
     printf("\n");


//====================================================================================================
//拆包
//====================================================================================================
//拆包测试解密
    uint8_t dencrypted_buffer[512];
    stream_crypt(key,3,nonce,2,encrypted_buffer,encoded_len,dencrypted_buffer);

    printf("dencrypted_buffer:");
    for(int i = 0;i<encoded_len; i++)
    {
        printf("%02X",dencrypted_buffer[i]);
    }
    printf("\n");
//--------------------------------------------------------------------------------------------------------
    memset(&re_frame,0,sizeof(frame_t));
    printf("----receive-----");
    int consumed_len = frame_decode(buffer,encoded_len,&re_frame);
    if(consumed_len<0)
    {
         printf("depack failed\n");
         return -1;

    }
        printf("depack ok len:%dbyte\n",consumed_len);
        printf("analyse data:msg_type=%d,seq=%d,payload=%s(长度：%d)\n",re_frame.msg_type,re_frame.seq,re_frame.payload,re_frame.payload_len);



    if(tx_frame.seq == re_frame.seq && tx_frame.payload_len == re_frame.payload_len && strcmp((char*)tx_frame.payload,   (char*)re_frame.payload) == 0)
   {
            printf("ok");
    }else{
             printf("not ok");
     }
      return 0;
}