#ifndef c_search_h
#define c_search_h

#include"willzhang.h"

void search_message(unsigned char a[][250],int m);//设定每个关键字不超过70个字节
void search_message_single(unsigned char a[]);
int kmp_find(unsigned char *T,unsigned char *P);

#endif
