/*************************************************************************
    > File Name: sandtears.c
    > Author: Sandtears
    > Mail: me@sandtears.com
    > Created Time: 2013年08月31日 星期六 20时47分14秒
 ************************************************************************/

#include <stdio.h>
#include "willzhang.h"
#include "sandtears.h"
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define MAXLEN 1024

#define DEBUG 1

void longlong_to_string(long long number, char* str) {
    int len, i;
    char re_str[20];
    // 求出倒序字符串
    for(len = 0; number > 0; len++) {
        re_str[len] = number % 10 + '0';
        number = number / 10;
    }
    //re_str[len] = number + '0';
    re_str[len] = 0;
    for(i = 0; i < len; i++) {
        str[i] = re_str[len - i - 1];
    }
    str[i] = 0;
    return;
}
void time_to_string(time_t time, char* str) {
    //char *str=(char *)malloc(sizeof(char)*50);
    // 时间转字符串
    // 2013-03-22 03:02:02
    char format[10] = "%F %T";
    struct tm *ptr = localtime(&time);
    strftime(str, MAXLEN, format, ptr);
    if(DEBUG) {
        printf("Time: %s\n", str);
    }
    return;
}

void sms_to_string(struct message msg, char* sms) {
    // 逐个转化为char* 型
    char str_receiver[20], str_sender[20], str_time[30];
    char str_flag_lms[2];
    time_to_string(msg.Time, str_time);
    longlong_to_string(msg.receiver, str_receiver);
    longlong_to_string(msg.sender, str_sender);
    str_flag_lms[0] = msg.flag_lms + '0';
    str_flag_lms[1] = 0;

    // 字符串合成
    strcat(sms, "3{\"sender\":\"");
    strcat(sms, str_sender);
    strcat(sms, "\",\"receiver\":\"");
    strcat(sms, str_receiver);
    strcat(sms, "\",\"content\":\"");
    strcat(sms, msg.content);
    strcat(sms, "\",\"Time\":\"");
    strcat(sms, str_time);
    strcat(sms, "\",\"flag_lms\":");
    strcat(sms, str_flag_lms);
    strcat(sms, "}\n");
    return;
}

int sock_power_on(char* phone_num, char* server_ip, int server_port, int client_port) //开机，返回文件描述符
{
    int sockfd, len;
    struct sockaddr_in serverAddr;
    char buf[1024];

    // 创建socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("on_socket_client");
        return -1;
    }

    // 绑定IP
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port);
    if(inet_pton(AF_INET, server_ip, &serverAddr.sin_addr) < 0){
        perror("on_inet_pton");
        return -1;
    }

    // 连接服务器
    if(connect(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
        perror("on_connect");
        printf("%s\n", server_ip);
        return -1;
    }

    // 数据传输，将手机号发给服务端
    buf[0] = '1';
    buf[1] = '|';
    buf[2] = 0;
    strcat(buf, phone_num);
    strcat(buf, "\n");
    len = strlen(buf);
    if(DEBUG) {
        printf("send: %s", buf);
    }
    if(write(sockfd, buf, len) != len) {
        perror("on_write");
        return -1;
    }
    memset(buf, 0, sizeof(buf));
    if((read(sockfd, buf, MAXLEN)) < 0) {
        perror("on_read");
        return -1;
    }
    if((buf[0] == "O")&&(buf[1] == "K")) {
        perror("on_read_2");
        return -1;
    }
    // 关闭socket
    close(sockfd);

    //建立连接，监听并返回
    // 创建socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("on_socket_server");
        return -1;
    }

    // 绑定地址
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_port = htons(client_port);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
        perror("on_bind");
        return -1;
    }

    //设置端口复用
    int on = 1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0) {
        perror("setsockopt");
        return -1;
    }

    // 监听
    if(listen(sockfd, 1) < 0){
        perror("on_listen");
        return -1;
    }

    // 返回结果
    return sockfd;
}

int sock_power_off(int serverfd, char* phone_num, char* server_ip, int server_port) //关机
{
    int sockfd, len;
    struct sockaddr_in serverAddr;
    char buf[1024];

    close(serverfd);
    // 创建socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("off_socket");
        return -1;
    }

    // 绑定IP
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port);
    if(inet_pton(AF_INET, server_ip, &serverAddr.sin_addr) < 0){
        perror("off_inet_pton");
        return -1;
    }

    // 连接服务器
    if(connect(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
        perror("off_connect");
        return -1;
    }

    // 数据传输，将手机号发给服务端
    buf[0] = '2';
    buf[1] = '|';
    buf[2] = 0;
    strcat(buf, phone_num);
    strcat(buf, "\n");
    len = strlen(buf);
    if(DEBUG) {
        printf("send: %s", buf);
    }
    if(write(sockfd, buf, len) != len){
        perror("off_write");
        return -1;
    }
    memset(buf, 0, sizeof(buf));
    if((read(sockfd, buf, MAXLEN)) < 0) {
        perror("off_read");
        return -1;
    }
    if((buf[0] == "O")&&(buf[1] == "K")) {
        perror("off_read_2");
        return -1;
    }
    // 关闭socket
    close(sockfd);
    return 1;
}

int sock_sendmsg(struct message msg, char* server_ip, int server_port)
{
    int sockfd, len;
    struct sockaddr_in serverAddr;
    char buf[1024];

    // 创建socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("sendmsg_socket");
        return -1;
    }

    // 绑定IP
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server_port);
    if(inet_pton(AF_INET, server_ip, &serverAddr.sin_addr) < 0){
        perror("sendmsg_inet_pton");
        return -1;
    }

    // 连接服务器
    if(connect(sockfd, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) < 0){
        perror("sendmsg_connect");
        return -1;
    }

    if(DEBUG) {
        printf("SendMsg: Connection success\n");
    }

    // 数据传输，将短信发送出去
    sms_to_string(msg, buf);
    len = strlen(buf);
    if(write(sockfd, buf, len) != len){
        perror("sendmsg_write");
        return -1;
    }
    if(DEBUG) {
        printf("SendMsg: %s\n", buf);
    }
    memset(buf, 0, sizeof(buf));
    if((read(sockfd, buf, MAXLEN)) < 0) {
        perror("sendmsg_read");
        return -1;
    }
    if((buf[0] == "O")&&(buf[1] == "K")) {
        perror("sendmsg_read_2");
        return -1;
    }
    if(DEBUG) {
        printf("SendMsg: get:%s\n", buf);
    }
    // 关闭socket
    printf("SendMsg Success...\n");
    return 1;
}
