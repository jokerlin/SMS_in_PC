#!/usr/bin/env python
# coding: utf-8
# A simple server software, whick work as a SMS server

from SocketServer import ThreadingMixIn, StreamRequestHandler, TCPServer
import socket, select, json
import sqlite3
import sys, os, time

# 读取配置文件信息并进行相应配置
config = {}
f = open('config.json')
config = json.load(f)
f.close()
DEBUG = config['DEBUG']
client = {}
def opendb():
    con = sqlite3.connect(config['db'])
    cursor = con.cursor()
    return con, cursor
    

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
    con, cursor = opendb()
    cursor.execute(sql)
    con.commit()
    con.close()

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
    s2.timeout(30)
    try:
        a = s2.recv(2)
        s2.close()
        if "OK" in a:
            flag_send = False
    except:
        flag_send = True
    finally:
        if (id) and (not flag_send):
            sql = "DELETE FROM new WHERE id = %d" % id
            con, cursor = opendb()
            cursor.execute(sql)
            con.commit()
            con.close()
        savesms(sms, flag_send)

# 开机查询逻辑
def check(phone_num):
    # 定制查询
    sql = 'SELECT * FROM new WHERE receiver="%s"' % phone_num
    if DEBUG:
        print sql
    con, cursor = opendb()
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
    con.close()

# 开机函数
def poweron(s, client_ip):
	phone_num = s.rfile.readline()[:-1]
    s.wfile.write('OK')
    phone_num = phone_num[1:]
    if DEBUG:
        print str(phone_num) + " Power On..."
    client[phone_num] = client_ip
    # 查询是否有短信并发送
    check(phone_num)

# 关机函数
def poweroff(s):
	phone_num = s.rfile.readline()[:-1]
    s.wfile.write('OK')
    phone_num = phone_num[1:]
    if DEBUG:
        print str(phone_num) + " Power Off..."
    client.pop(phone_num)

# 短信函数
def message(s):
    print "message"
	sms = s.rfile.readline()[:-1]
    s.wfile.write('OK')
    sms = json.loads(sms)
    if DEBUG:
        print sms['receiver']
        print client.keys()
    if sms['receiver'] in client.keys():
        sendsms(sms, client[sms['receiver']])
    else:
        savesms(sms, True)

# 创建多线程服务器
class ThreadServer(ThreadingMixIn, TCPServer):
    pass

# 创建服务器处理逻辑
class Handler(StreamRequestHandler):

    def handle(self):
        addr, port = self.request.getpeername()
        flag = self.rfile.read(1)

        # 开机信息
        if flag == '1':
            poweron(self, addr)	

        # 关机信息
        elif flag == '2':
            poweroff(self)

        # 发送短信
        elif flag == '3':
            message(self)
        

# 主函数
def main():
    server = ThreadServer(('', config['PORT']), Handler)
    # 输出声明字符串并开始工作
    print "SMS Server alpha by Sandtears"  
    print "Start in Port %d...." % config['PORT']
    print "Please press <Control + C> to stop it"
    server.serve_forever()

if __name__ == "__main__":
    main()
