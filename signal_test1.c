//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o signal_test1 signal_test1.c
//https://blog.csdn.net/q1007729991/article/details/53842237

/*
 *不可重入函数：
       (1)引用了全局变量或静态变量的函数，称为不可重入函数，
       (2)如果一个函数使用了不可重入函数，那么该函数也会变成不可重入的
       (3)不可重入函数一定是线程不安全的
       (4)线程安全的函数，不一定是可重入函数
 *信号有三种处理方式:
       (4)忽略，但是需要注意的是有两种信号不可以：SIGKILL和SIGSTOP
       (2)执行信号的默认动作
       (3)捕获
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <pwd.h>
#include <uuid/uuid.h>
/*****************************************分割*****************************************/
//在信号处理函数中使用不可重入函数带来的危害
//struct passwd *getpwnam(const char *name); 获取用户登录相关的信息
int a = 100;

void handler(int sig)
{
    a++;
}

void test0()
{
    if (SIG_ERR == signal(SIGINT, handler))
    {
        perror("signal");
    }
    printf("a=%d\n", a);
    sleep(5);
    printf("a=%d\n", a);
}

int main(int argc, char **argv)
{
    test0();
    //test1();
    //test2();
    return 0;
}