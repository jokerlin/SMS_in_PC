#ifndef c_personoperation_h
#define c_personoperation_h
//此内容单元测试通过
#include"willzhang.h"

#define the_person_num_of_one_page 10

void list_person(int numofpage);
void add_person(struct person x);
void delete_person(long long id);
int exist_in_list(long long person_id);//0表示不存在1表示存在

#endif
