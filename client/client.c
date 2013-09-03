/*************************************************************************
    > File Name: client.h
    > Author: linheng
    > Mail: i@haroquee.me 
    > Created Time: Sat 31 Aug 2013 05:16:19 PM CST
 ************************************************************************/

#include "client.h"
#include "power_on.h"
#include "willzhang.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

char input_phonenumber_s[12];
char input_page_number_s[5];
char input_message_id_s[5];

void power_off(int serverfd, char* phone_num, char* server_ip, int server_port)
{
	sock_power_off(serverfd, phone_num, server_ip, server_port) //关机
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

long long legalinput(char a[])
{
	int len = strlen(a);
	long long digit = 0;
	for (int i = 0; i<len; i++)
	{
		if (isdigit(a[i])) digit = digit + (a[i]-48) * pow(10, len-i-1); 
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
	
	scanf("%s",&input_page_number_s);
	input_page_number = legalinput(input_page_number_s);
	if (input_page_number != -1) list_person(input_page_number);
	else printf("WRONG INPUT.\n");
	
	lockflag = 0;
}

void client_list_person_message()
{
	lockflag = 1;
	
	printf("Please Enter the Phone Number You Want to Read: ");
	scanf("%s",&input_phonenumber_s);
	input_phonenumber = legalinput(input_phonenumber_s);
	if (input_phonenumber !=  -1) 
	{
		page_number = person_message_pages_nums(input_phonenumber) - 1;
		printf("Choose Page to Read(0~%d):  ",page_number);
		scanf("%s",&input_page_number_s);
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

	printf("Please Enter the Phone Number You Want to Delete: ");
	scanf("%s",&input_phonenumber_s);
	input_phonenumber = legalinput(input_phonenumber_s);
	if (input_page_number != -1) delete_person(input_phonenumber);;
	else printf("WRONG INPUT.\n");

	lockflag = 0;
}
void client_delete_message()
{
	lockflag = 1;

	printf("Please Enter the Phone Number You Want to Delete: ");
	scanf("%s",&input_phonenumber_s);
	input_phonenumber = legalinput(input_phonenumber_s);
	if (input_phonenumber !=  -1) 
	{
		printf("Choose the Message ID: ");
		scanf("%s",&input_message_id_s);
		input_message_id = legalinput(input_message_id_s);
		if (input_page_number != -1) delete_message(input_phonenumber, input_message_id);
		else printf("WRONG INPUT.\n");
		delete_message(input_phonenumber, input_message_id);
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
	scanf("%s",&input_phonenumber_s);
	input_phonenumber = legalinput(input_phonenumber_s);
	if (input_phonenumber !=  -1) 
	{
		printf("Enter the Message Content:\n");
		int i;
		for (i = 0; i <  100; i++) input_message_content[i] = '\0';	
		scanf("%s", input_message_content);
		
		struct message msg;
		msg.receiver = input_phonenumber;
		msg.sender = local_phonenumber;
		strcpy(msg.content, input_message_content);
		msg.Time = time(NULL);
		msg.flag_lms = 0;
		msg.HasBeenReaded = 0;
		
		save_message(msg.receiver,msg);
		sock_sendmsg(struct message msg, server_ip, server_port) 
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
	char[70] input_search_content;
	char[10][70] input_search_content2;
	lockflag = 1;
	
	printf("(1)Single_Search; (2)Multi_Search. Press '1' or '2' to choose: ");
	scanf("%d", &search_way);
	if (search_way == 1) 
	{
		printf("Please Input the Key: ");
		for (i = 0; i <  70; i++) input_search_content = '\0';	
		scanf("%s", input_search_content);
		search_message_single(input_search_content);
	}
	else
	{
		(if search_way == 2) 
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
