#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/route.h>
#include <dirent.h>
#include <sys/types.h>
#include <ifaddrs.h>

/*IPV4 start*/

/*
    function: check ipv4_addr is valid?
    parameter:
    ipv4_addr[in]:ipv4 address
    return:
        1 - ipv4_addr is a valid ipv4 address
        0 - ipv4_addr is not a valid ipv4 address
*/

int IsValidipv4(const char *ipv4_addr)
{

    int ret = 0;
    struct in_addr ip_addr;
    if(ipv4_addr == NULL)
    {
        printf("in IsValidipv4::ip is NULL\n");
        return -1;
    }
    ret =inet_pton(AF_INET,ipv4_addr,(void*)&ip_addr);
    if(ret == 1)
        return 1;
    else
        return 0;
}

int IsIPv4InSubnet(const char *ip1,const char *netmask1,const char *ip2,const char* netmask2)
{
    if(ip1 == NULL || netmask1 == NULL || ip2 == NULL || netmask2 == NULL )
    {
        printf("in IsIPv6InSubnet::parameter err\n");
        return -1;
    }
    struct in_addr l_ip1={0};
    struct in_addr l_ip2={0};
    struct in_addr l_mask1={0};
    struct in_addr l_mask2={0};

    inet_pton(AF_INET,ip1,(void*)&l_ip1);
    inet_pton(AF_INET,ip2,(void*)&l_ip2);
    inet_pton(AF_INET,netmask1,(void*)&l_mask1);
    inet_pton(AF_INET,netmask2,(void*)&l_mask2);
    return (l_ip.s_addr1&l_mask1.s_addr) == l_ip2.s_addr1&l_mask2.s_addr1 ;
}




/*
    function: ifup a ipv4 address
    parameter:
    device[in]:eth name, eth0 eg
    ipaddr[in]:ip
    ipmask[in]:mask
    return:
        0 - ifup ip and address success
        !=0 - ifup failed
        
*/

int	ifup(const char * device, const char * ipaddr, const char * ipmask)
{
    struct	ifreq		ifr;
    struct	sockaddr_in *	p;
    struct	in_addr		dest;
    struct	in_addr		mask;

    int	inetfd;
    int	error;

//	TODO: error check ?
    strncpy(ifr.ifr_name, device, sizeof(ifr.ifr_name));
    if (!inet_aton(ipaddr, &dest) || !inet_aton(ipmask, &mask))
        return EINVAL;

    inetfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inetfd < 0)
        return errno;

    p = (struct sockaddr_in *)&ifr.ifr_addr;
    p->sin_family = AF_INET;
    p->sin_port = 0;
    error = 0;
    do
    {
        p->sin_addr = dest;
        if (ioctl(inetfd, SIOCSIFADDR, &ifr) == -1)//先设置IP，再设置掩码
        {
            error = errno;
            break;
        }

        p->sin_addr = mask;
        if (ioctl(inetfd, SIOCSIFNETMASK, &ifr) == -1)
        {
            error = errno;
            break;
        }

        if (ioctl(inetfd, SIOCGIFFLAGS, &ifr) == -1)
        {
            error = errno;
            break;
        }

        ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
        if (ioctl(inetfd, SIOCSIFFLAGS, &ifr) == -1)
            error = errno;
    }
    while (0);

    close(inetfd);
    return error;
}

/*

  function: ifdown a eth device
    parameter:
    device[in]:eth name, eth0 eg
    return:
        0 - ifdown success
        !=0 - ifdown failed
*/

int	ifdown(const char * device)
{
    struct	ifreq	ifr;
    int	inetfd;
    int	error;

    strncpy(ifr.ifr_name, device, sizeof(ifr.ifr_name));
    inetfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inetfd < 0)
        return errno;

    error = 0;
    do
    {
        if (ioctl(inetfd, SIOCGIFFLAGS, &ifr) == -1)
        {
            error = errno;
            break;
        }

        //ifr.ifr_flags &= ~(IFF_UP | IFF_RUNNING);
        ifr.ifr_flags &= ~(IFF_RUNNING);
        if (ioctl(inetfd, SIOCSIFFLAGS, &ifr) == -1)
            error = errno;
    }
    while (0);

    close(inetfd);
    return error;
}


/*
    function: add default gateway
    parameter:
    gwaddr[in]:gateway
    return:
        0 - add_default_gw success
        !=0 - add_default_gw failed
*/
int	add_default_gw(const char * gwaddr)
{
    struct	rtentry	rt;
    int	inetfd;
    int	error = 0;



    memset(&rt, 0, sizeof(rt));
    rt.rt_dst.sa_family = AF_INET;
//	((struct sockaddr_in *)&rt.rt_dst)->sin_addr = INADDR_ANY;

    rt.rt_gateway.sa_family = AF_INET;
    if (!inet_aton(gwaddr, &((struct sockaddr_in *)&rt.rt_gateway)->sin_addr))
        return EINVAL;

    rt.rt_flags = RTF_UP | RTF_GATEWAY;

    inetfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inetfd < 0)
        return errno;

    error = (ioctl(inetfd, SIOCADDRT, &rt) == -1) ? errno : 0;
    close(inetfd);
    return error;
}

/*
    function: delete default gateway
    parameter: null
    return:
        0 - del_default_gw success
        !=0 - del_default_gw failed
*/
int	del_default_gw()
{
    struct	rtentry	rt;
    int	inetfd;
    int	error;

    memset(&rt, 0, sizeof(rt));
    rt.rt_dst.sa_family = AF_INET;
    rt.rt_gateway.sa_family = AF_INET;
//	((struct sockaddr_in *)&rt.rt_dst)->sin_addr = INADDR_ANY;
    rt.rt_flags = RTF_UP;

    inetfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inetfd < 0)
        return errno;

    error = (ioctl(inetfd, SIOCDELRT, &rt) == -1) ? errno : 0;
    close(inetfd);
    return error;

}


/*
    function: get ip and mask by device
    parameter:
    device[in]:eth name,eth0 eg
    ip[out]:device ip
    netmask[out]:device mask
    return:
        0 - getipaddr success
        !=0 - getipaddr failed
*/
int getipaddr(const char *device,char *ip,char *netmask)
{
    if(device == NULL || ip == NULL || netmask == NULL)
    {
        printf("in getipaddr::parameter err\n");
        return -1;
    }
    int ret = 0;
    struct ifaddrs * ifap = NULL;
    struct ifaddrs * ifa = NULL;
    char buf[65]={0};

    if((ret = getifaddrs(&ifap))<0)
    {
        printf("in getipaddr::getifaddrs failed errno[%d]\n",errno);
        return -1;
    }
    for(ifa =ifap;ifa!=NULL;ifa=ifa->ifa_next)
    {
        if(ifa->ifa_name && strcmp(ifa->ifa_name,device)== 0&& ifa->ifa_addr->sa_family == AF_INET)
        {
            struct sockaddr_in * ptr = (struct sockaddr_in*)ifa->ifa_addr;
            inet_ntop(AF_INET,&(ptr->sin_addr),buf,sizeof(buf));
            strcpy(ip,buf);
            ptr = (struct sockaddr_in*)ifa->ifa_netmask;
            memset(buf,0,sizeof(buf));
            inet_ntop(AF_INET,&(ptr->sin_addr),buf,sizeof(buf));
            strcpy(netmask,buf);
            break;
        }
    }
    freeifaddrs(ifap);
    return 0;
}


/*
    function: get default gw
    parameter:
    gateway[out]:eth name,eth0 eg
    return:
        0 - get_gateway success
        !=0 - get_gateway failed
*/
int get_gateway(char* gateway)
{
    FILE *fp;
    char buf[256]; // 128 is enough for linux
    char iface[16];
    char tmpgateway[32]={0};
    unsigned long dest_addr, gate_addr,p;
    p = INADDR_NONE;
    fp = fopen("/proc/net/route", "r");
    if (fp == NULL)
    {
        printf("can not open %s\n","/proc/net/route");
        return -1;
        }
    /* Skip title line */
    fgets(buf, sizeof(buf), fp);
    while (fgets(buf, sizeof(buf), fp)) {

        if (sscanf(buf, "%s %lX %lX", iface,&dest_addr, &gate_addr) == 3 && gate_addr!= 0)
        {
            p = gate_addr;
            break;
        }
    }
    fclose(fp);
    inet_ntop (AF_INET, &p, tmpgateway,sizeof(tmpgateway));
    memcpy(gateway,tmpgateway,strlen(tmpgateway));
    return 0;
}



//return value negetive: failed  0: not running 1 :active

int Check_IPvInterface_active(const char *device,int sock_type)
{
    //SIOCGIFFLAGS, SIOCSIFFLAGS 读取 或 设置 设备的 活动标志字
    struct	ifreq	ifr;
    int	inetfd;
    int	error;
    if(sock_type != AF_INET && sock_type != AF_INET6)
    {
        printf("in Check_IPvInterface_active::wrong sock_type[%d]\n",sock_type);
        return -1;
    }
    strncpy(ifr.ifr_name, device, sizeof(ifr.ifr_name));
    inetfd = socket(sock_type, SOCK_DGRAM, 0);
    if (inetfd < 0)
        return errno;

    error = 0;
    do
    {
        if (ioctl(inetfd, SIOCGIFFLAGS, &ifr) == -1)
        {
            error = errno;
            break;
        }
        error = 0;
    }
    while (0);
    close(inetfd);
    if(!error)
        error =((ifr.ifr_flags & IFF_UP) && (ifr.ifr_flags & IFF_RUNNING));
    return error;
}



/*IPV4 end*/


/*   IPV6 start   */

struct in6_ifreq {
    struct in6_addr addr;
    uint32_t        prefixlen;
    unsigned int    ifindex;
};



int IsValidipv6(const char *ipv6_addr)
{

    int ret = 0;
    struct in6_addr addr6 = {0};;
    char str[INET6_ADDRSTRLEN];
    if(ipv6_addr == NULL)
    {
        printf("in IsValidipv6::ip is NULL\n");
        return -1;
    }
    ret =inet_pton(AF_INET6,ipv6_addr,(void*)&addr6);
    if(ret == 1)
        return 1;
    else
        return 0;
}


int CompareIPV6(const char *ip1,const char *ip2)
{
    if(ip1 == NULL || ip2 == NULL)
    {
        printf("in CompareIPV6::parameter err \n");
        return -1;
    }
    struct in6_addr addr1;
    struct in6_addr addr2;
    int ret = 0;
    memset(&addr1,0,sizeof(addr1));
    memset(&addr2,0,sizeof(addr2));
    ret = inet_pton(AF_INET6,ip1,(void*)&addr1);
    if(ret!=1)
    {
        printf("in CompareIPV6::inet_pton ip1[%s] failed errno[%d]\n",ip1,errno);
        return -1;
    }
    ret = inet_pton(AF_INET6,ip2,(void*)&addr2);
    if(ret!=1)
    {
        printf("in CompareIPV6::inet_pton ip2[%s] failed errno[%d]\n",ip2,errno);
        return -1;
    }
    if(!memcmp(&addr1,&addr2,sizeof(struct in6_addr)))//equal
    {
        return 0;
    }else{ //not equal
        return 1;
    }

}




int ifup_ipv6(const char * device,const char *ipv6_addr, int prefixlen)
{
    int ret = 0;
    int sockfd = 0;
    struct in6_ifreq ifr6;
    struct ifreq ifr;
    struct sockaddr_in6 addr;
    int error = 0;

    //parameter check
    if(ipv6_addr == NULL || prefixlen <0 || prefixlen > 128)
    {
        printf("in ifup_ipv6::ipv6_addr is NULL or prefixlen[%d] not legal\n",prefixlen);
        return -1;
    }
    if(IsValidipv6(ipv6_addr)<=0)
    {
        printf("in ifup_ipv6::ipv6_addr[%s]not legal\n",ipv6_addr);
        return -1;
    }

    // Create IPv6 socket to perform the ioctl operations on
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd <0) {
       printf("in ifup_ipv6::Bad fd errno[%d]\n",errno);
       return -1;
    }

    do{
         // Copy the interface name to the ifreq struct
        strncpy(ifr.ifr_name, device, sizeof(ifr.ifr_name));
        // Get the ifrindex of the interface
        if (ioctl(sockfd, SIOGIFINDEX, &ifr) < 0) {
                printf("in ifup_ipv6::SIOGIFINDEX errno[%d]\n",errno);
                error = -1;
                break;
            }
        // Prepare the in6_ifreq struct and set the address to the interface
        inet_pton(AF_INET6, ipv6_addr, &ifr6.addr);
        ifr6.ifindex = ifr.ifr_ifindex;
        ifr6.prefixlen = prefixlen;
        if(ioctl(sockfd, SIOCSIFADDR, &ifr6)<0)
        {
            printf("in ifup_ipv6::ioctl SIOCSIFADDR failed errno[%d]\n",errno);
            error = -1;
            break;
        }

        //get dev flag
        //if(ioctl(sockfd,SIOCGIFFLAGS,&ifr))


        ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
        if(ioctl(sockfd, SIOCSIFFLAGS, &ifr)<0)
        {
            printf("in ifup_ipv6::ioctl SIOCSIFFLAGS failed errno[%d]\n",errno);
            error = -1;
            break;
        }
        error = 0;
    }while(0);

    close(sockfd);
    return error;
}

int ifdown_ipv6(const char * device)
{
    int ret = 0;
    struct	ifreq	ifr;
	struct in6_ifreq ifr6;
    int sockfd = 0;
    int error = 0;
    if(device == NULL)
    {
        printf("in ifdown_ipv6::device is NULL\n");
        return -1;
    }
    // Create IPv6 socket to perform the ioctl operations on
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd <0) {
       printf("in ifdown_ipv6::Bad fd errno[%d]\n",errno);
       return -1;
    }

    do{
        // Copy the interface name to the ifreq struct
        strncpy(ifr.ifr_name, device, sizeof(ifr.ifr_name));
        if (ioctl(sockfd, SIOCGIFFLAGS, &ifr) == -1)
        {
            printf("in ifdown_ipv6::ioctl SIOCGIFFLAGS failed errno[%d]\n",errno);
            error = -1;
            break;
        }
        //ifr.ifr_flags &= ~(IFF_UP | IFF_RUNNING);
        ifr.ifr_flags &= ~(IFF_RUNNING);
        if(ioctl(sockfd,SIOCSIFFLAGS,&ifr)<0)
        {
            printf("in ifdown_ipv6::ioctl SIOCSIFFLAGS failed errno[%d]\n",errno);
            error = -1;
            break;
        }
        error = 0;
    }while(0);


    close(sockfd);
    return error;
}

int add_default_gw_ipv6(const char * gwaddr)
{
    if(gwaddr == NULL)
    {
        printf("in add_default_gw_ipv6::gwaddr is NULL\n");
        return -1;
    }
    if(IsValidipv6(gwaddr)<=0)
    {
        printf("in add_default_gw_ipv6::gwaddr[%s]not legal\n",gwaddr);
        return -1;
    }
    int ret = 0;
    struct	in6_rtmsg	rt;
    int sockfd = 0;
    int error=0;
    memset(&rt, 0, sizeof(rt));
    // Prepare the in6_rtmsg struct and set the address to the interface
    inet_pton(AF_INET6, gwaddr, (void*)&rt.rtmsg_gateway);
    rt.rtmsg_flags = RTF_UP | RTF_GATEWAY;

    // Create IPv6 socket to perform the ioctl operations on
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd <0) {
       printf("in add_default_gw_ipv6::Bad fd errno[%d]\n",errno);
       return -1;
    }
    do{
        if(ioctl(sockfd, SIOCADDRT, &rt)<0)
        {
            printf("in add_default_gw_ipv6::ioctl SIOCADDRT failed errno[%d]\n",errno);
            error = -1;
            break;
        }
        error = 0;
    }while(0);
    close(sockfd);
    return error;
}

int delete_default_gw_ipv6()
{
    int ret = 0;
    int sockfd = 0;
    struct	in6_rtmsg	rt;
    int error = 0;
    memset(&rt, 0, sizeof(rt));

    // Create IPv6 socket to perform the ioctl operations on
    sockfd = socket(AF_INET6, SOCK_DGRAM, 0);
    if (sockfd <0) {
       printf("in delete_default_gw_ipv6::Bad fd errno[%d]\n",errno);
       return -1;
    }
    do{

        rt.rtmsg_flags  =RTF_UP;
        if(ioctl(sockfd, SIOCDELRT, &rt)<0)
        {
            printf("in delete_default_gw_ipv6::ioctl SIOCDELRT failed errno[%d]\n",errno);
            error = -1;
            break;
        }
        error = 0;
    }while(0);

    close(sockfd);
    return error;
}


int cale_prefix_len(struct in6_addr *mask)
{
    int i = 0;
    int len = 0;
    int u = 0;

    for(i = 0; i < 16; i++)
    {
        u = mask->s6_addr[i];
        if(ffs(u) > 0)
        {
            len += 9 - ffs(u);
        }
        else
        {
            break;
        }
    }

    return len;
}


int getipv6addr(const char * device,char *ipv6_addr,int *prefixlen)
{

    if(device == NULL || ipv6_addr == NULL || prefixlen == NULL)
    {
        printf("in getipv6addr::parameter err \n");
        return -1;
    }
    int ret = 0;
    int sockfd = 0;
    struct ifaddrs *ifap, *ifa;
    struct sockaddr_in6 *sa;
    int len = 0;
    char addr[128]={0};
    if (getifaddrs(&ifap) == -1) {
        printf("in getipv6addr::getifaddrs failed errno[%d]\n",errno);
        return -1;
    }
    for(ifa = ifap;ifa!=NULL;ifa = ifa->ifa_next)
    {
         if (ifa->ifa_name && strcmp(ifa->ifa_name,device)== 0 && ifa->ifa_addr->sa_family==AF_INET6)
         {
                 sa = (struct sockaddr_in6 *) ifa->ifa_addr;
                 inet_ntop(AF_INET6, &sa->sin6_addr, addr, sizeof(addr));
                 len = cale_prefix_len(&(((struct sockaddr_in6*)(ifa->ifa_netmask))->sin6_addr));
                 break;
         }
    }
    freeifaddrs(ifap);
    strcpy(ipv6_addr,addr);
    *prefixlen =len;
    return 0;
}


int getipv6_default_gw(char *ipv6_default_gw)
{
    if(ipv6_default_gw == NULL)
    {
        printf("in getipv6_default_gw::parameter is err \n");
        return -1;
    }
    int ret = 0;
    char *file = "/proc/net/ipv6_route";
    FILE *fp = NULL;
    char buf[1024]={0};
    //IPv6 destination network displayed in 32 hexadecimal chars without colons as separator
    char dst[64];
    //IPv6 destination prefix length in hexadecimal
    int  dst_prefix_len = 0;
    //IPv6 source network displayed in 32 hexadecimal chars without colons as separator
    char src[64]={0};
    //IPv6 source prefix length in hexadecimal
    int src_prefix_len = 0;
    //IPv6 next hop displayed in 32 hexadecimal chars without colons as separator
    char gw[64]={0};
    //Metric in hexadecimal
    unsigned int metrix;
    //Reference counter
    unsigned int ref_cnt;
    //Use counter
    unsigned int usr_cnt;
    //Flags
    unsigned int flag;
    //Device name
    char devname[32];
    struct in6_addr gw_addr;
    int status = 0;
    fp = fopen(file,"rb");
    if(fp == NULL)
    {
        printf("in getipv6_default_gw::fopen [%s] failed errno[%d]\n",file,errno);
        return -1;
    }
    while(fgets(buf,sizeof(buf),fp))
    {

            sscanf(buf,"%s %x %s %x %s %x %x %x %x %s",dst, &dst_prefix_len, src, &src_prefix_len, gw,
                   &metrix, &ref_cnt, &usr_cnt, &flag, devname);
            if(memcmp(gw,"00000000000000000000000000000000",32))
            {
                sprintf(gw, "%c%c%c%c:%c%c%c%c:%c%c%c%c:%c%c%c%c:%c%c%c%c:%c%c%c%c:%c%c%c%c:%c%c%c%c",
                                gw[0], gw[1], gw[2], gw[3], gw[4], gw[5], gw[6], gw[7], gw[8], gw[9],
                                gw[10], gw[11], gw[12], gw[13], gw[14], gw[15], gw[16], gw[17], gw[18], gw[19],
                                gw[20], gw[21], gw[22], gw[23], gw[24], gw[25], gw[26], gw[27], gw[28], gw[29],
                                gw[30], gw[31]);
                memset(&gw_addr,0,sizeof(gw_addr));
                inet_pton(AF_INET6, gw, &gw_addr);
                memset(gw,0,sizeof(gw));
                inet_ntop(AF_INET6, &gw_addr, gw, sizeof(gw));
                strcpy(ipv6_default_gw,gw);
                status = 1;
                break;
            }
            memset(dst,0,sizeof(dst));
            memset(src,0,sizeof(src));
            memset(gw,0,sizeof(gw));
            memset(devname,0,sizeof(devname));
            dst_prefix_len = 0;
            src_prefix_len = 0;
            metrix = 0;
            ref_cnt = 0;
            usr_cnt = 0;
            flag = 0;
    }
    fclose(fp);
    fp = NULL;
    return status;
}



void getipv6netmask(struct in6_addr* netmask,int prefixlen)
{
    uint32_t *pnetmask = NULL;
    if(prefixlen<0)
    {
        prefixlen = 0;
    }else if(prefixlen>128){
        prefixlen = 128;
    }
    pnetmask = (uint32_t*)&netmask[0];
    int left = 0;
    left = prefixlen;
    while(left>32)
    {
        *pnetmask = 0XFFFFFFFF;
        pnetmask++;
        left-=32;
    }
    if(left!=0)
    {
        *pnetmask = htonl((0XFFFFFFFF << (32-left)));
    }
}

int get_ipv6_prefix(char *prefix,int prefixlen)
{
    int ret  = 0;
    char buf[256]={0};
    struct in6_addr mask;
    getipv6netmask(&mask,prefixlen);
    inet_ntop(AF_INET6,&mask,buf,sizeof(buf));
    strcpy(prefix,buf);
    return 0;
}



/*
struct in6_addr {
    unsigned char   s6_addr[16];   // IPv6 address 
};
*/

int IsIPv6InSubnet(char *ip_1,int prefix_len_1,char *ip_2,int prefix_len_2)
{
    struct in6_addr ip1;
    struct in6_addr ip2;
    struct in6_addr ip_netmask_1;
    struct in6_addr ip_netmask_2;
    struct in6_addr result_1;
    struct in6_addr result_2;
    struct in6_addr result_p1;
    struct in6_addr result_p2;
    int  i = 0;
    getipv6netmask(&ip_netmask_1,prefix_len_1);
    getipv6netmask(&ip_netmask_2,prefix_len_2);
    inet_pton(AF_INET6,ip_1,&ip1);
    inet_pton(AF_INET6,ip_2,&ip2);

    for (i=0;i<16;i++)
    {
        result_p1.s6_addr[i] = ip1.s6_addr[i]&ip_netmask_1.s6_addr[i];
        result_p2.s6_addr[i] = ip2.s6_addr[i]&ip_netmask_2.s6_addr[i];
    }
    return memcmp(&result_p1,&result_p2,sizeof(struct in6_addr));
}



/*   IPV6 END*/


int main(int argc,char **argv)
{

    if(argc<5)
    {
        printf("Usage::ip1 netmask1 ip2 netmask2 \n");
        return -1;
    }
    char ip1[65]={0};
    char netmask1[65]={0};
    char ip2[65]={0};
    char netmask2[65]={0};
    strcpy(ip1,argv[1]);
    strcpy(netmask1,argv[2]);
    strcpy(ip2,argv[3]);
    strcpy(netmask2,argv[4]);

    if((ret = IsIPv4InSubnet(ip1,netmask1,ip2,netmask2)) == 0)
    {
        printf("in the same subnet\n");
    }else{
        printf("not in the same subnet\n");
    }
    return 0;
}