//
// Created by lizhong on 2021/2/03.
//
//gcc -m32 -g -o pthread_mutex_test pthread_mutex_test.c -lpthread

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

int tickets = 3;
/*****************************************分割*****************************************/
//不加锁会有问题，运行会发现3张票卖出了6张
void *lizhong(void *arg)
{
    int flag = 1;
    int t = 0;

    while (flag)
    {
        t = tickets;
        //sleep(4);
        if (t > 0)
        {
            printf("lizhong buy a ticket!!!\n");
            t--;
            sleep(4);
            tickets = t;
        }
        else
        {
            flag = 0;
        }
    }

    return  NULL;
}

void* zhangsan(void *arg)
{
    int flag = 1;
    int t = 0;

    while (flag)
    {
        t = tickets;
        //sleep(4);
        if (t > 0)
        {
            printf("zhangsan buy a ticket!!\n");
            t--;
            sleep(4);
            tickets = t;
        }
        else
        {
            flag = 0;
        }
    }

    return  NULL;
}

void test0()
{
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, lizhong ,NULL);
    pthread_create(&t2, NULL,  zhangsan, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

/*****************************************分割*****************************************/
//加上互斥锁
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
void *lizhong1(void *arg)
{
    int flag = 1;
    int t = 0;

    while (flag)
    {
        pthread_mutex_lock(&mutex);
        t = tickets;
        //sleep(4);
        if (t > 0)
        {
            printf("lizhong1 buy a ticket!!!\n");
            t--;
            sleep(4);
            tickets = t;
        }
        else
        {
            flag = 0;
        }
        pthread_mutex_unlock(&mutex);
    }

    return  NULL;
}

void* zhangsan1(void *arg)
{
    int flag = 1;
    int t = 0;

    while (flag)
    {
        pthread_mutex_lock(&mutex);
        t = tickets;
        //sleep(4);
        if (t > 0)
        {
            printf("zhangsan1 buy a ticket!!\n");
            t--;
            sleep(4);
            tickets = t;
        }
        else
        {
            flag = 0;
        }
        pthread_mutex_unlock(&mutex);
    }

    return  NULL;
}

void test1()
{
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1, NULL, lizhong1, NULL);
    pthread_create(&t2, NULL,  zhangsan1, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}



/*****************************************分割*****************************************/
int main(int argc, char **argv)
{
    //test0();
    test1();

    return 0;
}