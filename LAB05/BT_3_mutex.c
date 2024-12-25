#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int x = 0;
pthread_mutex_t mutex;

void *ProcessA(void* mess)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("x(P_A) = %d \n",x);
        pthread_mutex_unlock(&mutex);
    }
}

void *ProcessB(void* mess)
{
    while (1)
    {
        pthread_mutex_lock(&mutex);
        x = x + 1;
        if (x == 20)
            x = 0;
        printf("x(P_B) = %d \n",x);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_t Process_A,Process_B;
    pthread_mutex_init(&mutex,NULL);
    pthread_create(&Process_A,NULL,&ProcessA,NULL);
    pthread_create(&Process_B,NULL,&ProcessB,NULL);
    while(1){};
    return 0;
}