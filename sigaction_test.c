//
// Created by lizhong on 2021/1/30.
//
//gcc -g -o sigaction_test sigaction_test.c

/*
 *
 *
 *
 *
 *
 */

#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void printsigset(const sigset_t *set)
{
    int i;
    for (i = 1; i <= 32; i++)
    {
        if ((i-1)% 8 == 0) putchar(' ');
        if (sigismember(set, i) == 1)
        {
            putchar('1');
        }
        else
        {
            putchar('0');
        }
    }
    puts("");
}

/*****************************************分割*****************************************/
void handler(int sig)
{
    if (sig == SIGTSTP)
    {
        printf("hello SIGTSTP\n");
    }

    if (sig == SIGINT)
    {
        printf("hello SIGINT\n");
    }

    sleep(5);
    sigset_t st;
    sigpending(&st);
    printsigset(&st);
}

void test0()
{
    printf("I'm %d\n", getpid());
    struct sigaction act, oldact;
    act.sa_handler = handler; // 设置普通信号处理函数

    // 向 sa_mask 中添加 SIGINT
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGINT);//sa_mask 的含义是 “在执行信号处理函数时，应该屏蔽掉哪些信号”。
    act.sa_flags = 0; // 先置 0

    sigaction(SIGTSTP, &act, &oldact);
    sigaction(SIGINT, &act, &oldact);

    while(1)
    {
        write(STDOUT_FILENO, ".", 1);
        pause();
    }
}

/*****************************************分割*****************************************/
void test1()
{

}


/*****************************************分割*****************************************/
void test2()
{

}


/*****************************************分割*****************************************/
int main(int argc, char **argv)
{
    test0();
    //test1();
    //test2();

    return 0;
}