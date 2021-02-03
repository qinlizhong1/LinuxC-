//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o process_test process_test.c
/*
 * pid_t setsid(void) 可以用来创建一个新会话
    (1)调用此函数的进程不能是进程组组长，否则会失败。
    (2)调用进程成为新进程组的组长。也即新进程组 id 等于该进程 id.
    (3)调用程变成新会话的 session leader，此时，该进程是新会话中的唯一进程。
    (4)调用进程没有控制终端（控制终端的概念后面会说）。如果在调用 setsid 之前该进程有一个控制终端，那么这种联系也被切断。
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

/*****************************************分割*****************************************/
//验证组长进程调用setsid会报错
void test0()
{
    pid_t pid;

    setpgid(getpid(), getpid());
    printf("parent process, pid[%ld], ppid[%ld], pgid[%ld], sid[%ld]\n", \
        (long)getpid(), (long)getppid(), (long)getpgid(getpid()), (long)getsid(0));

    if (setsid() < 0)
    {
        perror("setsid error");
    }
};

/*****************************************分割*****************************************/
//验证调用setsid的进程成为会话首进程和新进程组的组长进程
void test1()
{
    pid_t pid;

    setpgid(getpid(), getpid());

    pid = fork();
    if (pid > 0)//父进程
    {
        printf("parent process, pid[%ld], ppid[%ld], pgid[%ld], sid[%ld]\n", \
            (long)getpid(), (long)getppid(), (long)getpgid(getpid()), (long)getsid(0));
        sleep(10);//防止子进程变为僵尸进程
    }
    else if(pid == 0) //子进程，创建会话
    {
        printf("befor setsid parent process, pid[%ld], ppid[%ld], pgid[%ld], sid[%ld]\n", \
            (long)getpid(), (long)getppid(), (long)getpgid(getpid()), (long)getsid(0));
        if (setsid() < 0)
        {
            perror("setsid error");
        }
        printf("after setsid parent process, pid[%ld], ppid[%ld], pgid[%ld], sid[%ld]\n", \
            (long)getpid(), (long)getppid(), (long)getpgid(getpid()), (long)getsid(0));
    }
    else
    {
        perror("fork error");
    }
};

/*****************************************分割*****************************************/

int main(int argc, char **argv)
{
    //test0();
    test1();
    //test2();

    return 0;
}