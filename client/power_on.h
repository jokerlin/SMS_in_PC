/*************************************************************************
    > File Name: power_on.h
    > Author: linheng
    > Mail: i@haroquee.me 
    > Created Time: Sat 31 Aug 2013 09:54:56 PM CST
 ************************************************************************/

#ifndef c_power_on_h
#define c_power_on_h

void welcome();//欢迎屏
void power();//开机函数

long long local_phoneNumber;
char server_ip[16];
int local_port;
int server_port;

#define WELCOME_POS_X 4 
#define WELCOME_POS_Y 25 

#define INDEX_POS_X WELCOME_POS_X + 6
#define INDEX_POS_Y 35
#define INDEX_ADD_X 3

unsigned char content_INDEX[3][20];
int index_INDEX_CONTENT;

#endif
