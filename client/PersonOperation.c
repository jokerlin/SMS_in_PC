#include"PersonOperation.h"
#include"memory.h"

#include<stdio.h>
int person_pages_nums()
{
    struct person *p=longlong_to_PersonPoint(PersonList->NextPerson);
    int ans=0;
    while(p!=MemBasePerson)
    {
        ans++;
        p=longlong_to_PersonPoint(p->NextPerson);
    }
    return ans/the_person_num_of_one_page+1;
}
void list_person(int numofpage)
{
    struct person *p=longlong_to_PersonPoint(PersonList->NextPerson);//开发者默认不输出
    for(int i=0;i<numofpage;i++)
    {
        for(int j=0;j<the_person_num_of_one_page;j++)
        {
            if(p==MemBasePerson) return;
            p=longlong_to_PersonPoint(p->NextPerson);
        }
    }
    for(int i=0;i<the_person_num_of_one_page;i++)
    {
        if(p==MemBasePerson) break;
        printf("%s %lld\n",p->name,p->id);
        p=longlong_to_PersonPoint(p->NextPerson);
    }
}

void add_person(struct person x)
{
    //printf("%lld\n",x.id);//debug
    struct person *q=new_person();
    //if(q==NULL) printf("1");
    //else printf("2");
    *q=x;
    struct person *p=PersonList;
    while(p->NextPerson&&longlong_to_PersonPoint(p->NextPerson)->Time>x.Time)
    {
        p=longlong_to_PersonPoint(p->NextPerson);
    }
    q->NextPerson=p->NextPerson;
    p->NextPerson=(void *)q-(void *)MemBasePerson;
}

void delete_person(long long id)
{
    struct person *p=PersonList;
    while(longlong_to_PersonPoint(p->NextPerson)->id!=id)
    {
        p=longlong_to_PersonPoint(p->NextPerson);
    }
    Recycle_Person(p->NextPerson);
    p->NextPerson=longlong_to_PersonPoint(p->NextPerson)->NextPerson;
}
void rename_person(long long id,char *name)
{
	struct person *p=PersonList;
 	while(longlong_to_PersonPoint(p->NextPerson)->id!=id)
    {
        p=longlong_to_PersonPoint(p->NextPerson);
    }
	strcpy(longlong_to_PersonPoint(p->NextPerson)->name,name);
}

int exist_in_list(long long person_id)
{
    struct person *p=longlong_to_PersonPoint(PersonList->NextPerson);
    while(p!=MemBasePerson&&p->id!=person_id)
    {
        p=longlong_to_PersonPoint(p->NextPerson);
    }
    if(p==MemBasePerson) return 0;
    return 1;
}
