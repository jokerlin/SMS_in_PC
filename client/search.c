#include"search.h"
#include"willzhang.h"
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
int strlen_unsigned(unsigned char a[])
{
    int ans=0;
    unsigned char *p=a;
    while(p!=0)
    {
        ans++;
        p++;
    }
    return ans;
}
/******************AC自动机***********调用clear初始化*******hdu2222验证*********/
struct node
{
    struct node *father;//父亲节点
    struct node *son[256];//儿子节点
    struct node *pre;//前缀指针
    unsigned char ch;//其父节点通过ch字符到达此节点
    int bStopNode;//是否某个模式串的终止节点,及该模式串出现的次数（用来应对重复串）
} NODES[1000000];
int nNode;
struct node *root;
void Clear()//自动机的初始化,与清空
{
    root=NODES;
    nNode=1;
    root->father=NULL;//root的父亲为空
    memset(root->son,0,sizeof(root->son));//初始没有儿子
    root->pre=NULL;//root的前缀指针为空
    root->bStopNode=0;//root的终止节点数显然为0
}
void add_str(unsigned char a[])
{
    struct node *p=root;
    int len=strlen_unsigned(a);
    for(int i=0; i<len; i++)
    {
        if(p->son[a[i]])
        {
            p=p->son[a[i]];
        }
        else//新增节点
        {
            p->son[a[i]]=NODES+nNode++;
            struct node *q=p->son[a[i]];//q为p的子节点
            q->father=p;
            q->ch=a[i];
            q->pre=NULL;
            memset(q->son,0,sizeof(q->son));
            p=q;
            p->bStopNode=0;
        }
    }
    p->bStopNode++;//终止节点
}

/********队列结构********/
struct node * Q[50000];//队列中的有效元素为[qhead+1.....qtail]
int qtail,qhead;
void Qinit()
{
    qtail=-1;
    qhead=-1;
}
void Qpush_back(struct node *x)
{
    qtail=(qtail+1)%50000;
    Q[qtail]=x;
}
struct node * Qfront()
{
    return Q[qhead+1];
}
void Qpop_front()
{
    qhead=(qhead+1)%50000;
}
/**************************/

void add_pre()//增加前缀指针，这之前只是字典树，只有执行了这一步才称为AC自动机
{
    struct node *p=root;
    Qinit();
    Qpush_back(p);
    while(qhead+1<=qtail)//bfs
    {
        p=Qfront();Qpop_front();
        for(int i=0; i<256; i++) //26个儿子
        {
            if(p->son[i]==NULL)	continue;
            struct node *q=p->son[i];//q为p的儿子
            Qpush_back(q);
            struct node *Father=p;
            //对于当前节点q，设他的父节点Father与他的边上的字符为Ch，如果他的父节点的前缀指针所指向的节点的儿子中，有通过Ch字符指向的儿子，那么当前节点的前缀指针指向该儿子节点，否则通过当前节点的父节点的前缀指针所指向点的前缀指针，继续向上查找，直到到达根节点为止。
            Father=Father->pre;
            q->pre=root;
            while(Father)
            {
                if(Father->son[i])
                {
                    q->pre=Father->son[i];
                    break;
                }
                Father=Father->pre;
            }
        }
    }
}
int Find(unsigned char a[])//在自动机中查找a串匹配的字符串数目
{
    struct node *p=root;
    int len=strlen_unsigned(a);
    int ans=0;
    for(int i=0; i<len; i++)
    {
        while(p&&p->son[a[i]-'\0']==NULL)
        {
            p=p->pre;
        }
        if(p==NULL)
        {
            p=root;
            continue;
        }
        p=p->son[a[i]-'\0'];
        struct node *q=p;
        while(q)//此节点的前缀节点若为终止节点，则显然也被匹配到
        {
            ans+=q->bStopNode;
            q->bStopNode=0;//避免被重复计算
            q=q->pre;
        }
    }
    return ans;
}
/*************************************************************/
struct ans_of_search
{
    struct message *cont;
    int num;//匹配的关键字个数
}ans[1000010];
int nans;
int cmp(const void *a,const void *b)
{
    struct ans_of_search *aa=(struct ans_of_search *)a;
    struct ans_of_search *bb=(struct ans_of_search *)b;
    return bb->num-aa->num;
}
void search_message(unsigned char a[][70],int m)
{
    Clear();
    for(int i=0;i<m;i++)
    {
        add_str(a[m]);
    }
    add_pre();
    nans=0;
    for(struct person *p=PersonList->NextPerson;p;p=p->NextPerson)
    {
        for(struct message *q=p->HeadMessage;q;q=q->NextMessage)
        {
            int tnum=Find(q->content);
            if(tnum>=1)
            {
                ans[nans].cont=q;
                ans[nans++].num=tnum;
            }
        }
    }
    qsort(ans,0,sizeof(ans[0]),cmp);
    for(int i=0;i<nans;i++)
    {
        printf("%s 匹配的关键字数：%d\n",ans[i].cont->content,ans[i].num);
    }
}
/**********************KMP********************/
int Fail[400];//j=fail[i]意为0,,,,,j-1=i-j,,,,,,,i-1,设定关键字不超过400个字节
void getFail(unsigned char *P)
{
	int m=strlen_unsigned(P);
	Fail[0]=0;Fail[1]=0;
	for(int i=1;i<m;i++)
	{
		int j=Fail[i];
		while(j&&P[i]!=P[j])	j=Fail[j];
		Fail[i+1]=P[i]==P[j]?j+1:0;
	}
}
int kmp_find(unsigned char *T,unsigned char *P)//在T中寻找P出现的位置
{
	int n=strlen_unsigned(T),m=strlen_unsigned(P);
	getFail(P);
	int j=0;
	for(int i=0;i<n;i++)
	{
		while(j&&P[j]!=T[i])	j=Fail[j];
		if(P[j]==T[i])	j++;
		if(j==m)	return 1;//匹配成功
	}
	return 0;//匹配失败
}
/****************************************/
void search_message_single(unsigned char a[])
{
    for(struct person *p=PersonList->NextPerson;p;p=p->NextPerson)
    {
        for(struct message *q=p->HeadMessage;q;q=q->NextMessage)
        {
            if(kmp_find(q->content,a))
            {
                printf("%s\n",q->content);
            }
        }
    }
}
