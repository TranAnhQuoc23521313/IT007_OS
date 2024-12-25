#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <time.h>

#define BUFFER_SIZE 10

int main()
{
    srand(time(NULL));
    const char *name_1 = "OS";
    const char *name_2 = "AS";
    const int SIZE = 4096;
    int fd;
    char *ptr;
    fd = shm_open(name_1, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, SIZE);
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE,
               MAP_SHARED, fd, 0);
    int *buffer;
    int n;
    n = shm_open(name_2, O_CREAT | O_RDWR, 0666);
    ftruncate(n, BUFFER_SIZE);
    buffer = mmap(0, BUFFER_SIZE, PROT_READ | PROT_WRITE,
                  MAP_SHARED, n, 0);
    int sum = 0;
    strcpy(ptr, "Start");
    *buffer = 0;
    while (strncmp(ptr, "Start", 5) == 0)
    {
        if (*buffer == -1)
        {
            *buffer = rand() % 11 + 10;
            //sum += *buffer;
            printf("Producer: %d\n", *buffer);
            sleep(1);
        }
        else
        {
            printf("Waiting !\n");
            sleep(1);
        }
    }
    munmap(ptr, SIZE);
    munmap(buffer, BUFFER_SIZE);
    close(n);
    close(fd);
    return 0;
}