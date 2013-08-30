
#include"message.h"
#include"memory.h"
#include<stdio.h>

void list_person_message(long long person_id,int NumOfPage)
{
    struct person *p=PersonList->NextPerson;
    while(p->id!=person_id)
    {
        p=p->NextPerson;
    }
    struct message *q=p->HeadMessage;
    for(int i=0;i<NumOfPage;i++)
    {
        for(int j=0;j<the_message_num_of_one_page;j++)
        {
            if(q==NULL) return;
            q=q->NextMessage;
        }
    }
    for(int i=0;i<the_message_num_of_one_page;i++)
    {
        if(q==NULL) return;
        printf("%s",q->content);
        q=q->NextMessage;
    }
}

void delete_message(long long person_id,int message_id)
{
    struct person *p=PersonList->NextPerson;
    struct person *pre_p=PersonList;
    while(p->id!=person_id)
    {
        pre_p=pre_p->NextPerson;
        p=p->NextPerson;
    }
    if(message_id==0)
    {
        p->HeadMessage=p->HeadMessage->NextMessage;
        if(p->HeadMessage==NULL)
        {
            p->Time=0;
        }
        else
        {
            p->Time=p->HeadMessage->Time;
        }
        if(p->Time>p->NextPerson->Time) return;
        struct person *q=p;
        while(q->NextPerson->Time>p->Time)
        {
            q=q->NextPerson;
        }
        pre_p->NextPerson=pre_p->NextPerson->NextPerson;
        p->NextPerson=q->NextPerson;
        q->NextPerson=p;
        return;
    }
    struct message *q=p->HeadMessage;
    for(int i=0;i<message_id-1;i++)
    {
        q=q->NextMessage;
    }
    q->NextMessage=q->NextMessage->NextMessage;
}

void save_message(long long person_id,struct message x)
{
    struct person *p=PersonList->NextPerson;
    while(p->id!=person_id)
    {
        p=p->NextPerson;
    }
    struct message *p_message=new_message();
    *p_message=x;
    p_message->NextMessage=p->HeadMessage;
    p->HeadMessage=p_message;
    p->Time=p->HeadMessage->Time;
    //调整p的位置
    struct person *q=PersonList;
    while(q->NextPerson->Time>p->Time)
    {
        q=q->NextPerson;
    }
    p->NextPerson=q->NextPerson;
    q->NextPerson=p;
}
