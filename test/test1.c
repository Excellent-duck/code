#define initsize 100  //表长度初定义
#include<stdio.h>
#include<stdlib.h>
typedef struct
{
  int *data;  //指示动态分配数组指针
  int maxsize,length; /* data */ //数组最大容量和当前个数
}seqlist;  //动态分配数组顺序表的定义类型

void initlist(seqlist L){
   L.data=(int*)malloc(initsize*sizeof(int));  //分配存储空间
   L.length=0;          //顺序表初始化长度为0
   L.maxsize=initsize;   //初始化存储容量
}
int main(){
   seqlist L;
   initlist(L);
   printf("顺序表最大长度，&d\n");
   return 0;
}

// bool listinsert(seqlist &L,int i,int e){
//    if(i<1||i>L.length+1)
//    return false;
//    if(L.length>=maxsize)
//    return false;
//    for(int j=L.length;j>=i;j--)
//    L.data[j]=L.data[j-1];
//    L.data[i-1]=e;
//    L.length++;
//    return true;
// }
