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
#include "willzhang.h"
#include "sandtears.h"
#include "string_to_message.h"
/*
long long local_phoneNumber;
char server_ip[16];
int local_port;
int server_port;
*/
void welcome()
{
	printf("/#########################################################################\n\n");
	printf("	> Welcome to use SMS_in_PC\n\n");
	printf("	> Created by My Nine Partners and Me\n\n");
    printf("#########################################################################/\n\n");
}

int power_on()
{
	int flag = 0;//读取配置文件成功标志，0为成功，-1为失败

	int status;
	char buff[] = "./newdir";
	char buf[1024];
	DIR *dir=NULL;
	dir = opendir(buff);
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

	}

	flag = GetProfileString("./client.conf","server","port",server_port_s);
	if (flag == -1)
	{

	}
	server_port = string_to_longlong(server_port_s,strlen(server_port_s)-1+server_port_s);
	flag = GetProfileString("./client.conf","client","port",local_port_s);
	if (flag == -1)
	{

	}
	local_port = string_to_longlong(local_port_s,strlen(local_port_s)-1+local_port_s);
	printf("Please Enter Your Telephone Number: ");
	scanf("%lld",&local_phoneNumber);//输入电话号码

	longlong_to_string(local_phoneNumber, phone);
	//printf ("%s\n",phone);
	memset(buf,0,sizeof(buf));
	id[0]='.';id[1]='/';id[2]='\0';
	strcat(id,phone);
	char exitinstruction[50];
	strcpy(exitinstruction,"cd ");
	strcat(exitinstruction,id);
	if(opendir(exitinstruction) == NULL)
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
        strcat(instruction, " bs=1024 count=1024");
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
        strcat(instruction, " bs=1024 count=1024");
        system(instruction);
        close(fd);
		memset(fileid,'\0',sizeof(fileid));
		strcpy(fileid,id);
		strcat(fileid,"/MessageRecycleBin");
		fd=creat(fileid,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		memset(instruction, '\0', sizeof(instruction));
        strcpy(instruction, "dd if=/dev/zero of=");
        strcat(instruction, fileid);
        strcat(instruction, " bs=1024 count=1024");
        system(instruction);
		close(fd);
		DataInit(id);
		DataBaseStart(id);
	}
	else
	{
		DataBaseStart(id);
	}

	list_person(0);
	char local_phoneNumber_s[12];
	longlong_to_string(local_phoneNumber, local_phoneNumber_s);
	return sock_power_on(local_phoneNumber_s,server_ip,server_port,local_port);

}
