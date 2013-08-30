#ifndef c_willzhang_c
#define c_willzhang_c

#include"willzhang.h"

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
    MemPoint=mmap(NULL,DataFileState.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
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
    printf("hello, my name is baka willzhang\n");
    return 0;
}

#endif
