#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

void non_function(){
    printf("\nCount.sh has stopped !");
}

int main(int argc,char* argv[])
{
    printf("Welcome to IT007, I am 23521313 ! \n");
    signal(SIGINT,non_function);
    system("./count.sh 120");
    exit(0);
}