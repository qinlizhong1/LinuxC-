//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o fork_test fork_test.c

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int g_var = 100;

//没有修改变量值，父子进程变量地址应该一样
void test0()
{
    int l_var = 66;
    pid_t  pid;

    pid = fork();
    if (pid <= -1)
    {
        perror("fork error");
        exit(-1);
    }else if (0 == pid) //子进程
    {
        printf("child process:%ld--->&g_var = %p, &l_var = %p\n", \
            (long)getpid(), &g_var, &l_var);
    }else  //父进程
    {
        printf("parent process:%ld--->&g_var = %p, &l_var = %p\n", \
            (long)getpid(), &g_var, &l_var);
    }
}

//修改变量值，父子进程变量地址应该一样(因为是虚拟地址)，但是物理地址不一样了，读取变量值不同可以验证
void test1()
{
    int l_var = 66;
    pid_t  pid;

    pid = fork();
    if (pid <= -1)
    {
        perror("fork error");
        exit(-1);
    }else if (0 == pid) //子进程对两个变量自增1
    {
        g_var++;
        l_var++;
        printf("child process:%ld--->&g_var = %p, &l_var = %p, g_var = %d, l_var = %d\n", \
            (long)getpid(), &g_var, &l_var, g_var, l_var);
    }else  //父进程
    {
        printf("parent process:%ld--->&g_var = %p, &l_var = %p, g_var = %d, l_var = %d\n", \
            (long)getpid(), &g_var, &l_var, g_var, l_var);
    }
}

//验证父子进程共享文件表项目
void test2()
{
    int fd = open("test.txt", O_WRONLY | O_CREAT, 0664);
    if (fd == -1) {
        perror("open");
        exit(-1);
    }

    pid_t pid = fork();
    if (pid > 0) {
        sleep(3);
        printf("I'm father; I'm writing test.txt...\n");
        write(fd, "hello", 5);
        close(fd);
    }
    else if (pid == 0) {
        printf("I'm child; I'm writing test.txt...\n");
        write(fd, "world", 5);
        sleep(5);
        write(fd, "lalala", 6);
        close(fd);
    }
    else {
        perror("fork");
        exit(-1);
    }
}

int main(int argc, char **argv)
{
    //test0();
    //test1();
    test2();
    return 0;
}
