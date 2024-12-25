#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>

#define MAX_LINE 80 // The maximum length command
#define MAX_HISTORY 10

int number_of_args;
int fdi, fdo;
char *args[MAX_LINE / 2 + 1];
int SaveStdin, SaveStdout;
int count_HF = 0;
char Histroy_CMD[MAX_HISTORY][MAX_LINE];

// Cấp phát bộ nhớ cho mảng
void Allocate_Memory(char **args_arr)
{
    for (int i = 0; i < (MAX_LINE / 2 + 1); i++)
    {
        args_arr[i] = (char *)malloc(sizeof(char) * MAX_LINE / 2);
    }
}

// Nhập lệnh
void EnterCommand(char command[])
{
    fgets(command, MAX_LINE, stdin);
    short int len = strlen(command);
    command[len - 1] = 0;
}

// tách lệnh
void Tokernizer(char *tokens[], char *source, char *delim, int *number_of_words)
{
    char *p = strtok(source, delim);
    int count = 0;
    while (p)
    {
        strcpy(tokens[count], p);
        count++;
        p = strtok(NULL, delim);
    }
    *number_of_words = count;
    return;
}

// Kiểm tra kết thúc lệnh
void control_sig(int sign)
{
    printf("\n");
    fflush(stdout);
}

// Chuyển hướng đầu ra vào file
void redirect_output()
{
    SaveStdout = dup(STDOUT_FILENO);
    fdo = open(args[number_of_args - 1], O_CREAT | O_WRONLY | O_TRUNC);
    dup2(fdo, STDOUT_FILENO);
    free(args[number_of_args - 2]);
    args[number_of_args - 2] = NULL;
}

// Chuyển hướng đầu vào file
void redirect_input()
{
    SaveStdin = dup(STDIN_FILENO);
    fdi = open(args[number_of_args - 1], O_RDONLY);
    dup2(fdi, STDIN_FILENO);
    free(args[number_of_args - 2]);
    args[number_of_args - 2] = NULL;
}

void RestoreOut()
{
    close(fdo);
    dup2(SaveStdout, STDOUT_FILENO);
    close(SaveStdout);
}

void RestoreIn()
{
    close(fdi);
    dup2(SaveStdin, STDIN_FILENO);
    close(SaveStdin);
}

void Execute_Pile(char *parsed[], char *parsedpipe[]) {
    int pipefd[2]; // Mảng lưu file descriptors cho pipe
    pid_t p1, p2;

    // Tạo pipe
    if (pipe(pipefd) < 0) {
        perror("Pipe không thể tạo");
        return;
    }

    // Fork tiến trình đầu tiên
    p1 = fork();
    if (p1 < 0) {
        perror("Fork thất bại");
        return;
    }

    if (p1 == 0) {
        // Tiến trình con đầu tiên
        close(pipefd[0]);              // Đóng đầu đọc của pipe
        dup2(pipefd[1], STDOUT_FILENO); // Kết nối stdout với đầu ghi của pipe
        close(pipefd[1]);              // Đóng đầu ghi của pipe

        if (execvp(parsed[0], parsed) < 0) {
            perror("Không thực thi được lệnh đầu tiên");
            exit(1);
        }
    } else {
        // Fork tiến trình thứ hai
        p2 = fork();

        if (p2 < 0) {
            perror("Fork thất bại");
            return;
        }

        if (p2 == 0) {
            // Tiến trình con thứ hai
            close(pipefd[1]);              // Đóng đầu ghi của pipe
            dup2(pipefd[0], STDIN_FILENO); // Kết nối stdin với đầu đọc của pipe
            close(pipefd[0]);              // Đóng đầu đọc của pipe

            if (execvp(parsedpipe[0], parsedpipe) < 0) {
                perror("Không thực thi được lệnh thứ hai");
                exit(1);
            }
        } else {
            // Tiến trình cha
            close(pipefd[0]); // Đóng cả hai đầu của pipe
            close(pipefd[1]);

            // Chờ cả hai tiến trình con kết thúc
            waitpid(p1, NULL, 0);
            waitpid(p2, NULL, 0);
        }
    }
}

int Find_Pile_Char(char *cmd, char *cmdpiped[])
{
    int i;
    for (int i = 0; i < 2; i++)
    {
        cmdpiped[i] = strsep(&cmd, "|");
        if (cmdpiped[i] == NULL)
        {
            break;
        }
    }
    if (cmdpiped[1] == NULL)
        return 0;
    return 1;
}

void ParseCommandLine(char *cmd, char *parsedArg[])
{
    int i = 0;
    while (cmd)
    {
        parsedArg[i] = strsep(&cmd, " ");
        if (parsedArg[i][0] != 0)
            i++;
    }
    parsedArg[i] = NULL;
}

int Execute_String(char *cmd, char *parsed[],char *parsedpipe[])
{
    char *cmdpiped[2];
    int piped = 0;
    piped = Find_Pile_Char(cmd,cmdpiped);
    if (piped)
    {
        ParseCommandLine(cmdpiped[0],parsed);
        ParseCommandLine(cmdpiped[1],parsedpipe);
    }
    return piped;
}

void Add_To_History(char *cmd)
{
    if (count_HF < MAX_HISTORY)
    {
        strcpy(Histroy_CMD[count_HF],cmd);
        count_HF++;
        //printf("Da them vao HF !");
    }
    else 
    {
        for (int i = 0; i < MAX_HISTORY; i++)
        {
            strcpy(Histroy_CMD[i],Histroy_CMD[i+1]);
        }
        strcpy(Histroy_CMD[count_HF-1],cmd);
    }
}

int main(void)
{
    int should_run = 1;
    char command[MAX_LINE];
    char *first_command[MAX_LINE / 2 + 1];
    char *second_command[MAX_LINE / 2 + 1];
    int iPileExecute = 0;
    pid_t PID;
    int iRedirectIn = 0, iRedirectOut = 0;
    signal(SIGINT,control_sig);
    while (should_run)
    {
        do 
        {
            printf("\nit007sh>");
            fflush(stdout);
            EnterCommand(command);
        } while (command[0] == 0);
        if (strcmp(command,"HF") == 0)
        {
            if (count_HF == 0)
            {
                printf("NULL\n");
                continue;
            }
            else 
            {
                for (int i = 0; i < count_HF; i++)
                {
                    printf("%s \n",Histroy_CMD[i]);
                }
                continue;
            }
        }
        else 
            Add_To_History(command);
        iPileExecute = Execute_String(command,first_command,second_command);
        if (iPileExecute == 0)
        {
            Allocate_Memory(args);
            //strcpy(command,Histroy_CMD[count_HF-1]);
            Tokernizer(args,command," ",&number_of_args);
            free(args[number_of_args]);
            args[number_of_args] = NULL;
            if (strcmp(args[0],"exit")==0)
            {
                break;
            }
            else 
            {
                if ((number_of_args > 1) && (strcmp(args[number_of_args - 2],">") == 0))
                {
                    redirect_output();
                    iRedirectOut = 1;
                }
                else if ((number_of_args > 1) && (strcmp(args[number_of_args - 2],"<")==0))
                {
                    redirect_input();
                    iRedirectIn = 1;
                }
                PID = fork();
                if (PID < 0)
                {
                    fprintf(stderr,"Loi khong the tao tien trinh !\n");
                    return -1;
                }
                if (PID == 0)
                {
                    int ret = execvp(args[0],args);
                    if (ret == -1)
                    {
                        printf("Lenh khong hop le !\n");
                    }
                    exit(ret);
                }
                else 
                {
                    while (wait(NULL) != PID){}
                    if (iRedirectIn)
                    {
                        RestoreIn();
                        iRedirectIn = 0;
                    }
                    if (iRedirectOut)
                    {
                        RestoreOut();
                        iRedirectOut = 0;
                    }
                }
            }
        }
        else 
            Execute_Pile(first_command,second_command);
    }
    return 0;
}

// https://www.studocu.com/vn/document/truong-dai-hoc-cong-nghe-thong-tin-dai-hoc-quoc-gia-thanh-pho-ho-chi-minh/he-dieu-hanh/lab6-22520714-he-dieu-hanh-thuc-hanh/111257943?origin=course-trending-2