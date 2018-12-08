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

/*
reference struct

// This structure gets passed by the SIOCADDRT and SIOCDELRT calls.
struct rtentry 
{
        unsigned long   rt_hash;        // hash key for lookups         
        struct sockaddr rt_dst;         // target address               
        struct sockaddr rt_gateway;     // gateway addr (RTF_GATEWAY)   
        struct sockaddr rt_genmask;     // target network mask (IP)     
        short           rt_flags;
        short           rt_refcnt;
        unsigned long   rt_use;
        struct ifnet    *rt_ifp;
        short           rt_metric;      // +1 for binary compatibility! 
        char            *rt_dev;        // forcing the device at add    
        unsigned long   rt_mss;         // per route MTU/Window         
        unsigned long   rt_window;      // Window clamping              
        unsigned short  rt_irtt;        // Initial RTT                  
};


struct sockaddr_in {
     sa_family_t sin_family;             // AF_INET 
     in_port_t sin_port;                 // Port number.  
     struct in_addr sin_addr;            // Internet address.  

     // Pad to size of `struct sockaddr'.  
     unsigned char sin_zero[sizeof (struct sockaddr) -
                            sizeof (sa_family_t) -
                            sizeof (in_port_t) -
                            sizeof (struct in_addr)];
};
typedef uint32_t in_addr_t;
struct in_addr  {
    in_addr_t s_addr;                    // IPv4 address 
};



*/

/*
 * Add a routing table entry with ip & netmask with dev as the target.
 * Returns 0 if success, non zero for error.
 */

int route_add(const char *ip, const char *netmask, const char *gateway, char *device)
{
    struct rtentry rt;
    struct sockaddr_in *sinaddr;
    int inetfd;
    int error = 0;
    int ret = 0;

    //create socket
    inetfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inetfd < 0)
    {
        printf("in route_add::socket failed errno[%d]\n", errno);
        return errno;
    }
    memset(&rt, 0, sizeof(rt));
    error = 0;
    do
    {
        //set target ip
        sinaddr = (struct sockaddr_in *)(&rt.rt_dst);
		sinaddr->sin_family = AF_INET;
        ret = inet_pton(AF_INET, ip, &(sinaddr->sin_addr));
        if (ret != 1)
        {
            printf("in route_add::inet_pton ip[%s] failed errno[%d]\n", ip,errno);
            error = errno;
            break;
        }

        //set netmask
        sinaddr = (struct sockaddr_in *) (&rt.rt_genmask);
		sinaddr->sin_family = AF_INET;
        ret = inet_pton(AF_INET,netmask,&(sinaddr->sin_addr));
        if (ret != 1)
        {
            printf("in route_add::inet_pton netmask[%s] failed errno[%d]\n", netmask,errno);
            error = errno;
            break;
        }

        //set gateway
        sinaddr = (struct sockaddr_in *) (&rt.rt_gateway);
		sinaddr->sin_family = AF_INET;
        ret = inet_pton(AF_INET,gateway,&(sinaddr->sin_addr));
		rt.rt_flags |= RTF_GATEWAY;
        if (ret != 1)
        {
            printf("in route_add::inet_pton gateway[%s] failed errno[%d]\n", gateway,errno);
            error = errno;
            break;
        }

        //set device 
        if(device != NULL  && strlen(device)!=0)
            rt.rt_dev = device;

        if (ioctl(inetfd, SIOCADDRT, &rt) == -1)
		{
			printf("in route_add::ioctl failed\n");
            error = errno;
		}

    } while (0);
    close(inetfd);
    return error;
}


int route_del(const char *ip, const char *netmask,char *device)
{
    struct rtentry rt;
    struct sockaddr_in *sinaddr;
    int inetfd;
    int error = 0;
    int ret = 0;

    //create socket
    inetfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (inetfd < 0)
    {
        printf("in route_del::socket failed errno[%d]\n", errno);
        return errno;
    }
    memset(&rt, 0, sizeof(rt));
    error = 0;
    do
    {
        //set target ip
        sinaddr = (struct sockaddr_in *)(&rt.rt_dst);
		sinaddr->sin_family = AF_INET;
        ret = inet_pton(AF_INET, ip, &(sinaddr->sin_addr));
        if (ret != 1)
        {
            printf("in route_del::inet_pton ip[%s] failed errno[%d]\n", ip,errno);
            error = errno;
            break;
        }

        //set netmask
        sinaddr = (struct sockaddr_in *) (&rt.rt_genmask);
		sinaddr->sin_family = AF_INET;
        ret = inet_pton(AF_INET,netmask,&(sinaddr->sin_addr));
        if (ret != 1)
        {
            printf("in route_del::inet_pton netmask[%s] failed errno[%d]\n", netmask,errno);
            error = errno;
            break;
        }

		
        //set device 
        if(device != NULL  && strlen(device)!=0)
            rt.rt_dev = device;
	
		
        if (ioctl(inetfd, SIOCDELRT, &rt) == -1)
            error = errno;

    } while (0);
    close(inetfd);
    return error;
}


int main(int argc,char **argv)
{
    int ret = 0;
    if(argc<2)
    {
        printf("%s add|del ip netmask gateway dev\n",argv[0]);
        return -1;
    }
    char operation[32]={0};
    char ip[32]={0};
    char netmask[32]={0};
    char gateway[32]={0};
    char devname[32]={0};
    strcpy(operation,argv[1]);
    if(strcmp(operation,"add") ==0)
    {
        strcpy(ip,argv[2]);
        strcpy(netmask,argv[3]);
        strcpy(gateway,argv[4]);
        if(argc > 5)
            strcpy(devname,argv[5]);
    printf("add %s netmask %s gw %s %s\n",ip,netmask,gateway,devname);
    ret = route_add(ip,netmask,gateway,devname);
    if(ret)
    {
        perror("route_add failed \n");
    }else{
		printf("route_add ok\n");
	}
    }else if(strcmp(operation,"del")==0)
    {
        strcpy(ip,argv[2]);
        strcpy(netmask,argv[3]);
        if(argc > 4)
              strcpy(devname,argv[4]);
        printf("del %s netmask  %s %s\n",ip,netmask,devname);
        ret = route_del(ip,netmask,devname);
        if(ret)
        {
            perror("route_del failed \n");
        }else
		{
			printf("route_del ok\n");
		}
    }else{
        printf("err operation\n");
    }
    return 0;

}

