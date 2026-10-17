#include <stdio.h>
#include<stdint.h>
#include<stdbool.h>

//----------------------------------------------------
//      宏定义区
//------------------------------------------------------

#define MAX_NODES             24     //最大节点数
#define WAVES_PER_NODE        40     //每个节点包含的波形总数
#define WAVES_FIRST_HALF      32     //前半段波形数
#define WAVES_SECOND_HALF      8     //后半段波形数
#define BASE_ADDR             0x400  // 寄存器地址


//----------------------------------
//   数据结构定义区
//-------------------------------------


typedef struct{
    uint16_t wave_data[WAVES_PER_NODE];//存放40个波形的最后数据

}NodeWaveConfig_t;

NodeWaveConfig_t g_node_wave_config[MAX_NODES];  //全局暂存区



//===================================================================
//环形缓存区
//==================================================================
#define RING_BUFFER_SIZE   10//缓冲池只能装10个数据
typedef struct{
    uint32_t buffer[RING_BUFFER_SIZE];
    uint32_t head;//写指针
    uint32_t tail;//读指针
}RingBuffer_t;

void ring_buffer_init(RingBuffer_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
}

bool ring_buffer_is_full(RingBuffer_t *rb)
{
    if((rb->head+1)% RING_BUFFER_SIZE == rb->tail)
    {
        return false;
    }
}






//==============================================================
//底层模拟硬件区
//==================================================================

void pcieRead(uint32_t addr, uint32_t *pData,uint8_t len)
{
    *pData =0x12345678;
}



//===================================================
//底层读取区
//===================================================
bool read_wave_register(uint8_t node_idx,uint8_t wave_idx,uint32_t *p_out_data)
{
    if((0<= node_idx <= 23) &&(0<=wave_idx<=40))
    {
       uint32_t k = BASE_ADDR+node_idx*WAVES_PER_NODE+wave_idx;
        pcieRead( k, p_out_data,1);
        return true;
    }else{
        return false;
    }
    
}

//====================================================
// 业务处理层
//========================================================
void process_wave_update(uint8_t node_idx,uint8_t update_status,uint8_t extra_status)
{
    uint32_t nodeData = 0;
    uint8_t j = 0;
    //处理32个波形
    for(j=0;j<WAVES_FIRST_HALF;j++)
    {
       if((update_status & (1 << j)) != 0)
       {
        if(read_wave_register(node_idx,j,&nodeData) == true)
        {
        uint16_t new_data=(uint16_t)(nodeData>>16);
        uint16_t cur_data= (uint16_t)(nodeData&0xFFFF);
        if(new_data != cur_data)
            {
            g_node_wave_config[node_idx].wave_data[j] = new_data;
            }
         }
        }
    }
    for(j=0;j<WAVES_PER_NODE;j++)
    {
        if((extra_status & (1 << j)) !=0)
        {
         uint8_t real_j=j+WAVES_FIRST_HALF;
            if(read_wave_register(node_idx,real_j,&nodeData)==true)
                {
                 uint16_t new_data=(uint16_t)(nodeData>>16);
                 uint16_t cur_data= (uint16_t)(nodeData&0xFFFF);
                   if(new_data != cur_data)
                      {   
                         g_node_wave_config[node_idx].wave_data[real_j] = new_data;
                      }
                }
        }
    }

}


int main()
{
    printf("===chonggou start");
    process_wave_update(0,0x00000001,0x01);
    printf("node 0,wave 0,data: 0x%04X\n",g_node_wave_config[0].wave_data[0]);
    printf("node 0,wave 32 data: 0x%04X\n",g_node_wave_config[0].wave_data[32]);
    return 0;
}