//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o wait_test wait_test.c

/*
 * (1)子进程退出，而父进程并没有调用 wait() 或 waitpid() 获取子进程的状态信息，
 *     那么子进程的 PID 和 进程描述符 等资源仍然保存在系统中，这种进程称之为僵尸进程 。
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

//僵尸进程产生,父进程产生子进程，然后父进程休眠20s,杀死子进程，用ps查看子进程状态为Z
void test0()
{
    pid_t pid = fork();
    if (pid > 0)
    {
        printf("I'm parent\n");
        sleep(30);
    }
    else if (pid == 0)
    {
        printf("I'm child\n");
    }
    else
    {
        perror("fork");
        exit(-1);
    }
}

int main(int argc, char **argv)
{
    //test0();
    //test1();
    //test2();
    return 0;
}