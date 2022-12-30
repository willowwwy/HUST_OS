#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#define MAX_LEN 32
int main(char argc, char *argv[])
{
    int fd;
    int ret;
    int a, b;
    char write_buf[1024];
    char read_buf[1024];
    int num[2] = {123, 2};
    char *num2 = (char *)num;

    /* 将要打开的文件的路径通过main函数的参数传入 */
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return -1;
    }

    fd = open(argv[1], O_RDWR);
    if (fd < 0)
    {
        perror("fail to open file");
        return -1;
    }

    printf("Input First number:  ");
    scanf("%d", &a);
    printf("Input Second Number: ");
    scanf("%d", &b);
    sprintf(write_buf, "%d %d ", a, b);
    write(fd, write_buf, strlen(write_buf) + 1);
    printf("Read From Device: ");
    read(fd, read_buf, MAX_LEN);
    printf("%d\n", atoi(read_buf));
    close(fd);
    return 0;
}

