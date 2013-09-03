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

#include "power_on.h"
#include "kbhit.h"
#include "client.h"

int normal_power_flag = 0;//正常关机标志
char instruction;
pid_t pid;

int main(int argc, char** argv)
{
	welcome();
	power_on();
	if (pid > 0)
	{
		while (!normal_power_flag)
		{
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
		receive_message();
	}
}
