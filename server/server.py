#!/usr/bin/env python
# coding: utf-8
# A simple server software, whick work as a SMS server

from SocketServer import ThreadingMixIn, StreamRequestHandler, TCPServer
import socket, select, json
import sqlite3
import sys, os, time, threading

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
    if type(sms['Time']) == type(5):
        sms['Time'] = time.strftime("%F %T")
    tableName = 'old' # 分析要存入的表
    if flag_new:
        tableName = 'new'
    # 生成SQL语句
    sms['table'] = tableName
    sql = 'INSERT INTO %(table)s (sender, receiver, content, time, long) VALUES ("%(sender)s", "%(receiver)s", "%(content)s", "%(Time)s", %(flag_lms)d);' % sms

    # 执行sql语句并提交
    if DEBUG:
        print sql
    con, cursor = opendb()
    cursor.execute(sql)
    con.commit()
    con.close()
    
# 延时发送短信
def sendsms_wait(sms, time):
    time.sleep(time)
    if sms['receiver'] in client.keys():
        sendsms(sms, sms['receiver'], client[sms['receiver']])
    else:
        savesms(sms, True)

# 发送短信
def sendsms(sms, phone_num, client_ip, id=0):
    # 尝试发送短信，如果成功则存入
    if type(sms['Time']) == type(5):
        sms['Time'] = time.strftime("%F %T")
    if DEBUG:
        print "send message..."
    flag_send = True
    s2 = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    try:
        s2.connect((client_ip, config['PORT_client']))
        tmp_content = sms['content']
        sms['content'] = "||zwsbzwsb||"
        str_sms = json.dumps(sms)
        str_sms = str_sms.replace('||zwsbzwsb||', tmp_content.encode('utf8'))
        s2.send(str_sms)
        if DEBUG:
            print json.dumps(str_sms)
        sms['content'] = tmp_content
        s2.settimeout(1000)
        a = s2.recv(20)
        s2.close()
        if DEBUG:
            print a
            print phone_num
            print type(phone_num)
        if str(phone_num) in a:
            if DEBUG:
                print "send success, %s" % a
            flag_send = False
    except Exception as e:
        if DEBUG:
            print e
    finally:
        if (id):
            if not flag_send:
                sql = "DELETE FROM new WHERE id = %d" % id
                if DEBUG:
                    print sql
                con, cursor = opendb()
                cursor.execute(sql)
                con.commit()
                con.close()
                savesms(sms, flag_send)
        else:
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
       newsms['Time'] = i[3]
       newsms['flag_lms'] = i[4]
       id = i[5]
       sendsms(newsms, phone_num, client[newsms['receiver']], id)
    con.close()

# 开机函数
def poweron(s, client_ip):
    phone_num = s.rfile.readline()
    phone_num = phone_num[:-1]
    s.wfile.write('OK')
    phone_num = phone_num[1:]
    if DEBUG:
        print str(phone_num) + " Power On..."
    client[phone_num] = client_ip
    # 查询是否有短信并发送
    check(phone_num)

# 关机函数
def poweroff(s):
    phone_num = s.rfile.readline()
    phone_num = phone_num[:-1]
    s.wfile.write('OK')
    phone_num = phone_num[1:]
    if DEBUG:
        print str(phone_num) + " Power Off..."
    client.pop(phone_num)

# 短信函数
def message(s):
    print "message"
    sms = s.rfile.readline()
    sms = sms[:-1]
    s.wfile.write('OK')
    print repr(sms);
    sms = json.loads(sms)
    sms['flag_lms'] = 0
    if DEBUG:
        print sms['receiver']
        print client.keys()
    if sms["Time"] == 0:
        if sms['receiver'] in client.keys():
            sendsms(sms, sms['receiver'], client[sms['receiver']])
        else:
            savesms(sms, True)
    else:
        threading.Thread(target = sendsms_wait, args = (sms, sms['Time']));

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
