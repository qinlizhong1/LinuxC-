//
// Created by lizhong on 2021/2/03.
//
//gcc -m32 -g -o pthread_rwlock_test pthread_rwlock_test.c -lpthread

/*
 *
 *
 *
 *
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

struct Ticket{
    int remain; // 余票数，初始化为 1000
    pthread_rwlock_t rwlock; // 读写锁
    pthread_mutex_t mlock; // 互斥锁，主要是为了和读写锁进行对比
}ticket;

int lockType = 0;
/*****************************************分割*****************************************/
//不加锁会有问题，运行会发现3张票卖出了6张
void *buy(void *arg)
{
    int name = *((int *)arg);

    if (lockType == 1)
    {
        pthread_mutex_lock(&ticket.mlock);
    }

    else if (lockType == 2)
    {
        pthread_rwlock_wrlock(&ticket.rwlock); // 写模式加锁
    }

    int remain = ticket.remain;
    remain -= 10; // 一次买 10 张票
    sleep(1);
    ticket.remain = remain;
    printf("%03d buy 10 tickets\n", name);

    if (lockType == 1)
    {
        pthread_mutex_unlock(&ticket.mlock);
    }
    else if (lockType == 2)
    {
        pthread_rwlock_unlock(&ticket.rwlock);
    }

    sleep(rand() % 5 + 2);

    return  NULL;
}

void* query(void *arg)
{
    int name = *((int *)arg);
    int remin = 0;

    if (0 == lockType) //不加锁
    {
        ;
    }
    else if (1 == lockType)
    {
        pthread_mutex_lock(&ticket.mlock);
    }
    else if(2 == lockType)
    {
        pthread_rwlock_rdlock(&ticket.rwlock);
    }
    else
    {
        printf("the lockType[%d] not supprt\n", lockType);
    }

    remin = ticket.remain;
    sleep(1);
    printf("%03d query--->remin:%d\n", name, remin);

    if (1 == lockType)
    {
        pthread_mutex_unlock(&ticket.mlock);
    }
    else if(2 == lockType)
    {
        pthread_rwlock_unlock(&ticket.rwlock);
    }

    return  NULL;
}

void test0()
{
    int i;
    int names[100];
    ticket.remain = 1000; //初始化余票1000张

    printf("start:remain ticket = %d\n", ticket.remain);
    pthread_t tid[100];


    for (i = 0; i < 100; ++i)
    {
        names[i] = i;
    }

    pthread_rwlock_init(&ticket.rwlock, NULL);
    pthread_mutex_init(&ticket.mlock, NULL);

    for (i = 0; i < 100; ++i)
    {
        if (i % 10 == 0)
        {
            pthread_create(&tid[i], NULL, buy, (void*)(&names[i]));
        }
        else
        {
            pthread_create(&tid[i], NULL, query, (void*)(&names[i]));
        }
    }

    for (i = 0; i < 100; ++i)
    {
        pthread_join(tid[i], NULL);
    }

    pthread_rwlock_destroy(&ticket.rwlock);
    pthread_mutex_destroy(&ticket.mlock);

    printf("end:remain ticket = %d\n", ticket.remain);
}

/*****************************************分割*****************************************/
int main(int argc, char **argv)
{
    lockType = atoi(argv[1]);
    test0();
    //test1();

    return 0;
}