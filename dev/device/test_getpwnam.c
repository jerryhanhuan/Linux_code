#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>
/*
  struct passwd *getpwnam(const char *name); // 不可重入

  int getpwnam_r(const char *name, struct passwd *pwd,
                   char *buf, size_t buflen, struct passwd **result);// 可重入

 struct passwd {
               char   *pw_name;       // username 
               char   *pw_passwd;     // user password 
               uid_t   pw_uid;        // user ID 
               gid_t   pw_gid;        // group ID 
               char   *pw_gecos;      // user information 
               char   *pw_dir;        // home directory 
               char   *pw_shell;      // shell program 
           };




*/

int test_getpwnam(const char *uname)
{
    struct passwd *pw = NULL;
    if ((pw = getpwnam(uname)) == NULL)
    {
        perror("getpwnam failed\n");
        return -1;
    }
    printf("pw->pw_name = %s\n", pw->pw_name);
    printf("pw->pw_passwd = %s\n", pw->pw_passwd);
    printf("pw->pw_uid = %d\n", pw->pw_uid);
    printf("pw->pw_gid = %d\n", pw->pw_gid);
    printf("pw->pw_gecos = %s\n", pw->pw_gecos);
    printf("pw->pw_dir = %s\n", pw->pw_dir);
    printf("pw->pw_shell = %s\n", pw->pw_shell);
    return 0;
}

int test_getpwnam_r(const char *uname)
{
    struct passwd pw;
    struct passwd *result = NULL;
    int ret = 0;
    char buf[1024] = {0};
    ret = getpwnam_r(uname, &pw, buf, sizeof(buf), &result);
    if (result == NULL)
    {
        if (ret == 0)
        {
            printf("Not Found user[%s]\n", uname);
        }
        else
        {
            perror("getpwnam_r failed\n");
        }
        return -1;
    }
    printf("pw->pw_name = %s\n", pw.pw_name);
    printf("pw->pw_passwd = %s\n", pw.pw_passwd);
    printf("pw->pw_uid = %d\n", pw.pw_uid);
    printf("pw->pw_gid = %d\n", pw.pw_gid);
    printf("pw->pw_gecos = %s\n", pw.pw_gecos);
    printf("pw->pw_dir = %s\n", pw.pw_dir);
    printf("pw->pw_shell = %s\n", pw.pw_shell);
    return 0;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Usage uname\n");
        return -1;
    }
    char uname[256] = {0};
    strcpy(uname, argv[1]);
    test_getpwnam(uname);
    test_getpwnam_r(uname);
    return 0;
}
