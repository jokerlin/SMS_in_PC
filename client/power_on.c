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

void power_on()
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

	longlong_to_string(local_phoneNumber, phone);
	//printf ("%s\n",phone);
	memset(buf,0,sizeof(buf));
	id[0]='.';id[1]='/';
	strcat(id,phone);
	if(NULL == dir)
	{
		status = mkdir(id, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH); 
		int fd;
		strcpy(fileid,id);
		strcat(fileid,"/DataFile");
		fd=creat(fileid,S_IRUSR | S_IRWXG | S_IROTH | S_IXOTH);
		write(fd,buf,1024);
		close(fd);
		memset(fileid,'\0',sizeof(fileid));
		strcpy(fileid,id);
		//	printf("%s\n",fileid);
		strcat(fileid,"/PesonRecycleBin");
		fd=creat(fileid,S_IRUSR | S_IRWXG | S_IROTH | S_IXOTH);
        write(fd,buf,1024);
        close(fd);
		memset(fileid,'\0',sizeof(fileid));
		strcpy(fileid,id);
		strcat(fileid,"/MessageRecycleBin");
		fd=creat(fileid,S_IRUSR | S_IRWXG | S_IROTH | S_IXOTH);
		write(fd,buf,1024);
		close(fd);
		DataInit(id);
	}
	else
	{
		DataBaseStart(id);
	}
	
	list_person(1);
	sock_power_on(longlong_to_string(local_phoneNumber),server_ip,server_port,local_port);
}
