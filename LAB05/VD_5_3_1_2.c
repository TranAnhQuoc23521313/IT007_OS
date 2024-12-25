#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdio.h>

int products = 0, sells = 0;
sem_t Sem_Product,Sem_Sell;

void *Process_Sell(void *mess)
{
    while(1)
    {
        sem_wait(&Sem_Sell);
        sells++;
        printf("SELL: %d\n",sells);
        sem_post(&Sem_Product);
    }
}

void *Process_Product(void *mess)
{
    while (1)
    {
        sem_wait(&Sem_Product);
        products++;
        printf("PRODUCT: %d\n",products);
        sem_post(&Sem_Sell);
    }
    
}

int main()
{
    sem_init(&Sem_Sell,0,0);
    sem_init(&Sem_Product,0,1313);
    pthread_t Products,Sells;
    pthread_create(&Products,NULL,&Process_Product,NULL);
    pthread_create(&Sells,NULL,&Process_Sell,NULL);
    while (1){}
    return 0; 
}