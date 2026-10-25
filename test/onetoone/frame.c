#include<stdio.h>
#include"frame.h"
#include<stdint.h>
#include<string.h>



//============================================================================================
//把结构体转换字节流
static void put_u16_le(uint8_t *p,uint16_t v)
{
    p[0]=(uint8_t)(v&0xFF);
    p[1]=(uint8_t)((v>>8)&0xFF);  
}
static void put_u32_le(uint8_t *q,uint16_t v)
{
    q[0]=(uint8_t)(v&0x000000FF);
    q[1]=(uint8_t)((v>>8)&0x000000FF);
    q[2]=(uint8_t)((v>>16)&0x000000FF);
    q[3]=(uint8_t)((v>>24)&0x000000FF);
}
//===============================================================================================


//===============================================================================================
//字节流转结构体
static uint16_t get_u16_le(const uint8_t *x )
{
     uint16_t result_16 = ((x[1]<<8)|x[0]);
}
static uint32_t get_u32_le(const uint8_t *y)
{
    uint32_t result_32 = ((y[3]<<24)|(y[2]<<16)|(y[1]<<8)|y[0]);
}

//又犯了一个问题，const是只读，刚刚static后面用了void，表示无返回值，这是错误的，应该改

//===============================================================================================




//===============================================================================================
///进行加密帧格式判断
int frame_encode(const frame_t *f,uint8_t *buf,int buf_size)
{
    int total_len;
    if((f==NULL )||(buf==NULL))
    {
       return -1;   
    }
   
    if(f->payload_len > FRAME_MAX_PAYLOAD)
    {
        return -1;
    }
    total_len = 9+f->payload_len;
   if(total_len > buf_size)
    {
        return -1;
    }

    
    put_u16_le(buf,f->magic);
    buf[2]=f->msg_type;
    put_u32_le(buf+3,f->seq);
    put_u16_le(buf+7,f->payload_len);
    memcpy(buf+9,f->payload,f->payload_len);

    return total_len;
}
//===================================================================================================




//===================================================================================================
//进行解密帧格式判断
int frame_decode(const uint8_t *buf,int len,frame_t *f)
{
   if((f == NULL)||(buf == NULL))
   {
    return -1;
   }
   if(len < 9)
   {
    return -1;
   }
   if(get_u16_le(buf) != FRAME_MAGIC )
   {
     return -1;
   }
   uint16_t payload_len = get_u16_le(buf+7);

   if(payload_len > FRAME_MAX_PAYLOAD)
   {
    return -1;
   }
   if(payload_len + 9 >len)
   {
    return -1;
   }
   f->magic = FRAME_MAGIC;
   f->msg_type = buf[2];
   f->seq = get_u32_le(buf+3);
   f->payload_len = get_u16_le(buf+7);
   memcpy(f->payload,buf+9,payload_len);

   return 9+payload_len;
}