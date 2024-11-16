#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SORT_BY_ARRIVAL 0
#define SORT_BY_PID 1
#define SORT_BY_BURST 2
#define SORT_BY_START 3
typedef struct
{
    int iPID;
    int iArrival, iBurst;
    int iStart, iFinish, iWaiting, iResponse, iTaT;
} PCB;

void inputProcess(int n, PCB P[]);
void printProcess(int n, PCB P[]);
void exportGanttChart(int n, PCB P[]);
void pushProcess(int *n, PCB P[], PCB Q);
void removeProcess(int *n, int index, PCB P[]);
int swapProcess(PCB *P, PCB *Q);
int partition(PCB P[], int low, int high, int iCriteria);
void quickSort(PCB P[], int low, int high, int iCriteria);
void calculateAWT(int n, PCB P[]);
void calculateATaT(int n, PCB P[]);
void CreateFreeProcess(PCB *Q);

int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[10];
    int iNumberOfProcess;
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    // printf("Sorting Good !\n");
    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);
    ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
    ReadyQueue[0].iFinish = ReadyQueue[0].iStart +
                            ReadyQueue[0].iBurst;
    ReadyQueue[0].iResponse = ReadyQueue[0].iStart -
                              ReadyQueue[0].iArrival;
    ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
    ReadyQueue[0].iTaT = ReadyQueue[0].iFinish -
                         ReadyQueue[0].iArrival;
    printf("\nReady Queue:\n ");
    printProcess(1, ReadyQueue);
    while (iTerminated < iNumberOfProcess)
    {
        int Status = 0; // NOT FREE
        while (iRemain > 0)
        {
            if (iReady == 0)
            {
                // printf("Xay ra thoi gian ranh ! \n");
                Status = 1;
                break;
            }
            if (Input[0].iArrival <= ReadyQueue[0].iFinish)
            {
                // printf("ADDING NEW INPUT INTO READYQUEUE ! \n");
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                quickSort(ReadyQueue, 1, iReady - 1, SORT_BY_BURST);
                // printf("READY QUEUE \n");
                // printProcess(iReady-1,ReadyQueue);
                continue;
            }
            else
            {
                // printf("CANNOT ADDING NEW INPUT INTO READYQUEUE \n");
                break;
            }
        }
        if (Status == 1)
        {
            pushProcess(&iReady, ReadyQueue, Input[0]);
            removeProcess(&iRemain, 0, Input);
            ReadyQueue[0].iStart = ReadyQueue[0].iArrival;
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart + ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart - ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish - ReadyQueue[0].iArrival;
            Status = 0;
        }
        else if (iReady > 0)
        {
            pushProcess(&iTerminated, TerminatedArray,
                        ReadyQueue[0]);
            removeProcess(&iReady, 0, ReadyQueue);
            ReadyQueue[0].iStart = TerminatedArray[iTerminated - 1].iFinish;
            ReadyQueue[0].iFinish = ReadyQueue[0].iStart +
                                    ReadyQueue[0].iBurst;
            ReadyQueue[0].iResponse = ReadyQueue[0].iStart -
                                      ReadyQueue[0].iArrival;
            ReadyQueue[0].iWaiting = ReadyQueue[0].iResponse;
            ReadyQueue[0].iTaT = ReadyQueue[0].iFinish -
                                 ReadyQueue[0].iArrival;
        }
    }
    printf("\n===== SJF Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    quickSort(TerminatedArray, 0, iTerminated - 1,
              SORT_BY_PID);
    calculateAWT(iTerminated, TerminatedArray);
    calculateATaT(iTerminated, TerminatedArray);
    return 0;
}

void inputProcess(int n, PCB P[])
{
    srand(time(0));
    for (int i = 0; i < n; i++)
    {
        // printf("PID | Arrival Time | Burst Time of process: \n");
        P[i].iPID = i + 1;
        P[i].iArrival = rand() % 21;
        P[i].iBurst = rand() % 11 + 2;
        P[i].iStart = 0;
        P[i].iFinish = 0;
        P[i].iWaiting = 0;
        P[i].iResponse = 0;
        P[i].iTaT = 0;
    }
    P[0].iPID = 1; P[0].iArrival = 19; P[0].iBurst = 8;
    P[1].iPID = 2; P[1].iArrival = 12; P[1].iBurst = 7;
    P[2].iPID = 3; P[2].iArrival = 11; P[2].iBurst = 2;
    P[3].iPID = 4; P[3].iArrival = 12; P[3].iBurst = 9;
    P[4].iPID = 5; P[4].iArrival = 12; P[4].iBurst = 7;
    for (int i = 0; i < n; i++)
    {
        printf("PID | Arrival Time | Burst Time of process: %d | %d | %d \n", P[i].iPID, P[i].iArrival, P[i].iBurst);
    }
}

void printProcess(int n, PCB P[])
{
    printf("PID | Arrival |  Burst | Start | Finish | Waiting | Response | TaT \n");
    for (int i = 0; i < n; i++)
    {
        printf(" %d \t %d \t   %d  \t   %d  \t   %d  \t   %d  \t    %d    \t%d  \n",
               P[i].iPID, P[i].iArrival, P[i].iBurst, P[i].iStart, P[i].iFinish, P[i].iWaiting, P[i].iResponse, P[i].iTaT);
    }
}

void quickSort(PCB P[], int low, int high, int iCriteria)
{
    partition(P, low, high, iCriteria);
}

int partition(PCB P[], int low, int high, int iCriteria)
{
    int i = low, j = high;
    PCB pivot = P[(low + high) / 2];
    while (i <= j)
    {
        if (iCriteria == 0)
        {
            while (P[i].iArrival < pivot.iArrival ||
                   (P[i].iArrival == pivot.iArrival && P[i].iPID < pivot.iPID)) i++;
            while (P[j].iArrival > pivot.iArrival ||
                   (P[j].iArrival == pivot.iArrival && P[j].iPID > pivot.iPID)) j--;
            if (i <= j)
            {
                swapProcess(&P[i], &P[j]);
                i++;
                j--;
            }
        }
        if (iCriteria == 1)
        {
            while (P[i].iPID < pivot.iPID) i++;
            while (P[j].iPID > pivot.iPID) j--;
            if (i <= j)
            {
                swapProcess(&P[i], &P[j]);
                i++;
                j--;
            }
        }
        if (iCriteria == 2)
        {
            while ((P[i].iBurst < pivot.iBurst) ||
                   (P[i].iBurst == pivot.iBurst && P[i].iArrival < pivot.iArrival) ||
                   (P[i].iBurst == pivot.iBurst && P[i].iArrival == pivot.iArrival && P[i].iPID < pivot.iPID)) i++;
            while ((P[j].iBurst > pivot.iBurst) ||
                   (P[j].iBurst == pivot.iBurst && P[j].iArrival > pivot.iArrival) ||
                   (P[j].iBurst == pivot.iBurst && P[j].iArrival == pivot.iArrival && P[j].iPID > pivot.iPID)) j--;
            if (i <= j)
            {
                swapProcess(&P[i], &P[j]);
                i++;
                j--;
            }
        }
    }
    if (low < j)
        partition(P, low, j, iCriteria);
    if (high > i)
        partition(P, i, high, iCriteria);
    return 0;
}

int swapProcess(PCB *P, PCB *Q)
{
    PCB temp = *P;
    *P = *Q;
    *Q = temp;
    return 0;
}

void pushProcess(int *n, PCB P[], PCB Q)
{
    P[*n] = Q;
    *n = *n + 1;
}

void removeProcess(int *n, int index, PCB P[])
{
    for (int i = index; i < *n; i++)
    {
        P[i] = P[i + 1];
    }
    *n = *n - 1;
}

void exportGanttChart(int n, PCB P[])
{
    printProcess(n, P);
    printf("Biểu đồ Gantt:\n");
    for (int i = 0; i < n; i++)
    {
        printf("| P%d %d -> %d ", P[i].iPID, P[i].iStart, P[i].iFinish);
    }
    printf("|\n");
}

void calculateAWT(int n, PCB P[])
{
    float AVERAGE = 0;
    for (int i = 0; i < n; i++)
    {
        AVERAGE += (float)P[i].iWaiting;
    }
    AVERAGE /= n;
    printf("AWT : %f \n", AVERAGE);
}

void calculateATaT(int n, PCB P[])
{
    float AVERAGE = 0;
    for (int i = 0; i < n; i++)
    {
        AVERAGE += (float)P[i].iTaT;
    }
    AVERAGE /= n;
    printf("ATaT : %f \n", AVERAGE);
}

// TEST CASE 1
/*  P[0].iPID = 1; P[0].iArrival = 0; P[0].iBurst = 1;
    P[1].iPID = 2; P[1].iArrival = 1; P[1].iBurst = 2;
    P[2].iPID = 3; P[2].iArrival = 2; P[2].iBurst = 3;
    P[3].iPID = 4; P[3].iArrival = 3; P[3].iBurst = 4;
    P[4].iPID = 5; P[4].iArrival = 4; P[4].iBurst = 5; */

// TEST CASE 2
/*  P[0].iPID = 1; P[0].iArrival = 11; P[0].iBurst = 2;
    P[1].iPID = 2; P[1].iArrival = 8; P[1].iBurst = 4;
    P[2].iPID = 3; P[2].iArrival = 9; P[2].iBurst = 1;
    P[3].iPID = 4; P[3].iArrival = 3; P[3].iBurst = 4;
    P[4].iPID = 5; P[4].iArrival = 19; P[4].iBurst = 5; */

// TEST CASE 3
/*  P[0].iPID = 1; P[0].iArrival = 19; P[0].iBurst = 8;
    P[1].iPID = 2; P[1].iArrival = 12; P[1].iBurst = 7;
    P[2].iPID = 3; P[2].iArrival = 11; P[2].iBurst = 2;
    P[3].iPID = 4; P[3].iArrival = 12; P[3].iBurst = 9;
    P[4].iPID = 5; P[4].iArrival = 12; P[4].iBurst = 7;*/