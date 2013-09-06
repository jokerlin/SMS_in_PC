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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#include "power_on.h"
#include "kbhit.h"
#include "client.h"
#include "string_to_message.h"

#define DEBUG 1 


int main(int argc, char** argv)
{
    int normal_power_flag = 0;//正常关机标志
    int pipe_fd[2];
    int pipe_fd2[2];
	char instruction;
    pid_t pid;
    char buf_r[1024];

    welcome();
    int sockfd = power_on();

    if (pipe(pipe_fd) < 0)
    {
        printf("pipe create error\n");
        return -1;
    }
	if (pipe(pipe_fd2) < 0)
	{
		printf("pipe create error\n");
		return -1;
	}
    int maxfd = pipe_fd[0] > pipe_fd[1] ? pipe_fd[0] + 1 : pipe_fd[1] + 1;
    struct timeval timeout = {1, 0};
    fd_set rdfds;
    if ((pid = fork())  > 0)
    {
        while (!normal_power_flag)
        {
			//printf("啊哈，又一次循环\n");
            FD_ZERO(&rdfds);
            FD_SET(pipe_fd[0], &rdfds);

            int ret= select(maxfd, &rdfds, NULL, NULL, &timeout);
            //printf ("before reading %d\n", ret);
            if (ret < 0) perror("select");/* 这说明select函数出错 */
            else if (ret > 0)
            {
                memset(buf_r,0, sizeof(buf_r));
				read(pipe_fd[0],buf_r,1024);
                //printf("Fuck the pipesending: %s\n",buf_r);//for debug
				write(pipe_fd2[1], "OK", 2);
				if(DEBUG)
				{
					printf("Send OK success\n");
				}
                printf("comlete pipesending: %s\n",buf_r);//for debug
                struct message msg_receive = string_to_message(buf_r);
                printf("complete string to message\n");

                if (!exist_in_list(msg_receive.sender))
                {
                    struct person personadd;
                    personadd.HeadMessage = 0;
                    personadd.id = msg_receive.sender;
                    strcpy(personadd.name,"sb");
                    personadd.NextPerson = 0;
                    personadd.NumOfMessage = 0;
                    personadd.Time = 0;
                    add_person(personadd);
                }
				printf("before save msg\n");
                save_message(msg_receive.sender,msg_receive);
                printf("You Just Get a New Message!\n");
            }
            /*
            if ((r_num = read(pipe_fd[0],buf_r,100)) > 0)
            {
            	printf("\nYou Just Get a New Message!\n");
            	printf("%s\n",buf_r);
            	struct message msg_receive = string_to_message(buf_r);
            	save_message(msg_receive.receiver,msg_receive);
            	r_num = 0;
            }
            */
            //help();
			//close(pipe_fd[1]);
			//close(pipe_fd2[0]);
            //close(pipe_fd[0]);
            while (!kbhit()) nothing();
            instruction = getchar();
            switch (instruction)
            {
            case 'q':
                normal_power_flag = 1;
                char local_phoneNumber_s [12];
                memset(local_phoneNumber_s, 0, sizeof(local_phoneNumber_s));
                longlong_to_string(local_phoneNumber, local_phoneNumber_s);
                power_off(sockfd,local_phoneNumber_s, server_ip, server_port);
                break;
            case 'h':
                help();
                break;
            case 'l':
                client_list_person();
                break;
            case 'm':
                client_list_person_message();
                break;
            case 'r':
                client_delete_person();
                break;
            case 'd':
                client_delete_message();
                break;
            case 's':
                send_message();
                break;
            case 'f':
                client_search_message();
                break;
            case 10:
                printf("Refreshed.\n");
                break;
            default:
                printf("WRONG INSTRUCTION. Please press 'h' for help.\n");
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
            //printf("Start accept\n");
            if((childSockfd = accept(sockfd, (struct sockaddr*) &addr, &addrlen)) < 0)
            {
                perror("accept");
                //printf("%d\n",sockfd);
                return -1;
            }
            printf("Power On Successfully!\n");
            char buf[1024];
            memset(buf, 0, sizeof(buf));
            int len = read(childSockfd, buf, 1024);
            if(DEBUG)
            {
                printf("get message: %s", buf);
            }
            char local_phoneNumber_s[20];
            longlong_to_string(local_phoneNumber,local_phoneNumber_s);
            strcat(local_phoneNumber_s, "\n");
            write(childSockfd, local_phoneNumber_s, 20);
            if(DEBUG)
            {
                printf("My Phone Number: %s\n", local_phoneNumber_s);
            }
            close(childSockfd);
            close(pipe_fd[0]);
            //while (lockflag) sleep(1000);
            int pipedebug = write(pipe_fd[1], buf, 1024);
			char buf_back[3];
			memset(buf, 0, sizeof(buf));
			memset(buf_back, 0, sizeof(buf_back));
			printf ("%d\n",pipedebug);
			//close(pipe_fd2[1]);
			read(pipe_fd2[0], buf_back, 3);
			if (buf_back[0] != 'O' || buf_back[1]!='K') {
				perror("get msg from parent fail\n");			
			}
            if (pipedebug < 0)
            {
                perror("write");
                return -1;
            }
            //close(pipe_fd[1]);
        }
    }
}
