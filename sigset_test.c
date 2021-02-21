//
// Created by lizhong on 2021/1/30.
//
//gcc -g -o sigset_test sigset_test.c

/*
 *   int sigaddset(sigset_t *set, int signo);
     int sigdelset(sigset_t *set, int signo);
     int sigemptyset(sigset_t *set);
     int sigfillset(sigset_t *set);
     int sigismember(const sigset_t *set, int signo);
 *
 *
 *
 *linux和mac系统程序执行结果会不同，Mac系统信号会少很多;SIGSYS在linux是31号信号，在mac下是12号信号
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
void test0()
{
    sigset_t st;
    printf("1. create set\n");
    printsigset(&st);

    // fill set
    printf("\n2. fill set\n");
    sigfillset(&st);
    printsigset(&st);

    // empty set
    printf("\n3. empty set\n");
    sigemptyset(&st);
    printsigset(&st);

    // add sig
    printf("\n4. add SIGHUP(1), SIGINT(2), SIGKILL(9), SIGSYS(31)to set\n");
    sigaddset(&st, SIGHUP);
    sigaddset(&st, SIGINT);
    sigaddset(&st, SIGKILL);
    sigaddset(&st, SIGSYS);
    printsigset(&st);

    // delete sig
    printf("\n5. delete SIGKILL from set\n");
    sigdelset(&st, SIGKILL);
    printsigset(&st);

    // is member
    printf("\n");
    if (sigismember(&st, SIGKILL)) {
        printf("SIGKILL is member\n");
    }
    if (sigismember(&st, SIGINT)) {
        printf("SIGINT is member\n");
    }
}

/*****************************************分割*****************************************/
//该程序的功能是先把 SIGINT、SIGTSTP 加入到了进程阻塞信号集中去。接下来，每隔一秒打印一次未决信号集，
//第 10 次的时候，又把 SIGINT 信号从阻塞信号集中删除。
void handler(int sig)
{
    if (sig == SIGINT)
    {
        printf("hello SIGINT\n");
    }

    if (sig == SIGQUIT)
    {
        printf("hello SIGQUIT\n");
    }
}

void test1()
{
    printf("I'm %d\n", getpid());

    sigset_t st, oldst;
    sigemptyset(&st);
    sigaddset(&st, SIGINT);
    sigaddset(&st, SIGTSTP);
    sigprocmask(SIG_BLOCK, &st, &oldst);
    printf("new set:");
    printsigset(&st);
    printf("old set:");
    printsigset(&oldst);

    if (SIG_ERR == signal(SIGINT, handler))
    {
        perror("signal SIGINT");
        exit(-1);
    }

    if (SIG_ERR == signal(SIGQUIT, handler))
    {
        perror("signal SIGQUIT");
        exit(-1);
    }

    puts("");

    int n = 0;

    while(1)
    {
        sigpending(&st);
        printsigset(&st);
        puts("");
        sleep(6);

        if (n == 10)
        {
            sigset_t tmp;
            sigemptyset(&tmp);
            sigaddset(&tmp, SIGINT);
            sigprocmask(SIG_UNBLOCK, &tmp, NULL);
        }

        ++n;
    }
}


/*****************************************分割*****************************************/
void test2()
{

}


/*****************************************分割*****************************************/
int main(int argc, char **argv)
{
    //test0();
    test1();
    //test2();

    return 0;
}