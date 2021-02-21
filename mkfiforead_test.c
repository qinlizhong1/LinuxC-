//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o pthread_create_test pthread_create_test.c -lpthread

/*命名管道安全问题：
   只使用一个FIFO文件，如果有多个进程同时向同一个FIFO文件写数据，而只有一个读FIFO进程在同一个FIFO文件中读取数据时，会发生怎么样的情况呢，会发生数据块的相互交错是很正常的？
   为了解决这一问题，就是让写操作的原子化。怎样才能使写操作原子化呢？答案很简单，系统规定：在一个以O_WRONLY（即阻塞方式）打开的FIFO中， 如果写入的数据长度小于等待PIPE_BUF，
   那么或者写入全部字节，或者一个字节都不写入。如果所有的写请求都是发往一个阻塞的FIFO的，并且每个写记请求的数据长度小于等于PIPE_BUF字节，系统就可以确保数据决不会交错在一起。
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>

int main(int argc, char **argv)
{
    const char *fifo_name = "/tmp/my_fifo";
    int pipe_fd = -1;
    int data_fd = -1;
    int res = 0;
    int open_mode = O_RDONLY;
    char buffer[PIPE_BUF + 1];
    int bytes_read = 0;
    int bytes_write = 0;

    // 清空缓冲数组
    memset(buffer, '\0', sizeof(buffer));

    printf("Process %d opening FIFO O_RDONLY\n", getpid());

    // 以只读阻塞方式打开管道文件，注意与fifowrite.c文件中的FIFO同名
    pipe_fd = open(fifo_name, open_mode);

    // 以只写方式创建保存数据的文件
    data_fd = open("./DataFormFIFO.txt", O_WRONLY | O_CREAT, 0644);
    printf("Process %d result %d\n", getpid(), pipe_fd);

    if (pipe_fd != -1)
    {
        do
        {
            // 读取FIFO中的数据，并把它保存在文件DataFormFIFO.txt文件中
            res = read(pipe_fd, buffer, PIPE_BUF);
            bytes_write = write(data_fd, buffer, res);
            bytes_read += res;
        }
        while (res > 0);
        close(pipe_fd);
        close(data_fd);
    }
    else
    {
        exit(EXIT_FAILURE);
    }

    printf("Process %d finished, %d bytes read\n", getpid(), bytes_read);
    exit(EXIT_SUCCESS);

    return 0;
}