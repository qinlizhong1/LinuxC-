//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o pthread_create_test pthread_create_test.c -lpthread

/*
 *int kill(pid_t pid, int sig)
     pid > 0，表示向进程号为 pid 的进程发信号
     pid = 0，表示向同组进程发信号（有权限才行）
     pid = -1，向所有进程发信号（有权限才行)
     pid < -1，向进程组 |pid| 发信号（有权限才行）
 *
 */

#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
/*****************************************分割*****************************************/
//父进程 fork 出一个子进程，然后该子进程向父进程发送 10 次信号。父进程 fork 完后每 10 称向屏幕打点。

void handler(int sig)
{
    if (sig == SIGUSR1)
    {
        printf("Hello my child\n");
    }
    else if (sig == SIGCHLD)
    {
        wait(NULL);
    }
}


void child() {
    int n = 10;
    while(n--)
    {
        if (kill(getppid(), SIGUSR1) == -1)
        {
            perror("kill");
        }
        sleep(1);
    }

    exit(0);
}

void test0()
{
    if (SIG_ERR == signal(SIGUSR1, handler))
    {
        perror("signal SIGUSR1");
    }

    if (SIG_ERR== signal(SIGCHLD, handler))
    {
        perror("signal SIGUSR2");
    }

    pid_t pid = fork();

    if (pid == 0) {
        child();
    }
    else if (pid == -1)
    {
        perror("fork");
    }

    while(1)
    {
        write(STDOUT_FILENO, ".", 1);
        sleep(10);
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