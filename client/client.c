/*************************************************************************
    > File Name: client.h
    > Author: linheng
    > Mail: i@haroquee.me
    > Created Time: Sat 31 Aug 2013 05:16:19 PM CST
 ************************************************************************/

#include "client.h"
#include "power_on.h"
#include "willzhang.h"
#include "sandtears.h"
#include "message.h"
#include "PersonOperation.h"


#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <math.h>

char input_phonenumber_s[12];
char input_page_number_s[5];
char input_message_id_s[5];

void power_off(int serverfd, char* phone_num, char* server_ip, int server_port)
{
	sock_power_off(serverfd, phone_num, server_ip, server_port) ;//关机
	printf("\n/#########################################################################\n\n");
	printf("	> Shutdown Sucessfully.\n\n");
	printf("	> Bye Bye.\n\n");
	printf("	> Thanks to use SMS_in_PC\n\n");
	printf("	> Created by My Nine Partners and Me\n\n");
    printf("#########################################################################/\n\n");
}

void help()
{
	printf("\n\n#########################################################################\n\n");

	printf("	> HANDBOOK of SMS_in_PC\n\n");

	printf("	> Created by My Nine Partners and Me\n\n");

	printf("	> Edited on 2013.08.31\n\n");

	printf("#########################################################################\n\n");

	printf("指令：\n\n");

	printf("l	>	列出与您联系的联系人，\n");
	printf("		紧接着您需要输入联系人列表的页数，\n");
	printf("		数字越小代表越是最近与您联系的联系人。\n\n");

	printf("m	>	列出某位联系人与您之间发送的短信，\n");
	printf("		紧接着您需要输入该联系人的手机号，以及短信列表的页数，\n");
	printf("		数字越小代表越是最近该联系人与您之间发送的短信。\n\n");

	printf("r	>	删除您与某位联系人联系的所有短信，\n");
	printf("		紧接着您需要输入该联系人的手机号。\n\n");

	printf("d	>	删除您与某位联系人的某条短信，\n");
	printf("		紧接着您需要输入该联系人的手机号，\n");
	printf("		以及与该联系人之间发送的短信的message_id。\n\n");

	printf("s	>	给某位联系人发送短信，\n");
	printf("		紧接着您需要输入该联系人的手机号，\n");
	printf("		以及键入你想要发送的短信内容。\n\n");

	printf("f	>	根据关键字搜索所有的短信，\n");
	printf("		紧接着您需要输入您所输入的关键字。\n\n");

	printf("h	>	显示帮助\n\n");

	printf("q	>	退出该系统。\n\n");
}

int lh_pow(int a, int b) {
	int ans = a;
	if(b == 0)
		return 1;
	for(int i = 1; i < b; i++) {
		ans = ans * a;
	}
	return ans;
}

long long legalinput(char a[])
{
	int len = strlen(a);
	long long digit = 0;
	for (int i = 0; i<len; i++)
	{
		if (isdigit(a[i])) digit = digit + (a[i]-48) * lh_pow(10, 1*len-i-1);
		else
		{
			return -1;
		}
	}
	return digit;
}


void client_list_person()
{
	lockflag = 1;

	page_number = person_pages_nums() - 1;
	printf("Choose Page to Read(0~%d):  ",page_number);

	scanf("%s",input_page_number_s);
	input_page_number = legalinput(input_page_number_s);
	if (input_page_number != -1) list_person(input_page_number);
	else printf("WRONG INPUT.\n");

	lockflag = 0;
}

void client_list_person_message()
{
	lockflag = 1;

	printf("Please Enter the Phone Number You Want to Read: ");
	scanf("%s",input_phonenumber_s);
	input_phonenumber = legalinput(input_phonenumber_s);
	if (input_phonenumber !=  -1)
	{
		page_number = person_message_pages_nums(input_phonenumber) - 1;
		printf("Choose Page to Read(0~%d):  ",page_number);
		scanf("%s",input_page_number_s);
		input_page_number = legalinput(input_page_number_s);
		if (input_page_number != -1) list_person_message(input_phonenumber, input_page_number);
		else printf("WRONG INPUT.\n");
	}
	else
	{
		printf("WRONG INPUT.\n");
	}

	lockflag = 0;
}
void client_delete_person()
{
	lockflag = 1;

	list_person(0);
	printf("Please Enter the Phone Number You Want to Delete: ");
	scanf("%s",input_phonenumber_s);
	input_phonenumber = legalinput(input_phonenumber_s);
	if (input_page_number != -1) delete_person(input_phonenumber);
	else printf("WRONG INPUT.\n");

	lockflag = 0;
}
void client_delete_message()
{
	lockflag = 1;

	list_person(0);
	printf("Please Enter the Phone Number You Want to Delete: ");
	scanf("%s",input_phonenumber_s);
	input_phonenumber = legalinput(input_phonenumber_s);
	if (input_phonenumber !=  -1)
	{
		printf("Choose the Message ID: ");
		scanf("%s",input_message_id_s);
		input_message_id = legalinput(input_message_id_s);
		if (input_message_id != -1) delete_message(input_phonenumber, input_message_id);
		else printf("WRONG INPUT.\n");
		//delete_message(input_phonenumber, input_message_id);

	}
	else
	{
		printf("WRONG INPUT.\n");
	}
	lockflag = 0;
}
void send_message()
{
	lockflag = 1;

	printf("Please Enter the Phone Number You Want to Send to: ");
	scanf("%s",input_phonenumber_s);
	input_phonenumber = legalinput(input_phonenumber_s);
	if (input_phonenumber !=  -1)
	{
		printf("Enter the Message Content:\n");
		int i;
		for (i = 0; i <  100; i++) input_message_content[i] = '\0';
		scanf("%s", input_message_content);
		
		printf("Whether to delay sending?(y/n)\n");
		char delayflag[5];
		struct message msg;
		while(scanf("%s",delayflag))
		{
			if(delayflag[0] == 'y')
			{
				char DelayTime_s[30];
				printf("Please Input the Time:\n");
				printf("For Example:1970-1-1-08:00:00\n");
				scanf("%s",DelayTime_s);
				struct tm DelayTime;
				strptime(DelayTime_s,"%F-%T",&DelayTime);
				//printf("%d\n",DelayTime.tm_year,DelayTime.tm_hour);

				msg.receiver = input_phonenumber;
				msg.sender = local_phoneNumber;
				strcpy(msg.content, input_message_content);
				printf("Delay:%ld\n",DelayTime);
				printf("%ld\n",time(NULL));
				msg.Time = mktime(&DelayTime)-time(NULL);
				msg.flag_lms = 0;
				msg.HasBeenReaded = 0;
				break;
			}
			else if(delayflag[0] == 'n')
			{
				msg.receiver = input_phonenumber;
				msg.sender = local_phoneNumber;
				strcpy(msg.content, input_message_content);
				msg.Time = 0;
				msg.flag_lms = 0;
				msg.HasBeenReaded = 0;
				break;
			}
			else
			{
				printf("Please input 'y' or 'n'");
			}
		}

        if (!exist_in_list(msg.receiver))
        {
            struct person personadd;
            personadd.HeadMessage = 0;
            personadd.id = msg.receiver;
            strcpy(personadd.name,"sb");
            personadd.NextPerson = 0;
            personadd.NumOfMessage = 0;
            personadd.Time = 0;
            add_person(personadd);
        }
        save_message(msg.receiver,msg);
        printf("complete saving\n");//for debug
		sock_sendmsg(msg, server_ip, server_port) ;
		printf("complete sending\n");//for debug
	}
	else
	{
		printf("WRONG INPUT.\n");
	}


	lockflag = 0;
}
void client_search_message()
{
	int search_way;
	int i;
	int keys_num;
	char input_search_content[250];
	char input_search_content2[10][250];
	lockflag = 1;

	printf("(1)Single_Search; (2)Multi_Search. Press '1' or '2' to choose: ");
	scanf("%d", &search_way);
	if (search_way == 1)
	{
		printf("Please Input the Key: ");
		for (i = 0; i <  70; i++) input_search_content[i] = '\0';
		scanf("%s", input_search_content);
		search_message_single(input_search_content);
	}
	else
	{
		if (search_way == 2)
		{
			printf("Please Enter the Number of Keys: ");
			scanf("%d", &keys_num);
			for (i = 0; i< keys_num; i++)
			{
				scanf("%s", input_search_content);
				strcpy(input_search_content2[i],input_search_content);
			}
			search_message(*input_search_content2, keys_num);
		}
		else printf("WRONG INSTRUCTION.\n");
	}

	lockflag = 0;
}
