#include<stdio.h>
#include<time.h>
typedef struct test
{
   int year;
   int month;
   int day;
   int hour;
   int minture;
   int second;
   /* data */
}onlytime;
void get_current_time(onlytime *t) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    t->year = tm_info->tm_year + 1900;
    t->month = tm_info->tm_mon + 1;
    t->day = tm_info->tm_mday;
    t->hour = tm_info->tm_hour;
    t->minture = tm_info->tm_min;
    t->second = tm_info->tm_sec;
}

 int main()
 {
   onlytime t;
   get_current_time(&t);
    printf("当前时间：%d-%02d-%02d %02d:%02d:%02d\n", t.year, t.month, t.day, t.hour, t.minture, t.second);
    printf("ok");
    printf("123445");
    return 0;
 }