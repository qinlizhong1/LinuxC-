//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o pthread_create_test pthread_create_test.c -lpthread

/*
 *有时候我们希望线程退出时能够自动的执行某些函数，为了能达到此目的，OS 提供了两个函数帮我们完成这个功能：
    void pthread_cleanup_push(void (*rtn)(void*), void *arg);
    void pthread_cleanup_pop(int execute);
    注意点：在 Linux 中，pthread_cleanup_push 和 pthread_cleanup_pop 这两个函数是通过宏来做的，
           pthread_cleanup_push 被替换成以左花括号 { 为开头的一段代码，而 pthread_cleanup_pop
           被替换成以右花括号 } 结尾的一段代码，这就意味着这两个函数必须要成对出现才能将左右花括号匹配上，
           否则就出现编译错误。

 *线程清理函数调用时机:
       (1)线程还未执行 pthread_cleanup_pop 前，被 pthread_cancel 取消
       (2)线程还未执行 pthread_cleanup_pop 前，主动执行 pthread_exit 终止
       (3)线程执行 pthread_cleanup_pop，且 pthread_cleanup_pop 的参数不为 0.
 */

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>

/*****************************************分割*****************************************/
//线程还未执行 pthread_cleanup_pop 前，被 pthread_cancel 取消
void cleanUp(void *arg)
{
    printf("[ %s ]cleanup run\n", (char *)arg);
}

void* threadFun(void* arg)
{
    printf("thread[%x] run\n", (unsigned int)pthread_self());

    pthread_cleanup_push(cleanUp, (void *)__FUNCTION__);
    sleep(1);
    pthread_cleanup_pop(0);//这种情况pop参数可以为任何值
    return NULL;
}

void test0()
{
    pthread_t tid;
    int res = 0;
    if ((res = pthread_create(&tid, NULL, threadFun, NULL)) != 0)
    {
        printf("pthread_create error, return:%d\n", res);
    }
    pthread_cancel(tid);
    pthread_join(tid, NULL);

}

/*****************************************分割*****************************************/
//线程还未执行 pthread_cleanup_pop 前，被 pthread_cancel 取消
void* threadFun1(void* arg)
{
    printf("thread[%x] run\n", (unsigned int)pthread_self());
    pthread_cleanup_push(cleanUp, (void *)__FUNCTION__);
    pthread_exit(NULL);
    pthread_cleanup_pop(0);  //这种情况pop参数可以为任何值
    return NULL;
}

void test1()
{
    pthread_t tid;
    int res = 0;
    if ((res = pthread_create(&tid, NULL, threadFun1, NULL)) != 0)
    {
        printf("pthread_create error, return:%d\n", res);
    }

    pthread_join(tid, NULL);
}

/*****************************************分割*****************************************/
//线程执行 pthread_cleanup_pop，且 pthread_cleanup_pop 的参数不为 0.
void* threadFun2(void* arg)
{
    printf("thread[%x] run\n", (unsigned int)pthread_self());
    pthread_cleanup_push(cleanUp, (void *)__FUNCTION__);
    pthread_cleanup_pop(1);//这种情况只有该函数参数为非0，才会去调用清理函数
    return NULL;
}

void test2()
{
    pthread_t tid;
    int res = 0;
    if ((res = pthread_create(&tid, NULL, threadFun2, NULL)) != 0)
    {
        printf("pthread_create error, return:%d\n", res);
    }

    pthread_join(tid, NULL);
}

/*****************************************分割*****************************************/
int main(int argc, char **argv)
{
    test0();
    test1();
    test2();
    return 0;
}