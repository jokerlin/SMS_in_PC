
#include"message.h"
#include"memory.h"
#include<stdio.h>
int person_message_pages_nums(long long person_id)
{
    struct person *p=longlong_to_PersonPoint(PersonList->NextPerson);
    while(p->id!=person_id)
    {
        p=longlong_to_PersonPoint(p->NextPerson);
    }
    return p->NumOfMessage/the_message_num_of_one_page+1;
}
void list_person_message(long long person_id,int NumOfPage)
{
    struct person *p=longlong_to_PersonPoint(PersonList->NextPerson);
    while(p->id!=person_id)
    {
        p=longlong_to_PersonPoint(p->NextPerson);
    }
    struct message *q=longlong_to_MessagePoint(p->HeadMessage);
    for(int i=0;i<NumOfPage;i++)
    {
        for(int j=0;j<the_message_num_of_one_page;j++)
        {
            if(q==MemBaseMessage) return;
            q=longlong_to_MessagePoint(q->NextMessage);
        }
    }
    for(int i=0;i<the_message_num_of_one_page;i++)
    {
        if(q==MemBaseMessage) return;
        printf("%s\n",q->content);
        q=longlong_to_MessagePoint(q->NextMessage);
    }
}

void delete_message(long long person_id,int message_id)
{
    struct person *p=longlong_to_PersonPoint(PersonList->NextPerson);
    struct person *pre_p=PersonList;
    while(p->id!=person_id)
    {
        pre_p=longlong_to_PersonPoint(pre_p->NextPerson);
        p=longlong_to_PersonPoint(p->NextPerson);
    }
    p->NumOfMessage--;
    if(message_id==0)
    {
        Recycle_Message(p->HeadMessage);
        p->HeadMessage=longlong_to_MessagePoint(p->HeadMessage)->NextMessage;
        if(p->HeadMessage==0)
        {
            p->Time=0;
        }
        else
        {
            p->Time=longlong_to_MessagePoint(p->HeadMessage)->Time;
        }
        if(p->Time>longlong_to_PersonPoint(p->NextPerson)->Time) return;
        struct person *q=p;
        while(longlong_to_PersonPoint(q->NextPerson)->Time>p->Time)
        {
            q=longlong_to_PersonPoint(q->NextPerson);
        }
        pre_p->NextPerson=longlong_to_PersonPoint(pre_p->NextPerson)->NextPerson;
        p->NextPerson=q->NextPerson;
        q->NextPerson=(void *)p-(void *)MemBasePerson;
        return;
    }
    struct message *q=longlong_to_MessagePoint(p->HeadMessage);
    for(int i=0;i<message_id-1;i++)
    {
        q=longlong_to_MessagePoint(q->NextMessage);
    }
    Recycle_Message(q->NextMessage);
    q->NextMessage=longlong_to_MessagePoint(q->NextMessage)->NextMessage;
}

void save_message(long long person_id,struct message x)
{
    struct person *p=longlong_to_PersonPoint(PersonList->NextPerson);
    while(p->id!=person_id)
    {
        p=longlong_to_PersonPoint(p->NextPerson);
    }
    p->NumOfMessage++;
    struct message *p_message=new_message();
    *p_message=x;
    p_message->NextMessage=p->HeadMessage;
    p->HeadMessage=(void *)p_message-(void *)MemBaseMessage;
    p->Time=longlong_to_MessagePoint(p->HeadMessage)->Time;
    //调整p的位置
    struct person *q=PersonList;
    while(q->NextPerson&&longlong_to_PersonPoint(q->NextPerson)->Time>=p->Time)
    {
        q=longlong_to_PersonPoint(q->NextPerson);
    }
    if(q==p)  return;
    p->NextPerson=q->NextPerson;
    q->NextPerson=(void *)p-(void *)MemBasePerson;
}
