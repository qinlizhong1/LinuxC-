//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o exec_test exec_test.c

/*
extern char **environ;

int execl(const char *path, const char *arg0, ... , (char *)0 );
int execle(const char *path, const char *arg0, ... , (char *)0, char *const envp[] );
int execlp(const char *file, const char *arg0, ... , (char *)0 );
int execv(const char *path, char *const argv[]);
int execvp(const char *file, char *const argv[]);
int execve(const char *file, const char *search_path, char *const argv[]);
 注意：p 代表 path，表示如果你指定的文件不包含路径，就从 path 环境变量指定的路径中搜索文件。
      也就是说不带p的4个函数需要带上命令的路径
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    if (-1 == execlp("ls", "ls", "/", (char *)0))
    {
        perror("execl error");
        exit(-1);
    }

    return 0;
}
