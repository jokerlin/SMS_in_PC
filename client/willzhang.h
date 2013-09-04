#ifndef c_willzhang_h
#define c_willzhang_h

#include<time.h>


struct message
{
    long long receiver;
    long long sender;
    unsigned char content[250];
    //struct message *NextMessage;
    long long NextMessage;//NextMessage+MemBase才为真正的指针,为0代表为空
    time_t Time;
    int flag_lms;//长短信标识，0代表非长短信，数字n代表长短信第n条
    int HasBeenReaded;//0表示未读，1表示读了
};

struct person
{
    long long id;//11位电话号码
    char name[21];//昵称为最长20位的ASCII码
    time_t Time;//该联系人的最新一条信息的时间,若无信息则置为0
    //struct message *HeadMessage;//指向第一条信息
    long long HeadMessage;//为0代表空
    //struct person *NextPerson;
    long long NextPerson;//为0代表空
    int NumOfMessage;
};

long long *DataHead;//文件内存的第一个long long大小的内存指针，存储的内容为未使用的文件内存首地址
void *MemPoint;//未使用的文件内存首地址
void *MemBase;//文件内存首地址
struct person *MemBasePerson;//由MemBase强制转换转换而来
struct message *MemBaseMessage;//由MemBase强制转换转换而来
struct person *PersonList;//文件内存的继第一个long long之后的第一个person大小的内存的指针，指向第一个联系人（开发者）
void DataBaseStart(char *path);//开机时调用，为DataHead和MemPoint和PersonList赋初值
void DataInit(char *path);//第一次开机时调用，初始化文件内存
//内存存的前两个变量,一个long long ,一个person

void *PersonRecycleBinPoint;//指向文件PersonRecycleBin的指针，文件的第一个longlong内存存着回收站的终点，弱终点等于起点，则回收站无内容
void *MessageRecycleBinPoint;//指向文件MessageRecycleBin的指针，

struct person * longlong_to_PersonPoint(long long x);
struct message * longlong_to_MessagePoint(long long x);
#endif
