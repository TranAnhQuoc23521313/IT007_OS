#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

int x_1 = 1, x_2 = 2, x_3 = 3, x_4 = 4, x_5 = 5, x_6 = 6;
int w, v, y, z, ans;
sem_t sem_AB, sem_CD, sem_EF, sem_G;

void *ProcessAB(void *mess)
{
    while (1)
    {
        sem_wait(&sem_G);
        w = x_1 * x_2;
        v = x_3 * x_4;
        printf("(A) w = x_1 * x_2 = %d\n", w);
        printf("(B) v = x_3 * x_4 = %d\n", v);
        sem_post(&sem_AB);
    }
}

void *ProcessCD(void *mess)
{
    while (1)
    {
        sem_wait(&sem_AB);
        y = v * x_5;
        z = v * x_6;
        printf("(C) y = v * x_5 = %d\n",y);
        printf("(D) z = v * x_6 = %d\n",z);
        sem_post(&sem_CD);
    }
}

void *ProcessEF(void *mess)
{
    while (1)
    {
        sem_wait(&sem_CD);
        y = w * y;
        z = w * z;
        printf("(E) y = w * y = %d\n", y);
        printf("(F) z = w * z = %d\n", z);
        sem_post(&sem_EF);
    }
}

void *ProcessG(void *mess)
{
    while (1)
    {
        sem_wait(&sem_EF);
        ans = y + z;
        printf("(G) ans = y + z = %d\n",ans);
        printf("------------------------\n");
        sem_post(&sem_G);
    }
}

int main()
{
    pthread_t Process_AB, Process_CD, Process_EF, Process_G;
    sem_init(&sem_AB, 0, 0);
    sem_init(&sem_CD, 0, 0);
    sem_init(&sem_EF, 0, 0);
    sem_init(&sem_G, 0, 1);
    pthread_create(&Process_AB, NULL, &ProcessAB, NULL);
    pthread_create(&Process_CD, NULL, &ProcessCD, NULL);
    pthread_create(&Process_EF, NULL, &ProcessEF, NULL);
    pthread_create(&Process_G, NULL, &ProcessG, NULL);
    while (1){}
    return 0;
}