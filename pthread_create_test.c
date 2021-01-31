//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o pthread_create_test pthread_create_test.c -lpthread

/*
 *线程独有的资源：
       (1)线程ID
       (2)线程栈
       (3)线程独有的寄存器
       (4)errno变量
       (5)线程私有数据
       (6)信号屏蔽字
       (7)调度优先级和策略
 *线程终止方式:
       (1)线程过程函数执行 return 正常返回，返回值是线程的退出码（主动终止）
       (2)线种过程函数执行 pthread_exit 函数退出，其参数是线程退出码（主动终止）
       (3)在其它线程中调用 pthread_cancel 函数(被动终止)
       (4)任意线程调用 exit、_Exit 或者 _exit 导致整个进程终止(被动终止)
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

#define ERR(name,err) do{printf("%s:%s\n",#name,strerror(err));exit(-1);}while(0);


void* th_fn1(void* arg)
{
    puts("thread 1 returning");
    return (void*)10;
}

void* th_fn2(void* arg)
{
    puts("thread 2 exiting");
    pthread_exit((void*)20);
    puts("thread 2 exited");
}


void test0()
{
    pthread_t tid1, tid2;
    int err;
    void* ret;

    err = pthread_create(&tid1, NULL, th_fn1, NULL);
    if (err != 0)
        ERR("pthread_create1", err);

    err = pthread_create(&tid2, NULL, th_fn2, NULL);
    if (err != 0)
        ERR("pthread_create2", err);

    sleep(2);

    // 主线程阻塞直到线程 tid1 退出
    err = pthread_join(tid1, &ret);
    if (err != 0)
        ERR("pthread_join1", err);
    printf("thread 1 exit code %d\n", (int)ret);

    // 主线程阻塞直到线程 tid2 退出
    err = pthread_join(tid2, &ret);
    if (err != 0)
        ERR("pthread_join2", err);
    printf("thread 2 exit code %d\n", (int)ret);
}

/****************************************分割***************************************/
void* th_fn3(void* arg)
{
    while(1)
    {
        puts("thread 3 running");
        sleep(1);
    }
    return (void*)10;
}

void* th_fn4(void* arg)
{
    while(1)
    {
        puts("thread 4 running");
        sleep(1);
    }
    pthread_exit((void*)20);
}

void test1()
{
    pthread_t tid1, tid2;
    int err;
    void* ret;

    err = pthread_create(&tid1, NULL, th_fn3, NULL);
    if (err != 0)
        ERR(pthread_create1, err);

    err = pthread_create(&tid2, NULL, th_fn4, NULL);
    if (err != 0)
        ERR(pthread_create2, err);

    sleep(5);

    // 通知 tid1 和 tid2 退出。
    pthread_cancel(tid1);
    pthread_cancel(tid2);

    err = pthread_join(tid1, &ret);

    // 线程退出码都是 PTHREAD_CANCELED
    printf("PTHREAD_CANCELED = %d\n", (int)PTHREAD_CANCELED);
    if (err != 0)
        ERR(pthread_join1, err);
    printf("thread 1 exit code %d\n", (int)ret);

    err = pthread_join(tid2, &ret);
    if (err != 0)
        ERR(pthread_join2, err);
    printf("thread 2 exit code %d\n", (int)ret);
}

int main(int argc, char **argv)
{
    //test0();
    test1();
    //test2();
    return 0;
}