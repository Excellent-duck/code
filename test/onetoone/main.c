#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include "frame.h"



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
    printf("字节流（hex）：");
    for(int i=0;i<encoded_len;i++)
    {
        printf("%02X",buffer[i]);

    }
      printf("\n\n");


//====================================================================================================
//拆包
//====================================================================================================
         memset(&re_frame,0,sizeof(frame_t));
         printf("----receive-----");
         int consumed_len = frame_decode(buffer,encoded_len,&re_frame);
    if(consumed_len<0)
    {
         printf("depack failed\n");
         return -1;

    }
        printf("depack ok len:%dbyte\n",consumed_len);
        printf("analyse data：msg_type=%d,seq=%d,payload=%s(长度：%d)\n",re_frame.msg_type,re_frame.seq,re_frame.payload,re_frame.payload_len);



    if(tx_frame.seq == re_frame.seq && tx_frame.payload_len == re_frame.payload_len && strcmp((char*)tx_frame.payload,   (char*)re_frame.payload) == 0)
   {
            printf("ok");
    }else{
             printf("not ok");
     }
      return 0;
}