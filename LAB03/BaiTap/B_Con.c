#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>

#define BUFFER_SIZE 10

int main()
{
    const int SIZE = 4096;
    const char *name_1 = "OS";
    const char *name_2 = "AS";

    int fd;
    char *ptr;
    fd = shm_open(name_1, O_RDWR, 0666);
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE,MAP_SHARED, fd, 0);
    
    int n;
    int *buffer;
    n = shm_open(name_2, O_RDWR, 0666);
    buffer = mmap(0, SIZE, PROT_READ | PROT_WRITE,MAP_SHARED, n, 0);
    
    printf("Status system: %s\n",(char *)ptr);
    *buffer = -1;
    printf("BUFFER: \n");
    int sum = 0;
    while (sum <= 100)
    {
        if (*buffer == -1)
            continue;
        printf("Consumer: %d\n", *buffer);
        sum += *buffer;
        sleep(1);
        *buffer = -1;
    }
    printf("Consumer: Sum = %d\n",sum);
    strcpy(ptr, "End");
    printf("Status system: %s\n", ptr);
    munmap(buffer,BUFFER_SIZE);
    close(n);
    munmap(ptr, SIZE);
    close(fd);
    shm_unlink(name_1);
    shm_unlink(name_2);
    return 0;
}
