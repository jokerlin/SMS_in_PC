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
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <curses.h>
#include <locale.h>
#include <time.h>
#include <math.h>

#include "power_on.h"
#include "kbhit.h"
#include "client.h"
#include "string_to_message.h"

#define DEBUG 0

void initial()
{
	initscr();
	cbreak();
	nonl();
	echo();
	intrflush(stdscr, FALSE);
	keypad(stdscr,TRUE);
	refresh();
}

void print_border()
{
    setlocale(LC_ALL,"");
    initial();
    box(stdscr, ACS_VLINE, ACS_HLINE);
    mvaddstr(WELCOME_POS_X, WELCOME_POS_Y,"Welcome to use SMS_in_PC");
    mvaddstr(WELCOME_POS_X + 1, WELCOME_POS_Y - 5, "Created by My Nine Partners and Me");
    refresh();
}

void unit_Address_Book()
{
    clear();
    print_border();
    list_person(0);
    refresh();
}

void unit_Send_Msg()
{
    clear();
    print_border();
    refresh();
	
	char receiver_s[12];
	mvaddstr(WELCOME_POS_X + 3, WELCOME_POS_Y - 5 , "收件人：");
	refresh();
	scanw("%s", receiver_s);
	long long receiver_num = string_to_longlong(receiver_s, strlen(receiver_s) - 1 + receiver_s);
	refresh();
	mvaddstr(WELCOME_POS_X + 5, WELCOME_POS_Y - 4 , "内容：");
	refresh();
	unsigned char input_content[250];
	getstr(input_content);
	mvaddstr(WELCOME_POS_X + 8, WELCOME_POS_Y - 6 , "是否发送定时短信（y/n）：");
	char delayflag[4];
    struct message msg;
	scanw("%s", delayflag);
    if(delayflag[0] == 'y')
    {
        char DelayTime_s[30];
        mvaddstr(WELCOME_POS_X + 9, WELCOME_POS_Y - 6 ,"请按格式输入时间 (1970-1-1-08:00:00):");
        scanw("%s",DelayTime_s);
        struct tm DelayTime;
        strptime(DelayTime_s,"%F-%T",&DelayTime);
        //printf("%d\n",DelayTime.tm_year,DelayTime.tm_ho
        msg.receiver = input_phonenumber;
        msg.sender = local_phoneNumber;
        strcpy(msg.content, input_content);
        //printf("Delay:%ld\n",DelayTime);
        //printf("%ld\n",time(NULL));
        msg.Time = mktime(&DelayTime)-time(NULL);
        msg.flag_lms = 0;
        msg.HasBeenReaded = 0;
    }
    else if(delayflag[0] == 'n')
    {
        msg.receiver = receiver_num;
        msg.sender = local_phoneNumber;
        strcpy(msg.content, input_content);
        msg.Time = 0;
        msg.flag_lms = 0;
        msg.HasBeenReaded = 0;
    }
	refresh();
	if (!exist_in_list(msg.receiver))
        {
            struct person personadd;
            personadd.HeadMessage = 0;
            personadd.id = msg.receiver;
            strcpy(personadd.name,"UNKNOWN");
            personadd.NextPerson = 0;
            personadd.NumOfMessage = 0;
            personadd.Time = 0;
            add_person(personadd);
        }
    save_message(msg.receiver,msg);
    sock_sendmsg(msg, server_ip, server_port) ;
    mvaddstr(WELCOME_POS_X + 10, WELCOME_POS_Y - 6 , "发送短信成功");
}

int main(int argc, char** argv)
{
    int normal_power_flag = 0;//正常关机标志
    int pipe_fd[2];
    int pipe_fd2[2];
	char instruction;
    pid_t pid;
    char buf_r[1024];

    int sockfd = power_on();
    
	if (sockfd < 0) 
	{
		printf ("开机失败！\n");
		return -1;
	}

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
    
    setlocale(LC_ALL,"");
    initial();
    box(stdscr, ACS_VLINE, ACS_HLINE);
    refresh();
    welcome();
	//printf("开机成功！\n");

    int maxfd = pipe_fd[0] > pipe_fd[1] ? pipe_fd[0] + 1 : pipe_fd[1] + 1;
    struct timeval timeout = {1, 0};
    fd_set rdfds;
    if ((pid = fork())  > 0)
    {
        while (!normal_power_flag)
        {
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
                //printf("comlete pipesending: %s\n",buf_r);//for debug
                struct message msg_receive = string_to_message(buf_r);
                //printf("complete string to message\n");

                if (!exist_in_list(msg_receive.sender))
                {
                    struct person personadd;
                    personadd.HeadMessage = 0;
                    personadd.id = msg_receive.sender;
                    strcpy(personadd.name,"UNKNOWN");
                    personadd.NextPerson = 0;
                    personadd.NumOfMessage = 0;
                    personadd.Time = 0;
                    add_person(personadd);
                }
				//printf("before save msg\n");
                save_message(msg_receive.sender,msg_receive);
                printf("您收到了一条来自 %lld 的新消息:\n",msg_receive.sender);
				printf("%s\n",msg_receive.content);
            }
            
            while(1)
            {
                int ch = getch();
                //mvaddstr(10,10,"hello");
                //refresh();
                switch (ch)
                {
                    case KEY_UP: 
                        if (index_INDEX_CONTENT == 0) break;
                        attroff(A_REVERSE);
                        print_content();
                        index_INDEX_CONTENT--;
                        attron(A_REVERSE);
                        print_content();
                        refresh();
                        break;
                    case KEY_DOWN:
                        if (index_INDEX_CONTENT == 2) break;
                        attroff(A_REVERSE);
                        print_content();
                        index_INDEX_CONTENT++;
                        attron(A_REVERSE);
                        print_content();
                        refresh();
                        break;
                    case 13:
                        if (index_INDEX_CONTENT == 1)
                        {
                            attroff(A_REVERSE);
                            unit_Send_Msg();
                        }
                        if (index_INDEX_CONTENT == 2) 
                        {
                            attroff(A_REVERSE);
                            unit_Address_Book();
                        }
                        break;
                    default: 
                        break;
                }
            }
            /*
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
				kill(pid, 15);
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
			case 'e':
				client_rename_person();
            case 10:
                printf("已刷新.\n");
                break;
            default:
                printf("错误的命令。请按 h 获取帮助。\n");
            }
            */
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
            //printf("Power On Successfully!\n");
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
			//printf ("%d\n",pipedebug);
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
