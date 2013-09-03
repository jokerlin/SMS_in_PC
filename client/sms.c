/*************************************************************************
    > File Name: sms.c
    > Author: linheng
    > Mail: i@haroquee.me 
    > Created Time: Fri 30 Aug 2013 09:26:23 PM CST
 ************************************************************************/

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <sys/types.h>

#include "power_on.h"
#include "kbhit.h"
#include "client.h"

int normal_power_flag = 0;//正常关机标志
int pipe_fd[2];
char instruction;
pid_t pid;
char buf_r[1024];
int r_num = 0;

int main(int argc, char** argv)
{
	welcome();
	int sockfd = power_on();
	if (pipe(pipe_fd) < 0)
	{
	    printf("pipe creat error\n");
	    return -1;
	}
	
	if (pid = fork()  > 0)
	{
		while (!normal_power_flag)
		{
			close(pipe_fd[1]);
			if ((r_num = read(pipe_fd[0],buf_r,100)) > 0)
			{
				printf("\nYou Just Get a New Message!\n");
				printf("%s\n",buf_r);
				struct msg_receive = string_to_message(buf_r);
				save_message(msg_receive.receiver,msg_receive);
				r_num = 0;
			}
			close(pipe_fd[0]);
			while (!kbhit()) nothing();
			instruction = getchar();
			switch (instruction)
			{
				case 'q': normal_power_flag = 1;power_off(); break;
				case 'h': help(); break;
				case 'l': client_list_person(); break;
				case 'm': client_list_person_message(); break;
				case 'r': client_delete_person(); break;
				case 'd': client_delete_message(); break;
				case 's': send_message(); break;
				case 'f': client_search_message(); break;
				case 10: printf("NO INSTRUCTION. Please press 'h' for help.\n");break;
				default: printf("WRONG INSTRUCTION. Please press 'h' for help.\n");
			}
		}
	}
	else

	{
		while(1) 
		{
			struct sockaddr_in addr;
			int addrlen = sizeof(addr);
			int childSockfd;
			if((childSockfd = accept(sockfd, ()struct sockaddr*) &addr, &addrlen) < 0) 
			{
				perror("accept");
				return -1;
			}
			char buf[1024];
			memset(buf, 0, sizeof(buf));
			int len = read(childSockfd, buf, 1024);
			close(pipe_fd[0]);
			while (lockflag) sleep(1000);
			write(pipe_fd[1], buf, 1);
			close(pipe_fd[1]);	
		}
	}
}
