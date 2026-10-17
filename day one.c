//给定一个整数数组，nums
// 给定一个目标值target
// 找出和为target的整数并返回下标
#include<stdio.h>
#include<stdlib.h>
 int i,k,m,n;
 int length;
 int target;
 int nums[10000];
 struct a{
   int *p;
   int value1;
 }head;
 struct b{
    int *q;
    int value2;
 }tail;
  int main(){
    printf("please input the length of nums:");
    scanf("%d",&length);
    printf("please input the target of nums:");
    scanf("%d",&target);
    
    if(length>=2 && length<=10000)
    printf("length is valid \n");
    else{
        printf("length is novalid \n");
    }
    if(target>=-100000 && target<=100000)
    printf("target is valid \n");
    else
    {
        printf("target is novalid \n");
    }
       for(i=0;i<length;i++)
       {
           printf("please input the %dth element of nums:",i);
           scanf("%d",&nums[i]);
           if(nums[i]>=-100000 && nums[i]<=100000)
           printf("the element id valid \n");
           else
           {
            printf("the element is novalid \n");
           }
       }
      
    
       for(k=0;k<length;k++)
       {
        head.p = &nums[k];
        head.value1 = nums[k];
        for(m=0;m<length;m++)
           {
              tail.q = &nums[m];
              tail.value2 = nums[m];
              if(head.value1 + tail.value2 == target)
              {
                 printf("head location is %02X ,tail location is %02X ",head.p,tail.q);
              }
           }
       }
    }
  