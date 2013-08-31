#!/usr/bin/env python2.7
# coding: utf-8
# A simple server software, whick work as a SMS server

import socket, select
import sqlite3
import sys, os, time

con = sqlite3.connect('sms.db')
cursor = con.cursor()
client_ston = {} # 根据socket寻找phone_num
client_ntos = {} # 根据phone_num寻找socket
client_new = []
timeout = 10
smsending = "||A SMS END||" # 短信结束标识

# 存储短信
def savesms(sms):
    # 分析短信并保存
    pass

# 发送短信
def sendsms(s, sms):
    # 接受短信，验证是否发送成功，未发送成功则保存
    s.send(sms)
    a = s.recv(2)
    if "OK" in a:
        pass
    else:
        savesms(sms)

# 子进程
def child(server):
    while 1:
        # 循环接受客户端手机号，并搜索是否有其短信
        s, addr = server.accept()
        phone_num = s.read()
        phone_num = phone_num[9:]
        result = cursor.execute("SELECT * FROM new WHERE receiver='%s'" % phone_num)
        result = result.fetchall()
       
        # 如果有短信，则将短信分条发送
        if result != 0:
            for smsobj in result:
                sms = ''
                for smsinfo in smsobj:
                    sms = sms + unicode(u) + u"|"
                sms = sms.encode('utf-8')
                sms = sms[:] + smsending
                while 1:
                    r, w, e = [s], [], []
                    r, w, e = select.select(r, w, e, timeout)
                    if s in r:
                        sendsms(s, sms)
                        break
                    time.sleep(2)
                
        # 将对应的socket和phone_num存入字典和记录数组
        client_ntos[phone_num] = s
        client_ston[s] = phone_num
        client_new.append(s)

# 父进程
def parent():
    r = w = e = []
    while 1:

        # 将新连接加入队列
        if client_new:
            try:
                r.append(client_new.pop())
            except:
                pass
        
        # 查看各套接字状态
        r, w, e = select.select(r, w, e, timeout)

        # 有套接字错误则删除
        if e:
            for s in e:
                tmp = client_ston.pop(s)
                client_ntos.pop(tmp)

        # 有套接字可读则转发
        if r:
            
            # 读入内容
            for s in r:
                read_result = s.recv(4096)
            
            # 分析套接字，区分关机和发送短信
            if "Off" in read_result[:12]:
                # 关机
                r.remove(s)
                tmp = client_ston.pop(s)
                client_ntos.pop(tmp)
            else:    
                # 发送短信
                read_result = read_result.split(smsending)
                for smsobj in read_result:
                    if smsobj:
                        smsinfo = smsobj.split("|")
                        receiver_num = smsinfo[1]
                        
                        # 若接受人在线
                        if client_ntos[receiver_num] in w:
                            sendsms(client_ntos[receiver_num], smsobj)
                        # 若接受人不在线
                        else:
                            savesms(smsobj)
        time.sleep(10)

# 主函数
def main():

	# 配置socket相关属性，开启端口复用
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    # 尝试绑定端口
    try:
        server.bind(('0.0.0.0', PORT))
    except Exception as e:
        print e.strerror
        print "端口绑定失败，程序结束"
        sys.exit()
    server.listen(-1)

    # 区分子进程和父进程
    if os.fork() != 0:
        print "SMS server v1.0 Start in Port %d...." % PORT
        print "Please press <Control + C> to stop it"
        parent()
    else:
        child(server)

# Test
if __name__ == "__main__":
    main()
