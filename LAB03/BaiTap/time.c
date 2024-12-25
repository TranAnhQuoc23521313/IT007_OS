#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: ./time <command> \n");
        return 1;
    }
    struct timeval start, end;
    gettimeofday(&start, NULL);
    pid_t PID = fork();
    if (PID == 0)
    {
        execvp(argv[1], &argv[1]);
        printf("execvp");
        exit(1);
    }
    else if (PID < 0)
    {
        perror("fork that bai !");
        exit(1);
    }
    else
    {
        int status;
        waitpid(PID, &status, 0);
        gettimeofday(&end, NULL);

        if (WIFEXITED(status))
        {
            double ex_time = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
            printf("Thoi gian thuc thi: %f giay \n", ex_time);
        }
        else 
        {
            fprintf(stderr, "Co loi xay ra ! \n");
        }
    }
    return 0;
}