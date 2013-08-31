#!/usr/bin/env python
# coding: utf-8
# A simple server software, whick work as a SMS server

import socket, select, json
import sqlite3
import sys, os, time

# 读取配置文件信息并进行相应配置
config = {}
f = open('config.json')
config = json.load(f)
f.close()
DEBUG = config['DEBUG']
con = sqlite3.connect(config['db'])
cursor = con.cursor()
client = {}

# 存储短信
def savesms(sms, flag_new):
    tableName = 'old' # 分析要存入的表
    if flag_new:
        tableName = 'new'
    # 生成SQL语句
    sms['table'] = tableName
    sql = 'INSERT INTO %(table)s (sender, receiver, content, time, long) VALUES ("%(sender)s", "%(receiver)s", "%(content)s", "%(time)s", %(long)d);' % sms

    # 执行sql语句并提交
    if DEBUG:
        print sql
    cursor.execute(sql)
    con.commit()

# 发送短信
def sendsms(sms, client_ip, id=0):
    # 尝试发送短信，如果成功则存入
    if DEBUG:
        print "send message..."
    flag_send = True
    s2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s2.connect((client_ip, config['PORT_client']))
    s2.send(json.dumps(sms))
    if DEBUG:
        print json.dumps(sms)
    a = s2.recv(2)
    s2.close()
    if "OK" in a:
        flag_send = False
    if (id) and (not flag_send):
        sql = "DELETE FROM new WHERE id = %d" % id
        cursor.execute(sql)
        con.commit()
    savesms(sms, flag_send)

# 开机查询逻辑
def check(phone_num):
    # 定制查询
    sql = 'SELECT * FROM new WHERE receiver="%s"' % phone_num
    if DEBUG:
        print sql
    result = cursor.execute(sql)
    result = result.fetchall()
    for i in result:
       newsms = {}
       newsms['sender'] = i[0]
       newsms['receiver'] = i[1]
       newsms['content'] = i[2]
       newsms['time'] = i[3]
       newsms['long'] = i[4]
       id = i[5]
       sendsms(newsms, client[newsms['receiver']], id)

# 开机函数
def poweron(s, client_ip):
    phone_num = s.recv(4096)
    s.send('OK')
    s.close()
    phone_num = phone_num[1:]
    if DEBUG:
        print str(phone_num) + " Power On..."
    client[phone_num] = client_ip
    # 查询是否有短信并发送
    check(phone_num)

# 关机函数
def poweroff(s):
    phone_num = s.recv(4096)
    s.send('OK')
    s.close()
    phone_num = phone_num[1:]
    if DEBUG:
        print str(phone_num) + " Power Off..."
    client.pop(phone_num)

# 短信函数
def message(s):
    print "message"
    sms = s.recv(4096)
    s.send('OK')
    s.close()
    sms = json.loads(sms)
    if DEBUG:
        print sms['receiver']
        print client.keys()
    if sms['receiver'] in client.keys():
        sendsms(sms, client[sms['receiver']])
    else:
        savesms(sms, True)

# 开始监听并进行操作
def serverdo(s, addr):
    if DEBUG:
        print str(addr) + " Connecting...."
    flag = s.recv(1) # 读取连接第一个字符，判断连接作用

    # 开机信息
    if flag == '1':
        poweron(s, addr[0])	

    # 关机信息
    elif flag == '2':
        poweroff(s)

    # 发送短信
    elif flag == '3':
        message(s)
        
    # 非法信息
    else:
        s.close()

# 主函数
def main():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1) # 配置socket相关属性，开启端口复用
    # 尝试绑定端口
    try:
        server.bind(('0.0.0.0', config['PORT']))
    except Exception as e:
        print e.message
        print "程序出现错误，强制结束"
        sys.exit()
    finally:
        server.listen(-1)
        print "SMS Server alpha by Sandtears"  
        print "Start in Port %d...." % config['PORT']
        print "Please press <Control + C> to stop it"
        # 输出声明字符串并开始工作

        # 获取客户端连接
        while 1:
            s, addr = server.accept()
            serverdo(s, addr)

if __name__ == "__main__":
    main()
