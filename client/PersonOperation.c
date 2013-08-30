#include"PersonOperation.h"
#include"memory.h"

#include<stdio.h>

void list_person(int numofpage)
{
    struct person *p=PersonList->NextPerson;//开发者默认不输出
    for(int i=0;i<numofpage;i++)
    {
        for(int j=0;j<the_person_num_of_one_page;j++)
        {
            if(p==NULL) return;
            p=p->NextPerson;
        }
    }
    for(int i=0;i<the_person_num_of_one_page;i++)
    {
        if(p==NULL) break;
        printf("%s %lld\n",p->name,p->id);
        p=p->NextPerson;
    }
}

void add_person(struct person x)
{
    struct person *q=new_person();
    *q=x;
    struct person *p=PersonList;
    while(p->NextPerson->Time>x.Time)
    {
        p=p->NextPerson;
    }
    q->NextPerson=p->NextPerson;
    p->NextPerson=q;
}

void delete_person(long long id)
{
    struct person *p=PersonList;
    while(p->NextPerson->id!=id)
    {
        p=p->NextPerson;
    }
    p->NextPerson=p->NextPerson->NextPerson;
}

int exist_in_list(long long person_id)
{
    struct person *p=PersonList->NextPerson;
    while(p&&p->id!=person_id)
    {
        p=p->NextPerson;
    }
    if(p==NULL) return 0;
    return 1;
}
