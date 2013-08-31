信息类型约定

服务器accept一个socket之后读入数据，根据第一位判断内容：

1:开机，此时后面紧跟用"|number|"格式标明的手机号码，服务器将手机号码和对应的IP存入。

2:关机，此时后面紧跟用"|number|"格式标明的手机号码，服务器剔除对应的IP和手机号码。

3:发送短信，格式如下：
    {
        "sender": "number",
        "receiver": "number",
        "content": "",
        "time": "",
        "long": num
    }
