//
// Created by lizhong on 2021/1/30.
//
//gcc -g -o pipe_test pipe_test.c

/*无名管道特点
   （1）：只适用于具有亲缘关系的进程，如父子进程
   （2）：半双工，如果两端需要相互通信，需要建立两个管道
   （3）：管道无数据，写端存在，读操作会阻塞
   （4）：管道无数据，写端关闭，读操作会立即返回
   （5）：管道有数据，写端关闭，可以将数据读出
   （6）：只有在管道的读端存在时，向管道中写入数据才有意义。否则，向管道中写入数据的进程将收到内核传来的SIGPIPE信号
   （7）：只适用于具有亲缘关系的进程，如父子进程
 *
 *
 *
 *
 *
 */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
/*****************************************分割*****************************************/
//父进程 fork 出一个子进程，通过无名管道向子进程发送字符，子进程收到数据后将字符串中的小写字符转换成大写并输出。
void child(int *fd)
{
    close(fd[1]); // 子进程关闭写端
    char buf[64];
    int n = 0,i;

    while(1)
    {
        n = read(fd[0], buf, 64); // 如果没有数据可读，read 会阻塞；如果父进程退出，read 返回 0.
        for (i = 0; i < n; ++i)
            putchar(toupper(buf[i]));

        if (*buf == 'q')
        {
            close(fd[0]);
            exit(0);
        }
        if (n == 0)
        {
            puts("no data to read!");
            sleep(1);
        }
    }
    exit(0);
}

void test0()
{
    int fd[2];
    int n = 0;
    char buf[64] = { 0 };
    if (pipe(fd) < 0)
    {
        perror("pipe");
        exit(-1);
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        child(fd);
    }

    close(fd[0]);// 父进程关闭读端
    while (1)
    {
        n = read(STDIN_FILENO, buf, 64);
        write(fd[1], buf, n);
        if (*buf == 'q')
        {
            close(fd[1]);
            exit(0);
        }
    }
}

/*****************************************分割*****************************************/
//管道中没有数据时且写端没有关闭，读操作阻塞
void test1()
{
    int fd[2];
    char buf[50] = {0};//缓存

    if(pipe(fd)!=0)// 创建无名管道
    {
        perror("pipe fail: ");
        exit(1);
    }
    printf("%d %d\n",fd[0],fd[1]);//打开的文件描述符，此处为3，4 默认打开 0,1,2,标准输入，输出，出错
    //管道中没有数据的时候读阻塞
    //write(fd[1],"hello",10);  //此处不向管道写数据时，读操作会阻塞，管道中有数据时，读操作后结束进程
    read(fd[0],buf,10);
    printf("%s",buf);
    putchar(10); // '\n'
}


/*****************************************分割*****************************************/
//管道中无数据，将写端关闭，读操作会立即返回
void test2()
{
    int fd[2];
    char buf[50] = {0};//缓存

    if(pipe(fd)!=0)// 创建无名管道
    {
        perror("pipe fail: ");
        exit(1);
    }
    printf("%d %d\n",fd[0],fd[1]);//打开的文件描述符，此处为3，4 默认打开 0,1,2,标准输入，输出，出错
    //管道中没有数据的时候读阻塞
    close(fd[1]);
    read(fd[0],buf,10);
    printf("%s",buf);
}

/*****************************************分割*****************************************/
//管道中有数据，将写端关闭，可以将数据读出
void test3()
{
    int fd[2];
    char buf[50] = {0};//缓存
    if(pipe(fd)!=0)// 创建无名管道
    {
        perror("pipe fail: ");
        exit(1);
    }
    printf("%d %d\n",fd[0],fd[1]);//打开的文件描述符，默认打开 0,1,2
    //管道中有数据，写端关闭，可以将数据读出
    write(fd[1],"12349",5); //向管道中写入数据，然后关闭写端
    close(fd[1]);
    read(fd[0],buf,5);
    printf("%s",buf);
    putchar(10); // '\n'
}

/*****************************************分割*****************************************/
//管道大小 64K
void test4()
{
    int fd[2];
    char buf[50] = {0};//缓存
    if(pipe(fd)!=0)// 创建无名管道
    {
        perror("pipe fail: ");
        exit(1);
    }
    printf("%d %d\n",fd[0],fd[1]);//打开的文件描述符，默认打开 0,1,2
    //管道大小 64k
    int num = 0;
    ssize_t size;
    while(1) //循环一直运行，当写入 64K 后悔阻塞
    {
        size = write(fd[1],"123",1024);
        printf("size = %ld\n",size);  //每次写入1024个字节，即 1K ，此次编译会警告
        num++;
        printf("num = %d\n",num);
    }
}


/*****************************************分割*****************************************/
int main(int argc, char **argv)
{
    //test0();
    //test1();
    //test2();
    //test3();
    test4();

    return 0;
}