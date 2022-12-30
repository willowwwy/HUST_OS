#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
    long ret;
    ret = syscall(549,1,2,3);   //Max
    printf("ret:%ld\n",ret);
    ret = syscall(549,7,6,5);   //Max
    printf("ret:%ld\n",ret);
    ret = syscall(548,4,6);     //Add
    printf("ret:%ld\n",ret);
    ret = syscall(549,7,9,8);   //Max
    printf("ret:%ld\n",ret);
}
