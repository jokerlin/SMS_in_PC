#include "willzhang.h"
int sock_power_on(char* phone_num, char* server_ip, int server_port, int client_port) ;
    //开机
    //传入参数： 手机号码，服务器IP地址，服务器端口，监听端口
    //返回接受信息用的文件描述符，-1代表失败
int sock_power_off(int serverfd, char* phone_num, char* server_ip, int server_port) ;
    //关机
    //传入参数： 监听用文件描述符，手机号码，服务器ip，服务器端口
    //返回运行结果，-1代表失败
int sock_sendmsg(struct message msg, char* server_ip, int server_port) ;
    //发送短信
    //传入参数：短信结构体，服务器ip，服务器端口
    //返回运行结果，-1代表失败
void longlong_to_string(long long number, char* str);
