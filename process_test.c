//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o process_test process_test.c

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

//进程扇，一个父进程，多个子进程（子进程之间互为兄弟进程）
void test0()
{
    int i=0;
    pid_t pid;

    for (; i<10; i++)
    {
        pid = fork();
        if (pid >0) //父进程
        {
            if (i == 0 || i == 1)
            {
                printf("I am parent, pid[%ld]\n", (long)getpid());
            }
        }
        else if (pid == 0)  //子进程退出
        {
            printf("child pid[%ld], my parent pid[%ld]\n", (long)getpid(), (long)getppid());
            sleep(5);
            exit(-1);
        }
        else
        {
            perror("fork error");
            exit(-1);
        }

    }
    sleep(60);
}

/*****************************************分割*****************************************/
//进程链，父生子，子生孙，都是单传
void test1()
{
    int i=0;
    pid_t pid;

    for (; i<10; i++)
    {
        pid = fork();
        if (pid >0) //父进程
        {
            if (i == 0)
            {
                printf("I am parent, pid[%ld]\n", (long)getpid());
            }
            sleep(1);  //父进程需要休眠，不然父进程退出，子进程就成了孤儿进程。子进程中打印的父进程ID就是1
            break;
        }
        else if (pid == 0)  //子进程退出
        {
            printf("child pid[%ld], my parent pid[%ld]\n", (long)getpid(), (long)getppid());
        }
        else
        {
            perror("fork error");
            exit(-1);
        }

    }
}

/*****************************************分割*****************************************/
/*进程组的生命期:
     从被创建开始，到其内所有进程终止或离开该组。
 *进程组相关系统调用
     pid_t getpgid(pid_t pid); //获取进程 pid 的进程组 id.
     int setpgid(pid_t pid, pid_t pgid); //指定 pid 为进程组组长或将 pid 加入到组 pgid.
 */
//创建三个子进程，设置父进程和第一个子进程是一组，其余两个子进程是一组
void test2()
{
    int i=0;
    pid_t pid;
    pid_t gid;
    setpgid(getpid(), getpid());//父进程为组长进程

    for (; i<3; i++)
    {
        pid = fork();
        if (pid >0) //父进程
        {
            if (i == 0)
            {
                printf("parent process, pid[%ld], ppid[%ld], pgid[%ld]\n", \
                    (long)getpid(), (long)getppid(), (long)getpgid(getpid()));
            }
        }
        else if (pid == 0)  //子进程退出
        {
            if (i == 0)
            {
                setpgid(getpid(), getppid());
                printf("child process, pid[%ld], ppid[%ld], pgid[%ld]\n", \
                    (long)getpid(), (long)getppid(), (long)getpgid(getpid()));
            }
            else if (i == 1)
            {
                setpgid(getpid(), getpid()); //第二组的第一个子进程为进程组长
                printf("child process, pid[%ld], ppid[%ld], pgid[%ld]\n", \
                    (long)getpid(), (long)getppid(), (long)getpgid(getpid()));
                gid  = getpgid(getpid());
                //printf("0000gid = %ld\n", (long)gid);
            }
            else
            {
                //printf("gid = %ld\n", (long)gid);
                //setpgid(getpid(), gid); todo如何将gid传给这个子进程
                printf("child process, pid[%ld], ppid[%ld], pgid[%ld]\n", \
                    (long)getpid(), (long)getppid(), (long)getpgid(getpid()));
            }

            sleep(5);
            exit(-1);
        }
        else
        {
            perror("fork error");
            exit(-1);
        }

    }
    sleep(60);
}

/*****************************************分割*****************************************/

int main(int argc, char **argv)
{
    //test0();
    //test1();
    test2();

    return 0;
}

