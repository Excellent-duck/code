#include <stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include <windows.h>

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
RingBuffer_t g_rb;


void ring_buffer_init(RingBuffer_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
}

bool ring_buffer_is_full(RingBuffer_t *rb)//判断是否满了
{
    if((rb->head+1)% RING_BUFFER_SIZE == rb->tail)
    {
        return true;
    }
}
bool ring_buffer_is_empty(RingBuffer_t *rb)//判空
{
    if(rb->tail == rb->head)
    {
        return true;
    }
}
bool ring_buffer_put(RingBuffer_t *rb,uint32_t data)
{
    if(true ==  ring_buffer_is_full(rb))
    {
        return false;
    }else{
        rb->buffer[rb->head] = data;
        rb->head = (rb->head +1)%RING_BUFFER_SIZE;
        return true;
        
    }    
}


bool ring_buffer_get(RingBuffer_t *rb,uint32_t *p_data)
{
    if(true == ring_buffer_is_empty(rb))
    {
        return false;
    }else{
       *p_data = rb->buffer[rb->tail];
       rb->tail = (rb->tail + 1)%RING_BUFFER_SIZE;
       return true; 
    }
}



// //===================================================================
// //中断模拟（生产者线程）
// //===================================================================


// //这个函数会被系统单独分配一个线程，相当于硬件中断
// DWORD WINAPI Interrupt_Simulator(LPVOID IpParam)
// {
//     uint32_t data = 100;//模拟发送数据从100开始
//     while(1)
//     {
//         //把数据塞进缓冲池
//         if(ring_buffer_put(&g_rb,data)==true)
//         {
//             printf("[中断]成功放入数据：%u\n",data);
//             data++;
//         }else{
//             printf("[中断]缓冲池满了，丢弃数据：%u\n",data);
//             data++;

//             //模拟硬件产生数据的速度（比方每200毫秒产生一个）
//             //Sleep就是让这个线程休眠，把cpu让给别人
//             Sleep(200);
//         }
      
//     }
//     return 0;
// }

// //=============================================================================
// //主循环模拟（消费者）
// //==============================================================================
// int main()
// {
//     ring_buffer_init(&g_rb);
//     //告诉windows：拍一个专门的线程去跑Interrupt_Simulator（模拟中断）
//     CreateThread(NULL,0,Interrupt_Simulator,NULL,0,NULL);
//     printf("===========模拟中断并发测试开始===========");
    
    
//     //主循环一直在跑
//     while(1)
//     {
//         uint32_t read_data =0;
//         //消费者逻辑：不断从池子里面拿数据
//         if(ring_buffer_get(&g_rb,&read_data) == true)
//         {
//             printf("----->[主程序]成功读取数据：%u\n",read_data);
//         }

//         Sleep(600);
//     }
//     return 0;
    
// }

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
    // printf("===chonggou start");
    // process_wave_update(0,0x00000001,0x01);
    // printf("node 0,wave 0,data: 0x%04X\n",g_node_wave_config[0].wave_data[0]);
    // printf("node 0,wave 32 data: 0x%04X\n",g_node_wave_config[0].wave_data[32]);

    printf("======环形缓冲池=========");
    RingBuffer_t my_rb;
    ring_buffer_init(&my_rb);//初始化池子

    uint32_t read_data = 0;
    printf("\nxie data : 111,222,333\n");
    ring_buffer_put(&my_rb,111);
    ring_buffer_put(&my_rb,222);
    ring_buffer_put(&my_rb,333);

    if(ring_buffer_get(&my_rb,&read_data))//第一次读数据
    {
        printf("read data is %u\n",&read_data);
    }
        if(ring_buffer_get(&my_rb,&read_data))//第二次读数据
    {
        printf("read data is %u\n",read_data);
    }
        if(ring_buffer_get(&my_rb,&read_data))//第三次读数据
    {
        printf("read data is %u\n",read_data);
    }
        if(ring_buffer_get(&my_rb,&read_data))//第四次读data
    {
        printf("read data is %u\n",read_data);
    }else{
        printf("读第四次数据 is not ok");
    }


    return 0;
}





// //=======================函数返回类型========================================
// //写函数时，决定怎么用哪个？对自己问三句话

// //第一句：我需要这个函数给我一个结果吗？  
// //no----用void     yes---问第二句

// // 第二句：我需要的是“是/否”，还是“一个数字”
// // 是/否：用bool
// // 数字：继续问第三句

// // 第三句：这个数字可能是负数吗？
// // 可能是负数：用int
// // 不可能是负数（数量，大小）：用unsigned int

// //还有返回指针地址类型或者结构体类型
// //===================================================================================