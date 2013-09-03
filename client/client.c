/*************************************************************************
    > File Name: client.h
    > Author: linheng
    > Mail: i@haroquee.me 
    > Created Time: Sat 31 Aug 2013 05:16:19 PM CST
 ************************************************************************/

#include "client.h"
#include"PersonOperation.h"
#include"message.h"
#include<stdio.h>
#include<string.h>

int lockflag = 0; //进程锁标记
//long long phonenumber;
long long input_phonenumber;
int page_number;
int input_page_number;
int input_message_id;
char input_message_content[100];

void power_off()
{
	//socket_power_off(); //结束监听
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

void client_list_person()
{
	lockflag = 1;
	
	page_number = person_pages_nums() - 1;
	printf("Choose Page to Read(0~%d):  ",page_number);
	scanf("%d",&input_page_number);
	
	list_person(input_page_number);
	
	lockflag = 0;
}

void client_list_person_message()
{
	lockflag = 1;
	
	printf("Please Enter the Phone Number You Want to Read: ");
	scanf("%lld",&input_phonenumber);
	page_number = person_message_pages_nums(input_phonenumber) - 1;
	printf("Choose Page to Read(0~%d):  ",page_number);
	scanf("%d",&input_page_number);
	
	list_person_message(input_phonenumber, input_page_number);

	lockflag = 0;
}
void client_delete_person()
{
	lockflag = 1;

	printf("Please Enter the Phone Number You Want to Delete: ");
	scanf("%lld",&input_phonenumber);

	delete_person(&input_phonenumber);

	lockflag = 0;
}
void client_delete_message()
{
	lockflag = 1;

	printf("Please Enter the Phone Number You Want to Delete: ");
	scanf("%lld",&input_phonenumber);
	printf("Choose the Message ID: ");
	scanf("%d",&input_message_id);

	delete_message(input_phonenumber, input_message_id);

	lockflag = 0;
}
void send_message()
{
	lockflag = 1;

	printf("Please Enter the Phone Number You Want to Send to: ");
	scanf("%lld",&input_phonenumber);
	printf("Enter the Message Content:\n");
	int i;
	for (i = 0; i <  100; i++) input_message_content[i] = '\0';	
	scanf("%s", input_message_content);
	//sock_send_message(); // wait the implement

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
