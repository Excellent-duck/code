#include<string.h>
#ifndef FRAME_H
#define FRAME_H
#include<stdint.h>
#define FRAME_MAGIC      0XEB01
#define FRAME_MAX_PAYLOAD   256

/*msg type*/
#define MSG_TYPE_HANDSHAKE  0X01
#define MSG_TYPE_DATA       0X02

typedef struct 
{
    uint16_t magic;    ///固定frame_magic
    uint8_t msg_type;  ///msg_type
    uint32_t seq;      ///序列号，小端
    uint16_t payload_len;   ///明文载荷
    uint8_t payload[FRAME_MAX_PAYLOAD];/* 载荷数据*/

}frame_t;


/*
*把frame_t 打包成字节流，写入buf
*成功返回写入的字节数（>0),失败返回-1
*字节序：小端
*布局： magic(2)  |msg_type(1)|seq(4)|payload_len(2)|payload(payload_len)
*/
int frame_encode(const frame_t *f,uint8_t *buf,int buf_size);

/*
*从字节流buf中解析frame_t
*成功返回消耗的字节数（>0），失败返回-1
*必须校验：magic正确性，payload_len<=FRAME_MAX_PAYLOAD，len足够
*/
int frame_decode(const uint8_t *buf,int len,frame_t *f);



static void put_u16_le(uint8_t *p,uint16_t v);
static void put_u32_le(uint8_t *q,uint16_t v);



#endif

/*
*写的时候犯了一个很严重的错误，把实现函数放在头文件里面，如果有多个实现函数，那么编译的时候会把多次重复编译实现函数
*这样会导致在链接器链接的时候傻眼，不知道用哪一个函数
*所以要区分.h和.c文件怎么配合，记住一句：.h是菜单，.c是后厨*/