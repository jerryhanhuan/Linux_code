#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>


//获取目录下子目录数目
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


//获取目录下文件数目
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


 



int main(int argc,char **argv)
{
    if(argc < 2)
    {
        printf("Usage::%s pathdir\n",argv[0]);
        return -1;
    }
    char path[256]={0};
    strcpy(path,argv[1]);

    char subdir[1024][128]={0};
    int num = GetSubDirNum(path,subdir);
    printf("num is %d\n",num);
    int i = 0;
    for(i=0;i<num;i++)
    {
        printf("%s\n",subdir[i]);
    }
    return 0;
}






