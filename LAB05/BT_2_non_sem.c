#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>

int *a;
int iNumber = 0;
int n;

void *Process_Array(void *mess)
{
    while (1)
    {
        srand(time(NULL));
        if (iNumber < n)
        {
            a[iNumber] = rand() % 100;
            iNumber++;
            printf("So phan tu trong A: %d \n", iNumber);
        }
    }
}

void Delete_Element(int *a, int x)
{
    if (x == iNumber) // Xoá phần tử ở cuối mảng
    {
        iNumber--;
    }
    else
    {
        for (int i = x; i < n; i++)
        {
            a[i] = a[i + 1];
        }
        iNumber--;
    }
}

void *Proces_Delete(void *mess)
{
    while (1)
    {
        // srand((int)time(0));
        if (iNumber == 0)
        {
            printf("Nothing in Array A\n");
        }
        else
        {
            int pos = 1;
            Delete_Element(a, pos);
            printf("So phan tu trong A sau khi lay phan tu x la: %d \n", iNumber);
        }
    }
}

int main()
{
    printf("Nhap vao N: ");
    scanf("%d", &n);
    pthread_t Process_Create_Array, Process_Takout;
    a = (int *)malloc(n * sizeof(int));
    pthread_create(&Process_Create_Array, NULL, &Process_Array, NULL);
    pthread_create(&Process_Takout, NULL, &Proces_Delete, NULL);
    while (1)
    {
    }
    return 0;
}