#ifndef c_willzhang_h
#define c_willzhang_h
#include<time.h>
struct message
{
    long long receiver;
    long long sender;
    unsigned char content[71];
    struct message *NextMessage;
    time_t Time;
    int flag_lms;//长短信标识，0代表非长短信，数字n代表长短信第n条
    int HasBeenReaded;//0表示未读，1表示读了
};

struct person
{
    long long id;//11位电话号码
    char name[21];//昵称为最长20位的ASCII码
    time_t Time;//该联系人的最新一条信息的时间
    struct message *HeadMessage;//指向第一条信息
};

long long *DataHead;//文件内存的第一个long long大小的内存指针，存储的内容为未使用的文件内存首地址
void *MemPoint;//未使用的文件内存首地址
struct person *PersonList;//文件内存的继第一个long long之后的第一个person大小的内存的指针，指向第一个联系人（开发者）
void DataBaseStart();//开机时调用，为DataHead和MemPoint和PersonList赋初值
void DataInit();//第一次开机时调用，初始化文件内存
//内存存的前两个变量,一个long long ,一个person

#endif
