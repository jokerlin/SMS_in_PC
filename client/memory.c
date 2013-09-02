#include"memory.h"
#include<stdio.h>
struct person * new_person()
{
    struct person *ans;

    if(((long long *)(PersonRecycleBinPoint))[0]!=0)
    {
        ans=(struct person *)(MemBase+(((long long *)(PersonRecycleBinPoint+((long long *)PersonRecycleBinPoint)[0]))[0]));
        (((long long *)PersonRecycleBinPoint)[0])-=sizeof(long long);
        return ans;
    }

    ans=(struct person *)MemPoint;
    MemPoint+=sizeof(struct person);
    DataHead[0]+=sizeof(struct person);
    return ans;
}
struct message * new_message()
{
    struct message *ans;

    if(((long long *)(MessageRecycleBinPoint))[0]!=0)
    {
        ans=(struct message *)(MemBase+(((long long *)(MessageRecycleBinPoint+((long long *)MessageRecycleBinPoint)[0]))[0]));
        (((long long *)MessageRecycleBinPoint)[0])-=sizeof(long long);
        return ans;
    }

    ans=(struct message *)MemPoint;
    MemPoint+=sizeof(struct message);
    DataHead[0]+=sizeof(struct message);
    return ans;
}

void Recycle_Person(long long addr)//实际上是放入回收站中
{
    ((long long *)PersonRecycleBinPoint)[0]+=sizeof(long long);
    ((long long *)(PersonRecycleBinPoint+((long long *)PersonRecycleBinPoint)[0]))[0]=addr;
}

void Recycle_Message(long long addr)//实际上是放入回收站中
{
    ((long long *)MessageRecycleBinPoint)[0]+=sizeof(long long);
    ((long long *)(MessageRecycleBinPoint+((long long *)MessageRecycleBinPoint)[0]))[0]=addr;
}
