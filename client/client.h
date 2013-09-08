/*************************************************************************
    > File Name: client.h
    > Author: linheng
    > Mail: i@haroquee.me 
    > Created Time: Sat 31 Aug 2013 05:16:19 PM CST
 ************************************************************************/
#ifndef c_client_h
#define c_client_h

int lockflag; //进程锁标记
//long long phonenumber;
long long input_phonenumber;
int page_number;
int input_page_number;
int input_message_id;
char input_message_content[250];

void power_off(); //关机 
void help(); //显示帮助
void client_list_person(); //显示联系列表 
void client_list_person_message(); //显示联系人短信
void client_delete_person(); //删除联系人所有短信
void client_delete_message(); //删除某个联系人的某个短信
void send_message(); //发送短信
void client_search_message(); //搜索关键字
void client_rename_person(); //对通讯录进行操作

#endif
