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

/* Hàm này dùng để sau chép 1 mảng sang 1 mảng khác*/
void CopyArray(PCB P[], PCB Q[], int n);

/* Hàm này dùng để gom các đoạn tiến trình con lại thành 1
và tính các khoảng thời gian cần thiết của từng tiến trình */
void GroupProcess(int n, PCB P[], int m, PCB Q[]);

/* Tạo ra 1 hàm tạo 1 tiến trình rảnh rỗi của hệ thống */
PCB CreateFreeProcess();

int main()
{
    PCB Input[10];
    PCB ReadyQueue[10];
    PCB TerminatedArray[20];

    // Dùng để lưu gọn các thời gian như waiting, response, tat của từng tiến trình cho gọn nhất
    PCB CalculatedArray[10];

    int iNumberOfProcess;
    printf("Please input number of Process: ");
    scanf("%d", &iNumberOfProcess);
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0, q_time;
    printf("Please input quantumn time: ");
    scanf("%d",&q_time);
    inputProcess(iNumberOfProcess, Input);
    CopyArray(Input, CalculatedArray, iNumberOfProcess);
    // printProcess(iNumberOfProcess, CalculatedArray);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);
    printf("\nReady Queue:\n ");
    printProcess(1, ReadyQueue);
    int Status = 0;
    while (1)
    {
        if (iReady == 0 && iRemain == 0)
            break;
        PCB Current_Process = ReadyQueue[0];
        removeProcess(&iReady, 0, ReadyQueue);
        if (Current_Process.iBurst >= q_time) // Nếu tiến trình chạy hết q_time mà còn iBurst
        {
            if (iTerminated == 0)
                Current_Process.iStart = Current_Process.iArrival;
            else
                Current_Process.iStart = TerminatedArray[iTerminated - 1].iFinish;
            Current_Process.iFinish = Current_Process.iStart + (q_time);
            Current_Process.iBurst -= q_time;
            pushProcess(&iTerminated, TerminatedArray, Current_Process);
        }
        else if (Current_Process.iBurst < q_time) // Nếu iBurst của tiến trình đang chạy ít hơn q_time
        {
            if (iTerminated == 0)
                Current_Process.iStart = Current_Process.iArrival;
            else
                Current_Process.iStart = TerminatedArray[iTerminated - 1].iFinish;
            Current_Process.iFinish = Current_Process.iStart + Current_Process.iBurst;
            Current_Process.iBurst = 0; // Do chắc chắn chạy hết nên sau khi chạy xong iBurst = 0
            pushProcess(&iTerminated, TerminatedArray, Current_Process);
        }

        while (iRemain > 0) // Tiến hành thêm các tiến trình khác vào sau chạy xong 1 đoạn q_time hoặc hết 1 tiến trình.
        {
            if (Input[0].iArrival <= TerminatedArray[iTerminated - 1].iFinish)
            {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                Status = 1; // Có công việc chuẩn bị thực hiện
            }
            else if (Status == 0)   // Hệ thống đang rảnh rỗi
            {
                PCB Temp = CreateFreeProcess();
                Temp.iStart = TerminatedArray[iTerminated - 1].iFinish;
                Temp.iFinish = Temp.iStart + q_time;
                pushProcess(&iTerminated, TerminatedArray, Temp);
                Status = 1; // Skip qua thời điểm rảnh rỗi.
                continue;
            }
            else
                break;
        }
        if (Current_Process.iBurst > 0)
            pushProcess(&iReady, ReadyQueue, Current_Process);
    }
    printf("\n===== Round Robin Scheduling =====\n");
    exportGanttChart(iTerminated, TerminatedArray);
    // quickSort(TerminatedArray, 0, iTerminated - 1, SORT_BY_PID);
    GroupProcess(iTerminated, TerminatedArray, iNumberOfProcess, CalculatedArray);
    printf("CALCULATEDARRAY:\n");
    printProcess(iNumberOfProcess, CalculatedArray);
    calculateAWT(iNumberOfProcess, CalculatedArray);
    calculateATaT(iNumberOfProcess, CalculatedArray);
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
    P[0].iPID = 1;
    P[0].iArrival = 0;
    P[0].iBurst = 3;

    P[1].iPID = 2;
    P[1].iArrival = 3;
    P[1].iBurst = 5;

    P[2].iPID = 3;
    P[2].iArrival = 8;
    P[2].iBurst = 3;

    P[3].iPID = 4;
    P[3].iArrival = 11;
    P[3].iBurst = 3;

    P[4].iPID = 5;
    P[4].iArrival = 14;
    P[4].iBurst = 8;
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
        if (P[i].iPID == -1)
            continue;
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
                   (P[i].iArrival == pivot.iArrival && P[i].iPID < pivot.iPID))
                i++;
            while (P[j].iArrival > pivot.iArrival ||
                   (P[j].iArrival == pivot.iArrival && P[j].iPID > pivot.iPID))
                j--;
            if (i <= j)
            {
                swapProcess(&P[i], &P[j]);
                i++;
                j--;
            }
        }
        if (iCriteria == 1)
        {
            while (P[i].iPID < pivot.iPID)
                i++;
            while (P[j].iPID > pivot.iPID)
                j--;
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
                   (P[i].iBurst == pivot.iBurst && P[i].iArrival == pivot.iArrival && P[i].iPID < pivot.iPID))
                i++;
            while ((P[j].iBurst > pivot.iBurst) ||
                   (P[j].iBurst == pivot.iBurst && P[j].iArrival > pivot.iArrival) ||
                   (P[j].iBurst == pivot.iBurst && P[j].iArrival == pivot.iArrival && P[j].iPID > pivot.iPID))
                j--;
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
    PCB Current_Process = *P;
    *P = *Q;
    *Q = Current_Process;
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
    //printProcess(n, P);
    printf("Biểu đồ Gantt:\n");
    for (int i = 0; i < n; i++)
    {
        if (P[i].iPID == -1)
            continue;
        printf("| P%d %d -> %d ", P[i].iPID, P[i].iStart, P[i].iFinish);
    }
    printf("|\n");
}

void GroupProcess(int n, PCB P[], int m, PCB Q[])
{
    for (int i = 0; i < m; i++)
    {
        for (int j = n - 1; j >= 0; j--)
        {
            if (P[j].iPID == Q[i].iPID)
            {
                Q[i].iFinish = P[j].iFinish;
                break;
            }
        }
        for (int j = 0; j < n; j++)
        {
            if (P[j].iPID == Q[i].iPID)
            {
                Q[i].iStart = P[j].iStart;
                break;
            }
        }
    }

    for (int i = 0; i < m; i++)
    {
        Q[i].iWaiting = Q[i].iFinish - Q[i].iArrival - Q[i].iBurst;
        Q[i].iResponse = Q[i].iStart - Q[i].iArrival;
        Q[i].iTaT = Q[i].iFinish - Q[i].iArrival;
    }
}

void calculateAWT(int n, PCB P[])
{
    double AVERAGE = 0;
    for (int i = 0; i < n; i++)
    {
        AVERAGE += (double)P[i].iWaiting;
    }
    AVERAGE /= n;
    printf("AWT : %f \n", AVERAGE);
}

void calculateATaT(int n, PCB P[])
{
    double AVERAGE = 0;
    for (int i = 0; i < n; i++)
    {
        AVERAGE += (double)P[i].iTaT;
    }
    AVERAGE /= n;
    printf("ATaT : %f \n", AVERAGE);
}

void CopyArray(PCB P[], PCB Q[], int n)
{
    for (int i = 0; i < n; i++)
        Q[i] = P[i];
}

PCB CreateFreeProcess()
{
    PCB Q;
    Q.iArrival = 0;
    Q.iBurst = 0;
    Q.iFinish = 0;
    Q.iStart = 0;
    Q.iPID = -1;
    Q.iWaiting = 0;
    Q.iResponse = 0;
    Q.iTaT = 0;
    return Q;
}

/*  P[0].iPID = 1;
    P[0].iArrival = 0;
    P[0].iBurst = 13;

    P[1].iPID = 2;
    P[1].iArrival = 4;
    P[1].iBurst = 9;

    P[2].iPID = 3;
    P[2].iArrival = 6;
    P[2].iBurst = 4;

    P[3].iPID = 4;
    P[3].iArrival = 7;
    P[3].iBurst = 20;

    P[4].iPID = 5;
    P[4].iArrival = 12;
    P[4].iBurst = 10;

    P[5].iPID = 6;
    P[5].iArrival = 57;
    P[5].iBurst = 1;*/

/*  P[0].iPID = 1;
    P[0].iArrival = 0;
    P[0].iBurst = 10;

    P[1].iPID = 2;
    P[1].iArrival = 13;
    P[1].iBurst = 3;

    P[2].iPID = 3;
    P[2].iArrival = 12;
    P[2].iBurst = 6;

    P[3].iPID = 4;
    P[3].iArrival = 12;
    P[3].iBurst = 10;

    P[4].iPID = 5;
    P[4].iArrival = 13;
    P[4].iBurst = 12;

    P[5].iPID = 6;
    P[5].iArrival = 57;
    P[5].iBurst = 1;*/