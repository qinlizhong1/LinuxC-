//
// Created by lizhong on 2021/1/30.
//
//gcc -m32 -g -o pthread_create_test pthread_create_test.c -lpthread

/*打开FIFO文件:mkfifo函数只是创建一个FIFO文件，要使用命名管道还是将其打开;FIFO文件也可以使用open调用来打开
      open(const char *path, O_RDONLY); // 1
      open(const char *path, O_RDONLY | O_NONBLOCK); // 2
      open(const char *path, O_WRONLY); // 3
      open(const char *path, O_WRONLY | O_NONBLOCK); // 4
   选项 O_NONBLOCK 表示非阻塞，加上这个选项后，表示open调用是非阻塞的，如果没有这个选项，则表示open调用是阻塞的。

   open调用的阻塞是什么一回事呢？很简单，对于以只读方式（O_RDONLY）打开的FIFO文件，如果open调用是阻塞的（即第二个参数为O_RDONLY），
   除非有一个进程以写方式打开同一个FIFO，否则它不会返回；如果open调用是非阻塞的的（即第二个参数为O_RDONLY | O_NONBLOCK），
   则即使没有其他进程以写方式打开同一个FIFO文件，open调用将成功并立即返回。

   对于以只写方式（O_WRONLY）打开的FIFO文件，如果open调用是阻塞的（即第二个参数为O_WRONLY），open调用将被阻塞，直到有一个进程以只读方式打开同一个FIFO文件为止；
   如果open调用是非阻塞的（即第二个参数为O_WRONLY | O_NONBLOCK），open总会立即返回，但如果没有其他进程以只读方式打开同一个FIFO文件，open调用将返回-1，
   并且FIFO也不会被打开。
 *
 *
 *
 *
 */

#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    const char *fifo_name = "/tmp/my_fifo";
    int pipe_fd = -1;
    int data_fd = -1;
    int res = 0;
    const int open_mode = O_WRONLY;
    int bytes_sent = 0;
    char buffer[PIPE_BUF + 1];

    if (access(fifo_name, F_OK) == -1)
    {
        // 管道文件不存在
        // 创建命名管道
        res = mkfifo(fifo_name, 0777);
        if (res != 0)
        {
            fprintf(stderr, "Could not create fifo %s\n", fifo_name);
            exit(EXIT_FAILURE);
        }
    }

    printf("Process %d mkfifo O_WRONLY\n", getpid());

    // 以只写阻塞方式打开FIFO文件，以只读方式打开数据文件
    pipe_fd = open(fifo_name, open_mode);
    data_fd = open("./Data.txt", O_RDONLY);
    printf("Process %d result %d\n", getpid(), pipe_fd);

    if (pipe_fd != -1)
    {
        int bytes_read = 0;

        // 向数据文件读取数据
        bytes_read = read(data_fd, buffer, PIPE_BUF);
        buffer[bytes_read] = '\0';
        while (bytes_read > 0)
        {
            // 向FIFO文件写数据
            res = write(pipe_fd, buffer, bytes_read);
            if (res == -1)
            {
                fprintf(stderr, "Write error on pipe\n");
                exit(EXIT_FAILURE);
            }

            // 累加写的字节数，并继续读取数据
            bytes_sent += res;
            bytes_read = read(data_fd, buffer, PIPE_BUF);
            buffer[bytes_read] = '\0';
        }
        close(pipe_fd);
        close(data_fd);
    }
    else
    {
        exit(EXIT_FAILURE);
    }

    printf("Process %d finished\n", getpid());
    exit(EXIT_SUCCESS);

    return 0;
}