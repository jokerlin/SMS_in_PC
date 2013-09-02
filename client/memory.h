#ifndef c_memory_h
#define c_memory_h
//已通过测试
#include"willzhang.h"

struct person * new_person();
struct message * new_message();
void Recycle_Person(long long addr);
void Recycle_Message(long long addr);

#endif
