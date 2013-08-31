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

struct person * longlong_to_PersonPoint(long long x)
{
    return (struct person *)(x+MemBase);
}

struct message * longlong_to_MessagePoint(long long x)
{
    return (struct message *)(x+MemBase);
}

void DataBaseStart()
{
    int fd=open("DataFile",O_RDWR);
    struct stat DataFileState;
    fstat(fd,&DataFileState);
    MemPoint=mmap(NULL,DataFileState.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    MemBase=MemPoint;
    MemBaseMessage=(struct message *)MemBase;
    MemBasePerson=(struct person *)MemBase;
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
    void *TmpMemPoint=mmap(NULL,DataFileState.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    close(fd);
    ((long long *)TmpMemPoint)[0]=sizeof(long long)+sizeof(struct person);
    ((struct person *)(TmpMemPoint+sizeof(long long)))->HeadMessage=0;
    ((struct person *)(TmpMemPoint+sizeof(long long)))->id=11111111111;
    ((struct person *)(TmpMemPoint+sizeof(long long)))->name[0]='w';
    ((struct person *)(TmpMemPoint+sizeof(long long)))->name[1]='z';
    ((struct person *)(TmpMemPoint+sizeof(long long)))->name[2]='\0';
    ((struct person *)(TmpMemPoint+sizeof(long long)))->Time=0;
    ((struct person *)(TmpMemPoint+sizeof(long long)))->NextPerson=0;
}
int main()
{
    //DataInit();
    DataBaseStart();
    printf("%d\n",person_pages_nums());
    /*增加一个联系人
    struct person tmp;
    tmp.HeadMessage=0;
    tmp.id=12;
    scanf("%s",tmp.name);
    tmp.Time=0;
    tmp.NumOfMessage=0;
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
    list_person_message(12,0);printf("\n");
    list_person_message(12,1);
    printf("\n");
    delete_message(12,3);
    list_person_message(12,0);
    list_person_message(12,1);
    */

    /*
    list_person_message(12,0);list_person_message(12,1);
    unsigned char a[70][70];
    scanf("%s",a[0]);
    scanf("%s",a[1]);
    search_message(a,2);
    //*/
    /*list_person_message(12,0);list_person_message(12,1);
    unsigned char a[70];
    scanf("%s",a);
    search_message_single(a);*/
    return 0;
}

#endif
