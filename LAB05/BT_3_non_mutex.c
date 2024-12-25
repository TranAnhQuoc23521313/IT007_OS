#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int x = 0;

void *ProcessA(void* mess)
{
    while (1)
    {
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("x(P_A) = %d \n",x);
    }
}

void *ProcessB(void* mess)
{
    while (1)
    {
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("x(P_B) = %d \n",x);
    }
}

int main()
{
    pthread_t Process_A,Process_B;
    pthread_create(&Process_A,NULL,&ProcessA,NULL);
    pthread_create(&Process_B,NULL,&ProcessB,NULL);
    while(1){};
    return 0;
}