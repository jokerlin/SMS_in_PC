#include <willzhang.h>
int sock_power_on() //开机，返回文件描述符
void sock_power_off(int sockfd) //关机，需传入sockey的文件描述符
int sock_sendmsg(int s, struct message msg) //发送短信，需传入文件描述符和短信结构体
