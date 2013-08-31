#ifndef c_message_h
#define c_message_h
//已通过测试
#include"willzhang.h"

#define the_message_num_of_one_page 10

void list_person_message(long long person_id,int NumOfPage);
void delete_message(long long person_id,int message_id);//message_id从0开始编号
void save_message(long long person_id,struct message x);

int person_message_pages_nums(long long person_id);
#endif
