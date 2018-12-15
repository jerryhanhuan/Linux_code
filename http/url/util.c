
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <arpa/inet.h>

#include <netdb.h>

/*
struct hostent { 
    char *h_name; //主机名，即官方域名 
    char **h_aliases; //主机所有别名构成的字符串数组，同一IP可绑定多个域名 
    int h_addrtype; //主机IP地址的类型，例如IPV4（AF_INET）还是IPV6 
    int h_length; //主机IP地址长度，IPV4地址为4，IPV6地址则为16 
    char **h_addr_list; // 主机的ip地址，以网络字节序存储。若要打印出这个IP，需要调用inet_ntoa()。
    };


*/



void UnionGetIpAddr(char *domain, char *ip_addr)
{
    char ip[65]={0};
	 int i = 0;
    /*通过域名得到相应的ip地址*/
    struct hostent *host = gethostbyname(domain);
    if (!host)
    {
        ip_addr = NULL;
        return;
    }
    for (i = 0; host->h_addr_list[i]; i++)
    {
        inet_ntop(AF_INET, host->h_addr_list[i],ip,sizeof(ip));
        strcpy(ip_addr,ip);
       // strcpy(ip_addr, inet_ntoa( * (struct in_addr*) host->h_addr_list[i]));
        break;
    }
}



void UnionParseUrl(const char *url, char *domain, int *port)
{
    /*通过url解析出域名, 端口, 以及文件名*/
    int j = 0;
    int i = 0;
    int start = 0;
    *port = 80;
    char *patterns[] = {"http://", "https://", NULL};

    for (i = 0; patterns[i]; i++)
        if (strncmp(url, patterns[i], strlen(patterns[i])) == 0)
            start = strlen(patterns[i]);

    //解析域名, 这里处理时域名后面的端口号会保留
    for (i = start; url[i] != '/' && url[i] != '\0'; i++, j++)
        domain[j] = url[i];
    domain[j] = '\0';

    //解析端口号, 如果没有, 那么设置端口为80
    char *pos = strstr(domain, ":");
    if (pos)
        sscanf(pos, ":%d", port);

    //删除域名端口号
    for (i = 0; i < (int)strlen(domain); i++)
    {
        if (domain[i] == ':')
        {
            domain[i] = '\0';
            break;
        }
    }


}


int UniongetIpPortFromUrl(const char *url,char *ip,int *port)
{
    char domain[1024]={0};
    UnionParseUrl(url,domain,port);
    UnionGetIpAddr(domain,ip);
    return 0;
}




int main(int argc, char **argv)
{
    if(argc < 2)
    {
        printf("Usage url\n");
        return -1;
    }
    char ip[32]={0};
    int port = 0;
    char url[1024]={0};
    strcpy(url,argv[1]);
    printf("url:%s\n",url);
    UniongetIpPortFromUrl(url,ip,&port);
    printf("ip:%s port %d\n",ip,port);
    return 0;
}