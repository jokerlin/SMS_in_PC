#include "string_to_message.h"
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
int strlen_unsigned(unsigned char a[])
{
    int ans=0;
    unsigned char *p=a;
        while((*p)!=0)
    {
        ans++;
        p++;
    }
    return ans;
}


long long string_to_longlong(unsigned char *Begin,unsigned char *End)
{
	unsigned char *p=End;
	long long quan=1;
	long long ans=0;
	while(p!=Begin)
	{
		ans+=quan*(p[0]-'0');
		quan*=10;
		p--;
	}
	ans+=quan*(p[0]-'0');
	return ans;
}
struct message string_to_message(unsigned char *str)
{
	struct message ans;
	int len=strlen_unsigned(str);
	int index1,index2;

	for(int i=0;i<len;i++)
	{
		if(str[i]==':'&&str[i-1]!='\\')
		{
			index1=i+2;
			break;
		}
	}
	for(int i=index1+1;i<len;i++)
	{
		if(str[i]==','&&str[i-1]!='\\')
		{
			index2=i-2;
			break;
		}
	}
	ans.receiver=string_to_longlong(str+index1,str+index2);
	//printf("%lld\n",ans.receiver);

	for(int i=index2+1;i<len;i++)
	{
		if(str[i]==':'&&str[i-1]!='\\')
		{
			index1=i+2;
			break;
		}
	}
	for(int i=index1+1;i<len;i++)
	{
		if(str[i]==','&&str[i-1]!='\\')
		{
			index2=i-2;
			break;
		}
	}
	ans.sender=string_to_longlong(str+index1,str+index2);
	//printf("%lld\n",ans.sender);

	for(int i=index2+1;i<len;i++)
	{
		if(str[i]==':'&&str[i-1]!='\\')
		{
			index1=i+2;
			break;
		}
	}
	for(int i=index1+1;i<len;i++)
	{
		if(str[i]==','&&str[i-1]!='\\')
		{
			index2=i-2;
			break;
		}
	}
	/*for(int i=index1;i<=index2;i++)
	{
		printf("%c",str[i]);
	}
	printf("\n");*/
	memcpy(ans.content,str+index1,sizeof(char)*(index2-index1+1));
	ans.content[index2-index1+1]='\0';
	//ans.content[index2-index1+1]='\0';
	//printf("%s\n",ans.content);

/*
	for(int i=index2+1;i<len;i++)
	{
		if(str[i]==':'&&str[i-1]!='\\')
		{
			index1=i+2;
			break;
		}
	}
	for(int i=index1+1;i<len;i++)
	{
		if(str[i]==','&&str[i-1]!='\\')
		{
			index2=i-2;
			break;
		}
	}
	ans.NextMessage=string_to_longlong(str+index1,str+index2);
	printf("%lld\n",ans.NextMessage);*/

	//printf("%d %d\n",index1,index2);
	for(int i=index2+1;i<len;i++)
	{
		if(str[i]==':'&&str[i-1]!='\\')
		{
			index1=i+2;
			break;
		}
	}
	//printf("%d\n",len);
	for(int i=index1+1;i<len;i++)
	{
		if(str[i]==','&&str[i-1]!='\\')
		{
			index2=i-2;
			//printf("sdsadas");
			break;
		}
	}
	char strtime[500];
	memcpy(strtime,str+index1,sizeof(char)*(index2-index1+1));
	strtime[index2-index1+1]='\0';
	struct tm tm_time;
	strptime(strtime, "%F %T", &tm_time);
	ans.Time = mktime(&tm_time);

	for(int i=index2+1;i<len;i++)
	{
		if(str[i]==':'&&str[i-1]!='\\')
		{
			index1=i+1;
			break;
		}
	}
	//printf("%c %c\n",*(str+index1),str[len-2]);
	ans.flag_lms=string_to_longlong(str+index1,str+len-2);
	//printf("%d\n",ans.flag_lms);

	/*for(int i=index2+1;i<len;i++)
	{
		if(str[i]==':'&&str[i-1]!='\\')
		{
			index1=i+2;
			break;
		}
	}
	ans.HasBeenReaded=string_to_longlong(str+index1,str+len-3);
	printf("%d\n",ans.HasBeenReaded);*/

	return ans;
}
/*
int main(int argc, char** argv) {
    unsigned char sms[1024] = "{\"sender\":\"123456789\",\"receiver\":\"789456123\",\"content\":\"hello\",\"Time\":\"2011-07-22 09:30:23\",\"flag_lms\":0}";
	string_to_message(sms);
    return 0;
}*/
