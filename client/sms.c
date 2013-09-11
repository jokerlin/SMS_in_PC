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
	noecho();
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
/*******************************************************************************************************/
//新添加的全局变量
#define the_person_num_of_one_page 10
char addr_cur_name[11][25];
char addr_cur_phone[11][12];
long long addr_cur_phone_longlong[11];

#define the_person_num_of_one_page 10
#define List_POS_X 10
#define List_POS_Y 10
#define List_ADD_X 1
unsigned char person_list_content[10][300];
struct person * person_detail[10];
int current_pages;//当前的页数
int current_index;//当前页的哪一条信息

/*******************************************************************************************************/




/********************************************zwzwzwzwzwzwzwz********************************************/
int init_person_list_content(int numofpage)
{
     int counter =0;
     struct person *p=longlong_to_PersonPoint(PersonList->NextPerson);//开发者默认不输出
     for(int i=0;i<numofpage;i++)
     {
         for(int j=0;j<the_person_num_of_one_page;j++)
         {
             if(p==MemBasePerson) return;
             p=longlong_to_PersonPoint(p->NextPerson);
         }
     }
     for(int i=0;i<the_person_num_of_one_page;i++)
     {
         if(p==MemBasePerson) break;
 //DON'T EDIT BEFORE 
         person_detail[i]=p;
         char id_s[20];
         longlong_to_string(p->id,id_s);
         strcpy(person_list_content[i],p->name);
         strcat(person_list_content,"   ");
         strcat(person_list_content[i],id_s);
         strcat(person_list_content,"   ");
         if(p->HeadMessage)
         {
            strcat(person_list_content,longlong_to_MessagePoint(p->HeadMessage));
         }
         //mvaddstr(WELCOME_POS_X + 3 + i, 30, p->name);
         //char id_s[20];
         //longlong_to_string(p->id, id_s);
         //mvaddstr(WELCOME_POS_X + 3 + i, 50, id_s);
         counter++;
         //refresh();
 //DON'T EDIT BELOW
         p=longlong_to_PersonPoint(p->NextPerson);
     }
     //printf("%d",count);
     refresh();
     return counter;//当前页的联系人个数
}
void Reverse_print()
{
    attron(A_REVERSE);
    mvaddstr(List_POS_X+List_ADD_X*current_index,List_POS_Y,person_list_content[current_index]);
    attroff(A_REVERSE);
}
void Normal_print()
{
    mvaddstr(List_POS_X+List_ADD_X*current_index,List_POS_Y,person_list_content[current_index]);
}
/***************细节************************/
unsigned char message_content[10][250];
int message_num;//当前页的信息的个数
int message_page;//信息的当前页数
int init_list_person_message(long long person_id,int NumOfPage)
{
    struct person *p=longlong_to_PersonPoint(PersonList->NextPerson);
    while(p->id!=person_id)
    {
        p=longlong_to_PersonPoint(p->NextPerson);
    }
    struct message *q=longlong_to_MessagePoint(p->HeadMessage);
    for(int i=0;i<NumOfPage;i++)
    {
        for(int j=0;j<10;j++)
        {
            if(q==MemBaseMessage) return;
            q=longlong_to_MessagePoint(q->NextMessage);
        }
    }
    int counter=0;
    for(int i=0;i<10;i++)
    {
        if(q==MemBaseMessage) return;
        /*************/
        strcpy(message_content,q->content);
        //printf("%d: %s\n",NumOfPage*the_message_num_of_one_page+i,q->content);
        /*************/
        q=longlong_to_MessagePoint(q->NextMessage);
    }
    return counter;
}
void unit_message_box_PersonDetail(struct person *Person)
{
    clear();
    print_border();
    message_page=0;
    message_num=init_list_person_message(Person->id,message_page);
    for(int i=0;i<message_num;i++)
    {
        mvaddstr(List_POS_X+List_ADD_X*i,List_POS_Y,message_content[i]);
    }
    refresh();
    int instruction;
    while(instruction=getch())
    {
        if(instruction == KEY_UP)
        {
            message_page--;
            message_num=init_list_person_message(Person->id,message_page);
            for(int i=0;i<message_num;i++)
            {
                mvaddstr(List_POS_X+List_ADD_X*i,List_POS_Y,message_content[i]);
            }
            refresh();
        }
        else if(instruction == KEY_DOWN)
        { 
            message_page++;
            message_num=init_list_person_message(Person->id,message_page);
            for(int i=0;i<message_num;i++)
            {
                mvaddstr(List_POS_X+List_ADD_X*i,List_POS_Y,message_content[i]);
            }
            refresh();
        }
        else if(instruction == 'q')
        {
            return;
        }
        else
        {
            continue;
        }
    }
}
/**************************************/
void unit_message_box()
{
    clear();
    print_border();
    current_pages=0;
    int person_num=init_person_list_content(current_pages);
    for(int i=0;i<person_num;i++)
    {
        mvaddstr(List_POS_X+List_ADD_X*i,List_POS_Y,person_list_content[i]);
    }
    current_index=0;
    Reverse_print();
    refresh();
    int instruction;
    while(instruction=getch())
    {
        if(instruction == KEY_UP)
        {
            if(current_index == 0)  continue;
            Normal_print();
            current_index--;
            Reverse_print();
        }
        else if(instruction == KEY_DOWN)
        {
            if(current_index == person_num - 1) continue;
            Normal_print();
            current_index++;
            Reverse_print();
        }
        else if(instruction == 13)
        {
            unit_message_box_PersonDetail(person_detail[current_index]);
        }
        else
        {
            continue;
        }
    }
}

/********************************************zwzwzwzwzwzwzwz********************************************/
int client_list_person_CUR(int numofpage)
{
    int counter = 0;
    struct person *p=longlong_to_PersonPoint(PersonList->NextPerson);//开发者默认不输出
    for(int i=0;i<numofpage;i++)
    {
        for(int j=0;j<the_person_num_of_one_page;j++)
        {
            if(p==MemBasePerson) return;
            p=longlong_to_PersonPoint(p->NextPerson);
        }
    }
    for(int i=0;i<the_person_num_of_one_page;i++)
    {
        if(p==MemBasePerson) break;
//DON'T EDIT BEFORE 
        //printf("%s %lld\n",p->name,p->id);
        counter++;
        strcpy(addr_cur_name[i], p->name);
        char id_s[20];
        addr_cur_phone_longlong[i] = p->id;
        longlong_to_string(p->id, id_s);
        strcpy(addr_cur_phone[i], id_s);
        //mvaddstr(WELCOME_POS_X + 3 + i, 25, p->name);
        //char id_s[20];
        //longlong_to_string(p->id, id_s);
        //mvaddstr(WELCOME_POS_X + 3 + i, 25, id_s);
        //refresh();
//DON'T EDIT BELOW
        p=longlong_to_PersonPoint(p->NextPerson);
    }
    return counter;
}

void print_addr_cur(int num, int page)
{
    client_list_person_CUR(page);
    mvaddstr(WELCOME_POS_X + 3 + num, 25, addr_cur_name[num]);
    mvaddstr(WELCOME_POS_X + 3 + num, 50, addr_cur_phone[num]);
}

void unit_Address_Book()
{
    int index_addr_cur = 0;
    int page_number_cur = 0;

    clear();
    print_border();
    page_number = person_pages_nums() - 1;
    list_person(page_number_cur);
    refresh();
    attron(A_REVERSE);
    print_addr_cur(index_addr_cur, page_number_cur);
    refresh();
    attroff(A_REVERSE);
    
    
    while (1)
    {
        int ch = getch();
                //mvaddstr(10,10,"hello");
                //refresh();
                switch (ch)
                {
                    case 'n': 
                        if (page_number_cur == page_number) break;
                        clear();
                        print_border();
                        refresh();
                        page_number_cur++;
                        list_person(page_number_cur);
                        refresh();
                        break;
                    case 'p':
                        if (page_number_cur == 0) break;
                        clear();
                        print_border();
                        refresh();
                        page_number_cur--;
                        list_person(page_number_cur);
                        refresh();
                        break;
                    case KEY_UP: 
                        if (index_addr_cur == 0) break;
                        attroff(A_REVERSE);
                        print_addr_cur(index_addr_cur, page_number_cur);
                        index_addr_cur--;
                        attron(A_REVERSE);
                        print_addr_cur(index_addr_cur, page_number_cur);
                        refresh();
                        break;
                    case KEY_DOWN:
                        if (index_addr_cur >= client_list_person_CUR(page_number_cur) - 1 ) break;
                        attroff(A_REVERSE);
                        print_addr_cur(index_addr_cur, page_number_cur);
                        index_addr_cur++;
                        attron(A_REVERSE);
                        print_addr_cur(index_addr_cur,page_number_cur);
                        refresh();
                        break;
                    case 13:
                        clear();
                        attroff(A_REVERSE);
                        print_border();
                        mvaddstr(WELCOME_POS_X + 3, WELCOME_POS_Y - 5 , "请输入你要修改的姓名：    ");
                        refresh();
                        char input_name[25];
                        echo();
                        scanw("%s", input_name);
                        rename_person(addr_cur_phone_longlong[index_addr_cur], input_name);
                        mvaddstr(WELCOME_POS_X + 5, WELCOME_POS_Y - 4 , "修改成功!");
                        refresh();
                        noecho();
                        clear();
                        print_border();
                        list_person(page_number_cur);
                        attron(A_REVERSE);
                        print_addr_cur(index_addr_cur, page_number_cur);
                        refresh();
                        attroff(A_REVERSE);
                        break;
                    case 127:
                        attroff(A_REVERSE);
                        return ;
                        break;
                    default:  break;
                }
    }
    refresh();

}

void unit_Send_Msg()
{
    clear();
    print_border();
    refresh();
	echo();
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
	mvaddstr(WELCOME_POS_X + 8, WELCOME_POS_Y - 6 , "是否发送定时短信（y/N）：");
	char delayflag[4];
    struct message msg;
	scanw("%s", delayflag);
    if(delayflag[0] == 'y' || delayflag[0] == 'Y')
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
    else
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
    refresh();
    noecho();
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
                        if (index_INDEX_CONTENT == 0)
                        {
                            attroff(A_REVERSE);
                            unit_message_box();
                        }
                        if (index_INDEX_CONTENT == 1)
                        {
                            attroff(A_REVERSE);
                            unit_Send_Msg();
                            clear();
                            print_border();
                            welcome();
                        }
                        if (index_INDEX_CONTENT == 2) 
                        {
                            attroff(A_REVERSE);
                            unit_Address_Book();
                            clear();
                            print_border();
                            welcome();
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
