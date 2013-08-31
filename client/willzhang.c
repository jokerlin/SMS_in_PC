#ifndef c_willzhang_c
#define c_willzhang_c

#include"willzhang.h"
#include"PersonOperation.h"
#include"message.h"
#include"search.h"

#include<sys/mman.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>

void DataBaseStart()
{
    int fd=open("DataFile",O_RDWR);
    struct stat DataFileState;
    fstat(fd,&DataFileState);
    MemPoint=mmap(NULL+0xb7d20008,DataFileState.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    DataHead=(long long *)MemPoint;
    PersonList=((struct person *)(MemPoint+sizeof(long long)));
    close(fd);
    MemPoint+=DataHead[0];
}
void DataInit()
{
    int fd=open("DataFile",O_RDWR);
    struct stat DataFileState;
    fstat(fd,&DataFileState);
    void *TmpMemPoint=mmap(NULL+0xb7d20008,DataFileState.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);
    ((long long *)TmpMemPoint)[0]=sizeof(long long)+sizeof(struct person);
    ((struct person *)(TmpMemPoint+sizeof(long long)))->HeadMessage=NULL;
    ((struct person *)(TmpMemPoint+sizeof(long long)))->id=11111111111;
    ((struct person *)(TmpMemPoint+sizeof(long long)))->name[0]='w';
    ((struct person *)(TmpMemPoint+sizeof(long long)))->name[1]='z';
    ((struct person *)(TmpMemPoint+sizeof(long long)))->name[2]='\0';
    ((struct person *)(TmpMemPoint+sizeof(long long)))->Time=0;
    ((struct person *)(TmpMemPoint+sizeof(long long)))->NextPerson=NULL;
}
int main()
{
    //DataInit();
    DataBaseStart();
    /*增加一个联系人
    struct person tmp;
    tmp.HeadMessage=NULL;
    tmp.id=12;
    scanf("%s",tmp.name);
    tmp.Time=0;
    add_person(tmp);
    //*/
    /*给该联系人增加5个信息
    struct message tmp;
    for(int i=0;i<12;i++)
    {
        scanf("%s",tmp.content);
        tmp.Time=10+i;
        save_message(12,tmp);
    }
    //*/
    /*
    list_person_message(12,0);
    list_person_message(12,1);
    printf("\n");
    delete_message(12,3);
    list_person_message(12,0);
    list_person_message(12,1);
    */
    ///*
    list_person_message(12,0);list_person_message(12,1);
    unsigned char a[70][70];
    scanf("%s",a[0]);
    scanf("%s",a[1]);
    search_message(a,2);
    //*/
    //list_person(0);
    return 0;
}

#endif
