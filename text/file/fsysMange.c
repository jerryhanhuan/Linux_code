#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>
#include <sys/stat.h>
#include <stdarg.h>

/*
struct dirent {
               ino_t          d_ino;       // inode number 
               off_t          d_off;       // not an offset; see NOTES 
               unsigned short d_reclen;    // length of this record 
               unsigned char  d_type;      // type of file; not supported  by all filesystem types 
               char           d_name[256]; // filename 
           };

d_type:
DT_BLK      This is a block device.
DT_CHR      This is a character device.
DT_DIR      This is a directory.
DT_FIFO     This is a named pipe (FIFO).
DT_LNK      This is a symbolic link.
DT_REG      This is a regular file.
DT_SOCK     This is a UNIX domain socket.
DT_UNKNOWN  The file type is unknown.


*/



char	pgUnionInputStr[8192+1];
int IsQuit(char *p)
{
	ToUpperCase(p);
	if ((strcasecmp (p,"QUIT") == 0) || (strcasecmp (p,"EXIT") == 0))
		return(1);
	else
		return(0);
}


int ToUpperCase(char *str)
{
	int	i = 0;

	while (str[i])
	{
		if ((str[i] >= 'a') && (str[i] <= 'z'))
			str[i] = str[i] + 'A' - 'a';
		i++;
	}
	return(0);
}

char *Input(const char *fmt,...)
{
	va_list args;
	int	i;

	va_start(args,fmt);
	vprintf(fmt,args);
	va_end(args);

	for (i = 0; i < sizeof(pgUnionInputStr)-1;)
	{
		pgUnionInputStr[i] = getchar();
		if ((pgUnionInputStr[i] == 10) || (pgUnionInputStr[i] == 13) || (pgUnionInputStr[i] == '\n'))
		{
			if (i == 0)
				continue;
			else
				break;
		}
		else
			i++;
	}
	pgUnionInputStr[i] = 0;
	return(pgUnionInputStr);
}


/*
    功能: 获取目录下所有的子目录路径(绝对路径，只包含子目录，不包含普通文件)
    函数名:GetSubDirNum
    参数：
    输入:
        rootdir  parent directory
    输出:
        subdirName[][128] child directory
    返回:
        child directory number
*/
int GetSubDirNum(char *rootdir,char subdirName[][128])
{
    int ret = 0;
    DIR *dir = NULL;
    struct dirent *pStResult = NULL;
    struct dirent *pStEntry = NULL; 
    int count = 0;
    if(access(rootdir,F_OK)!=0)
    {
        printf("in GetSubDirNum::can not access rootdir[%s] errno[%d]\n",rootdir,errno);
        return -1;
    }
    dir = opendir(rootdir);
    if(!dir)
    {
        printf("in GetSubDirNum::opendir rootdir[%s] errno[%d]\n",rootdir,errno);
        return -2;
    }
    pStEntry = malloc(sizeof(struct dirent)*1);
    while(!readdir_r(dir,pStEntry,&pStResult) && pStResult!=NULL)
    {
        if(pStEntry->d_name[0] == '.' || pStEntry->d_name[0] == '\\') //忽略 . ..
            continue;
        if(pStEntry->d_type == DT_DIR)
        {
            strcpy(subdirName[count],pStEntry->d_name);
            count++;
        }
    }   
    free(pStEntry);
    closedir(dir);
    return count;
}


/*
    功能: 获取目录下所有的文件名(绝对路径)
    函数名:GetSubDirNum
    参数：
    输入:
        dirPath  parent directory
    输出:
        fileName[][128] child file
    返回:
        child file number
*/
int GetDirFileNum(char *dirPath,char fileName[][128])
{
    int ret = 0;
    DIR *dir = NULL;
    struct dirent *pStResult = NULL;
    struct dirent *pStEntry = NULL; 
    int count = 0;
    if(access(dirPath,F_OK)!=0)
    {
        printf("in GetDirFileNum::can not access dirPath[%s] errno[%d]\n",dirPath,errno);
        return -1;
    }
    dir = opendir(dirPath);
    if(!dir)
    {
        printf("in GetDirFileNum::opendir dirPath[%s] errno[%d]\n",dirPath,errno);
        return -2;
    }
    pStEntry = malloc(sizeof(struct dirent)*1);
    while(!readdir_r(dir,pStEntry,&pStResult) && pStResult!=NULL)
    {
        if(pStEntry->d_type == DT_REG)
        {
            strcpy(fileName[count],pStEntry->d_name);
            count++;
        }
    }   
    free(pStEntry);
    closedir(dir);
    return count;
}

int GetDir(char *dirPath)
{
    int ret = 0;
    if(access(dirPath,F_OK)!=0)
    {
        if(mkdir(logDir,S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH) < 0)
        {
            printf("in GetDir::mkdir logDir[%s] errno[%d]\n",logDir,errno);
            return -1;
        }		
    }
    return 0；
}



int test_GetSubDirNum()
{

    int ret = 0;
    char root_dir_path[256]={0};
    char subdirName[1024][128]={0};
    int dirnum = 0;
    char *ptr = NULL;
    ptr = Input("Please input parent directory path::");
    strcpy(root_dir_path,ptr);
    if((dirnum = GetSubDirNum(root_dir_path,subdirName))<0)
    {
        printf("GetSubDirNum failed ret[%d]\n",dirnum);
        return dirnum;
    }
    printf("sub directory num is [%d]\n",dirnum);
    int i = 0;
    
    for( i = 0; i < dirnum; i++)
    {
        printf("%s\n",subdirName[i]);
    }
    return 0;
}

int test_GetDirFileNum()
{
    int ret = 0;
    char root_dir_path[256]={0};
    char subdirName[1024][128]={0};
    int dirnum = 0;
    char *ptr = NULL;
    ptr = Input("Please input  directory path::");
    strcpy(root_dir_path,ptr);
    if((dirnum = GetDirFileNum(root_dir_path,subdirName))<0)
    {
        printf("GetDirFileNum failed ret[%d]\n",dirnum);
        return dirnum;
    }
    printf("file num is [%d]\n",dirnum);
    int i = 0;
    
    for( i = 0; i < dirnum; i++)
    {
        printf("%s\n",subdirName[i]);
    }
    return 0;
}

int test_GetDir()
{
    int ret = 0;
    char root_dir_path[256]={0};
    char subdirName[1024][128]={0};
    int dirnum = 0;
    char *ptr = NULL;
    ptr = Input("Please input  directory path::");
    strcpy(root_dir_path,ptr);
    if((ret = GetDir(root_dir_path))<0)
    {
        printf("GetDir failed \n");
        return -1;
    }
    printf("get dir [%s] ok \n",root_dir_path);
    return 0;
}
 




int main(int argc,char **argv)
{
    int ret = 0;
	char choice[128]={0};
	loop:
#ifdef WIN32
	ret = system("cls");
#else
	ret = system("clear");
#endif
    printf("fsysMange test::\n");
	printf("01		Get directory sub directory\n");
	printf("02		Get directory subdirectory\n");
    printf("03      GetDir \n");
	printf("Exit	exit\n");
	printf("\n");

	printf("please select choice::");
	
#ifdef  WIN32
	ret = scanf("%s",choice);
#else
	ret = scanf("%s",choice);
#endif
	
	if ((strcmp(choice,"EXIT") == 0) || (strcmp("QUIT",choice) == 0))
		return(0);
    int c = atoi(choice);
	switch (c)
	{
	case 1:
		test_GetSubDirNum();
		break;
	case 2:
		test_GetDirFileNum();
		break;
	case 3:
        test_GetDir();
        break;
	default:
		printf("not support the choice\n");
		break;
	}

	printf("please enter... ...");
	getchar();
	getchar();
	goto loop;

}






