//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o signal_test signal_test.c

/*
 *产生信号的方式：
       (1)用户在终端按下某些组合键时,终端驱动程序会发送信号给前台进程；如Ctrl-C产生SIGINT信号
       (2)硬件异常产生信号,这些条件由硬件检测到并通知内核,然后内核向当前进程发送适当的信号
       (3)一个进程调用kill(2)函数可以发送信号给另一个进程。
 *信号有三种处理方式:
       (1)忽略，但是需要注意的是有两种信号不可以：SIGKILL和SIGSTOP
       (2)执行信号的默认动作
       (3)捕获
 *
 *
 *
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

void sighandler(int sig)
{
    switch(sig)
    {
        case SIGUSR1:
            printf("hello SIGUSR1\n");break;
        case SIGUSR2:
            printf("hello SIGUSR2\n");break;
        case SIGINT:
            printf("hello SIGINT\n");break;
        case SIGTSTP:
            printf("hello SIGTSTP\n");break;
        case SIGQUIT:
            printf("hello SIGQUIT\n");break;
        case SIGSEGV:
            printf("hello SIGSEGV\n");break;
        default:
            printf("hello, who are you %d?\n", sig);
    }
    sleep(2); // 删除这一行，再给程序发信号，看看 main 函数打点的情况。
}

void test0()
{
    printf("I'm %d\n", getpid());

    if (SIG_ERR == signal(SIGUSR1, sighandler))
    {
        perror("signal SIGUSR1");
    }
    if (SIG_ERR == signal(SIGUSR2, sighandler))
    {
        perror("signal SIGUSR2");
    }
    if (SIG_ERR == signal(SIGINT, sighandler))
    {
        perror("signal SIGINT");
    }
    if (SIG_ERR == signal(SIGTSTP, sighandler))
    {
        perror("signal SIGTSTP");
    }
    if (SIG_ERR == signal(SIGQUIT, sighandler))
    {
        perror("signal SIGQUTI");
    }
    if (SIG_ERR == signal(SIGSEGV, sighandler))
    {
        perror("signal SIGSEGV");
    }

    while(1)
    {
        write(STDOUT_FILENO, ".", 1);
        sleep(10);
    }
}

/***********************************分割*************************************/
void waitchild(int sig)
{
    int status;
    pid_t pid;
    if ((pid = waitpid(-1, &status, WUNTRACED | WCONTINUED)) > 0)
    {
        if (WIFEXITED(status))
        {
            printf("child %d exited! return code = %d\n\n", pid, WEXITSTATUS(status));
        }
        else if (WIFSIGNALED(status))
        {
            printf("child %d terminated by signal %d\n\n", pid, WTERMSIG(status));
        }
        else if (WIFSTOPPED(status))
        {
            printf("child %d stopped by signal %d\n\n", pid, WSTOPSIG(status));
        }
        else if (WIFCONTINUED(status))
        {
            printf("child %d continued\n\n", pid);
        }
    }
    sleep(1);
}

void child(int n)
{
    if (n == 9)
        *((int*)0) = 0;
    exit(n + 1);
}

void test1()
{
    printf("I'm %d\n", getpid());

    if (SIG_ERR == signal(SIGCHLD, waitchild))
    {
        perror("signal SIGSTOP");
    }

    int n = 10;
    pid_t pid;

    while(n--)
    {
        pid = fork();
        if (pid == 0)
        {
            child(n);
        }
        else if (pid == -1)
        {
            perror("fork");
        }
    }

    while(1)
    {
        write(STDOUT_FILENO, ".", 1);
        sleep(10);
    }
}

int main(int argc, char **argv)
{
    //test0();
    test1();
    //test2();
    return 0;
}