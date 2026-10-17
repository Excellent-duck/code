#ifndef protocol.h
#define protocol.h
#include<stdio.h>
#include<math.h>





//上位机----->下位机的指令帧（固定5byte）
typedef struct                                                                                                                                                                                                                        
{
    UINT8 head;
    UINT8 cmd;
    UINT8 len;
    UINT32 crc;
    UINT8 tail;/* data */
}cmd frame;


//下位机------>上位机 应答帧
typedef struct                                                                                                                                                                                                                        
{
  UINT8 head;
  UINT8 cmd;
  UINT8 len;
  UINT8 data[16];
  UINT32 crc;
  UINT8 tail;/* data */
}RESPON frame;


UINT8 calc CRC(UINT8 CMD,UINT8 LEN,const UINT8 *DATA);
void float(float f,uint8 *buff);
void handle cmd(const CMD Frame *CMD Frame);

#endif