//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o fork_test fork_test.c

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

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

int main(int argc, char **argv)
{
    //test0();
    test1();

    return 0;
}
