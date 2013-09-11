#include <curses.h>
#include"sandtears.h"
#include"PersonOperation.h"
#include"memory.h"
#include"power_on.h"

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
//DON'T EDIT BEFORE 
        //printf("%s %lld\n",p->name,p->id);
        //if (i==0)
        //{
        //    attron(A_REVERSE);
        //    mvaddstr(WELCOME_POS_X + 3 + i, 25, p->name);
        //    char id_s[20];
        //    longlong_to_string(p->id, id_s);
        //    mvaddstr(WELCOME_POS_X + 3 + i, 50, id_s);
        //    refresh();
        //    attroff(A_REVERSE);
        //    p=longlong_to_PersonPoint(p->NextPerson);
        //    continue;
        //}
        mvaddstr(WELCOME_POS_X + 3 + i, 25, p->name);
        char id_s[20];
        longlong_to_string(p->id, id_s);
        mvaddstr(WELCOME_POS_X + 3 + i, 50, id_s);
        refresh();
//DON'T EDIT BELOW
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

	long long message_to_be_delete=longlong_to_PersonPoint(p->NextPerson)->HeadMessage;
	while(message_to_be_delete != 0)
	{
		struct message *p_message=longlong_to_MessagePoint(message_to_be_delete);
		Recycle_Message(message_to_be_delete);
		message_to_be_delete=p_message->NextMessage;
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
