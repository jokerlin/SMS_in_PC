/*************************************************************************
    > File Name: power.h
    > Author: linheng
    > Mail: i@haroquee.me
    > Created Time: Fri 30 Aug 2013 09:50:55 PM CST
 ************************************************************************/
#include "power_on.h"
#include "getprofile.h"
#include "sandtears.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <curses.h>
#include <locale.h>

#include "willzhang.h"
#include "sandtears.h"
#include "string_to_message.h"

/*
long long local_phoneNumber;
char server_ip[16];
int local_port;
int server_port;
*/


void print_content()
{
	mvaddstr(INDEX_POS_X + index_INDEX_CONTENT * INDEX_ADD_X, INDEX_POS_Y, content_INDEX[index_INDEX_CONTENT]);
}
void welcome()
{
	attron(A_BOLD); 
    mvaddstr(LINES-2,4,"q 返回");
    mvaddstr(LINES-2,10,"f 搜索");
    mvaddstr(LINES-2,22,"ENTER 选中");
    mvaddstr(LINES-2,33,"d 删除");
    mvaddstr(LINES-2,42,"按上下方向键移动");
    mvaddstr(LINES-2,60,"按左右方向键翻页");
	attroff(A_BOLD);

	strcpy(content_INDEX[0],"收件箱");
	strcpy(content_INDEX[1],"发短信");
	strcpy(content_INDEX[2],"通讯录");

	mvaddstr(WELCOME_POS_X, WELCOME_POS_Y,"Welcome to use SMS_in_PC");
	mvaddstr(WELCOME_POS_X + 1, WELCOME_POS_Y - 5, "Created by My Nine Partners and Me");
	mvaddstr(INDEX_POS_X, INDEX_POS_Y, "收件箱");
	mvaddstr(INDEX_POS_X + INDEX_ADD_X, INDEX_POS_Y, "发短信");
	mvaddstr(INDEX_POS_X + INDEX_ADD_X * 2, INDEX_POS_Y, "通讯录");

	move(INDEX_POS_X, INDEX_POS_Y);
	attron(A_REVERSE);
	mvaddstr(INDEX_POS_X, INDEX_POS_Y, "收件箱");
	index_INDEX_CONTENT = 0;

	
	refresh();
	
	//getchar();
}

int power_on()
{
	int flag = 0;//读取配置文件成功标志，0为成功，-1为失败

	int status;
	char buff[] = "./newdir";
	char buf[1024];
	DIR *dir=NULL
;	dir = opendir(buff);
	char phone[12];
	char id[50];
	char fileid[50];

    char server_ip_s[20];
    char server_port_s[20];
    char local_port_s[20];

    //longlong_to_string(server_port, server_port_s);
    //longlong_to_string(local_port, local_port_s);

	flag = GetProfileString("./client.conf","server","ip",server_ip);
	if (flag == -1)
	{
		printf("读取配置文件失败！\n");
		return -1;
	}

	flag = GetProfileString("./client.conf","server","port",server_port_s);
	if (flag == -1)
	{
		printf("读取配置文件失败！\n");	\
		return -1;
	}
	server_port = string_to_longlong(server_port_s,strlen(server_port_s)-1+server_port_s);
	flag = GetProfileString("./client.conf","client","port",local_port_s);
	if (flag == -1)
	{
		printf("读取配置文件失败！\n");
		return -1;
	}
	local_port = string_to_longlong(local_port_s,strlen(local_port_s)-1+local_port_s);
	printf("请输入您的电话号码：");
	scanf("%lld",&local_phoneNumber);//输入电话号码

	longlong_to_string(local_phoneNumber, phone);
	//printf ("%s\n",phone);
	memset(buf,0,sizeof(buf));
	id[0]='.';id[1]='/';id[2]='\0';
	strcat(id,phone);
	if(opendir(id) == NULL)
	{
		status = mkdir(id, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		int fd;
		strcpy(fileid,id);
		strcat(fileid,"/DataFile");
		fd=creat(fileid,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        char instruction[128];
        memset(instruction, '\0', sizeof(instruction));
        strcpy(instruction, "dd if=/dev/zero of=");
        strcat(instruction, fileid);
        strcat(instruction, " bs=1024 count=1024 1>/dev/null 2>&1");
        system(instruction);
        //write(fd,buf,1024);
		close(fd);
		memset(fileid,'\0',sizeof(fileid));
		strcpy(fileid,id);
		//	printf("%s\n",fileid);
		strcat(fileid,"/PersonRecycleBin");
		fd=creat(fileid,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        memset(instruction, '\0', sizeof(instruction));
        strcpy(instruction, "dd if=/dev/zero of=");
        strcat(instruction, fileid);
        strcat(instruction, " bs=1024 count=1024 1>/dev/null 2>&1");
        system(instruction);
        close(fd);
		memset(fileid,'\0',sizeof(fileid));
		strcpy(fileid,id);
		strcat(fileid,"/MessageRecycleBin");
		fd=creat(fileid,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		memset(instruction, '\0', sizeof(instruction));
        strcpy(instruction, "dd if=/dev/zero of=");
        strcat(instruction, fileid);
        strcat(instruction, " bs=1024 count=1024 1>/dev/null 2>&1");
        system(instruction);
		close(fd);
		DataInit(id);
		DataBaseStart(id);
	}
	else
	{
		DataBaseStart(id);
	}

	//list_person(0);
	char local_phoneNumber_s[12];
	longlong_to_string(local_phoneNumber, local_phoneNumber_s);
	int sockfd =  sock_power_on(local_phoneNumber_s,server_ip,server_port,local_port);
	if (sockfd == -1)
	{
		printf("无法连接到远程服务器，请检查网路连接是否正常以及配置文件是否正确\n");
		return -1;
	}
	return sockfd;
}
