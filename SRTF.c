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
    int iRemain = iNumberOfProcess, iReady = 0, iTerminated = 0;
    inputProcess(iNumberOfProcess, Input);
    CopyArray(Input, CalculatedArray, iNumberOfProcess);
    //printProcess(iNumberOfProcess, CalculatedArray);
    quickSort(Input, 0, iNumberOfProcess - 1, SORT_BY_ARRIVAL);
    pushProcess(&iReady, ReadyQueue, Input[0]);
    removeProcess(&iRemain, 0, Input);
    printf("\nReady Queue:\n ");
    printProcess(1, ReadyQueue);
    int Delta_Time; // Biến này dùng để tính thời gian có thể chạy trước khi bắt đầu trưng dụng
    while (1)
    {
        if (iReady == 0 && iRemain == 0)
            break;
        if (iRemain > 0) // Dùng để xác định độ chênh lệch thời gian ( khoảng trưng dụng )
        {
            if (iReady == 0) // Xảy ra tình trạng hệ thống bị rảnh rỗi
            {
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                //quickSort(ReadyQueue,0,iReady-1,SORT_BY_BURST);
                PCB Temp = CreateFreeProcess();
                Temp.iStart = TerminatedArray[iTerminated - 1].iFinish;
                Temp.iFinish = ReadyQueue[0].iArrival;
                pushProcess(&iTerminated, TerminatedArray, Temp);
                continue;
            }
            /* Nếu như đã có 1 tiến trình chạy đến thời điểm cần trưng dụng thì ta
            cập nhật Delta_Time bằng Thời gian bắt đầu trưng dung - thời gian kết thúc tiến trình trước */
            else if (iTerminated >= 1)
            {
                Delta_Time = Input[0].iArrival - TerminatedArray[iTerminated - 1].iFinish;
            }
            /* Nếu như chưa có tiến trình nào chạy, thì ta tính Delta_Time bằng thời gian bắt đầu trưng dụng - thời gian bắt đầu tiến trình*/
            else if (iTerminated == 0)
            {
                Delta_Time = Input[0].iArrival - ReadyQueue[0].iArrival;
            }
        }
        // Nếu như không còn tiến trình nào cần trưng dụng nữa thì cho các tiến trình trong ReadyQueue chạy đến hết theo Burst nhỏ nhất
        else if (iRemain == 0) 
        {
            if (iReady > 0)
            {
                PCB Temp = ReadyQueue[0];
                Temp.iStart = TerminatedArray[iTerminated - 1].iFinish;
                Temp.iFinish = Temp.iStart + Temp.iBurst;
                pushProcess(&iTerminated, TerminatedArray, Temp);
                removeProcess(&iReady, 0, ReadyQueue);
                quickSort(ReadyQueue, 0, iReady - 1, SORT_BY_BURST);
                continue;
            }
        }
        if (Delta_Time == 0)
        {
            pushProcess(&iReady,ReadyQueue,Input[0]);
            removeProcess(&iRemain,0,Input);
            continue;
        }
        else if (Delta_Time <= ReadyQueue[0].iBurst) // Kiểm tra xem có thể trưng dụng toàn bộ không
        {
            PCB Temp = ReadyQueue[0];
            // Nếu như đã có tiến trình chạy xong thì ta cập nhật thời gian bắt đầu của tiến trình mới = thời gian kết thúc của tiến trình cũ
            if (iTerminated >= 1)
            {
                Temp.iStart = TerminatedArray[iTerminated - 1].iFinish;
            }
            // Nếu như không có tiến trình nào đã chạy thì mặc định Start của nó là Arrival của chính nó
            else if (iTerminated == 0)
            {
                Temp.iStart = ReadyQueue[0].iArrival;
            }
            Temp.iFinish = Temp.iStart + Delta_Time;
            pushProcess(&iTerminated, TerminatedArray, Temp);
            ReadyQueue[0].iBurst -= Delta_Time;
            if (ReadyQueue[0].iBurst <= 0) // Nêu như chạy hết đoạn thời gian trên mà hết Burst thì ta đưa nó ra khỏi ReadyQueue
                removeProcess(&iReady, 0, ReadyQueue);
            pushProcess(&iReady, ReadyQueue, Input[0]);
            removeProcess(&iRemain, 0, Input);
            quickSort(ReadyQueue, 0, iReady - 1, SORT_BY_BURST);
            continue;
        }
        else if (Delta_Time > ReadyQueue[0].iBurst) // Nếu như không trưng dụng toàn bộ thì có cần trưng dụng 1 đoạn nhỏ không
        {
            PCB Temp = ReadyQueue[0];
            if (iTerminated == 0)
            {
                Temp.iStart = ReadyQueue[0].iArrival;
                Temp.iFinish = Temp.iStart + ReadyQueue[0].iBurst;
                pushProcess(&iTerminated,TerminatedArray,Temp);
                removeProcess(&iReady,0,ReadyQueue);
                continue;
            }
            // Trường hợp này trưng dụng ngay khi đã chạy xong tiến trình
            if (TerminatedArray[iTerminated - 1].iFinish + ReadyQueue[0].iBurst <= Input[0].iArrival)
            {
                Temp.iStart = TerminatedArray[iTerminated - 1].iFinish;
                Temp.iFinish = Temp.iStart + ReadyQueue[0].iBurst;
                removeProcess(&iReady, 0, ReadyQueue);
                pushProcess(&iTerminated, TerminatedArray, Temp);
                continue;
            }
            // Trường hợp này tiến trình chỉ chạy đến thời điểm trưng dụng của tiến trình kế tiếp
            else if (TerminatedArray[iTerminated - 1].iFinish + ReadyQueue[0].iBurst > Input[0].iArrival)
            {
                Temp.iStart = TerminatedArray[iTerminated - 1].iFinish;
                Temp.iFinish = Temp.iStart + (Input[0].iArrival - ReadyQueue[0].iBurst);
                ReadyQueue[0].iBurst -= Temp.iFinish - Temp.iStart;
                pushProcess(&iReady, ReadyQueue, Input[0]);
                removeProcess(&iRemain, 0, Input);
                pushProcess(&iTerminated, TerminatedArray, Temp);
                continue;
            }
        }
    }
    printf("\n===== SRTF Scheduling =====\n");
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
    /*P[0].iPID = 1;
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
    P[4].iBurst = 12; */
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
       // if (P[i].iPID == -1)
         //   continue;
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
    P[3].iBurst = 18;

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