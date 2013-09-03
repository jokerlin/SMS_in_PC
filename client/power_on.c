/*************************************************************************
    > File Name: power.h
    > Author: linheng
    > Mail: i@haroquee.me 
    > Created Time: Fri 30 Aug 2013 09:50:55 PM CST
 ************************************************************************/
#include "power_on.h"
#include "getprofile.h"
#include <stdio.h>

long long local_phoneNumber;
char[16] server_ip;
int local_port;
int server_port;

void welcome()
{
	printf("/#########################################################################\n\n");
	printf("	> Welcome to use SMS_in_PC\n\n");
	printf("	> Created by My Nine Partners and Me\n\n");
    printf("#########################################################################/\n\n");
}

void power_on()
{
	int flag = 0;//读取配置文件成功标志，0为成功，-1为失败
	flag = GetProfileString("./client.conf","server","ip",server_ip);
	if (flag == -1)
	{

	}

	flag = GetProfileString("./client.conf","server","port",server_port);
	if (flag == -1)
	{

	}
	flag = GetProfileString("./client.conf","client","port",local_port);
	if (flag == -1)
	{

	}
	printf("Please Enter Your Telephone Number: "); 
	scanf("%lld",&local_phoneNumber);//输入电话号码
	list_person(1);
	sock_power_on(longlong_to_string(local_phoneNumber),server_ip,server_port,local_port);
}
