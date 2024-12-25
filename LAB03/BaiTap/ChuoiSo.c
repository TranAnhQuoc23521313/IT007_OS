#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <string.h>

struct Buffer
{
    int Number;
    int Status;
};
typedef struct Buffer BUFFER;

int main(int argc, char *argv[])
{
    const int SIZE = sizeof(BUFFER);
    const char *name = "OS";

    if (argc != 2)
    {
        printf("Sai cu phap hay nhap lai !\n");
        exit(0);
    }
    else
    {
        if (atoi(argv[1]) <= 0)
        {
            printf("N khong phai la 1 so nguyen duong. Hay nhap lai !\n");
            exit(0);
        }
    }
    pid_t PID = fork();
    if (PID > 0)
    {
        int n_Parent = shm_open(name, O_CREAT | O_RDWR, 0666);
        ftruncate(n_Parent, SIZE);
        BUFFER *ptr_num_Parent = mmap(0, SIZE, PROT_READ | PROT_WRITE,
                                      MAP_SHARED, n_Parent, 0);

        ptr_num_Parent->Number = atoi(argv[1]);
        ptr_num_Parent->Status = 0;
        while (1)
        {
            if (ptr_num_Parent->Status == -1)
            {
                if (ptr_num_Parent->Number != 1)
                    printf("%d, ", ptr_num_Parent->Number);
                //sleep(1);
                if (ptr_num_Parent->Number == 1)
                {
                    printf("%d\n ", ptr_num_Parent->Number);
                    break;
                }
                ptr_num_Parent->Status = ptr_num_Parent->Number;
            }
            sleep(1);
        }
        wait(NULL);
        munmap(ptr_num_Parent,SIZE);
        close(n_Parent); 
    }
    if (PID == 0)
    {
        int n_Child = shm_open(name, O_CREAT | O_RDWR, 0666);
        BUFFER *ptr_num_Child = mmap(0, SIZE, PROT_READ | PROT_WRITE,
                                     MAP_SHARED, n_Child, 0);
        ptr_num_Child->Status = -1;
        while (ptr_num_Child->Number > 1)
        {
            sleep(1);
            if (ptr_num_Child->Status == -1)
                continue;
            if (ptr_num_Child->Number % 2 == 0)
            {
                ptr_num_Child->Number = ptr_num_Child->Number / 2;
            }
            else
            {
                ptr_num_Child->Number = 3 * ptr_num_Child->Number + 1;
            }
            ptr_num_Child->Status = -1;
        } 
        munmap(ptr_num_Child,SIZE);
        close(n_Child);
        shm_unlink(name);
    }
    return 0;
}