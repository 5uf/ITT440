#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

void sigint_handler(int sig){
printf("Signal Interrupt detected! \n");
}

int main(){

    int fd[2]; // Used to store two ends of pipe
	int fd2[2];
    int fd3[2];

    if (pipe(fd)==-1||pipe(fd2)==-1||pipe(fd3)==-1){
	  	 exit(EXIT_FAILURE);
		 return 1; }

    int n1 = fork();  // Creating child process
    int n2 = fork();
    char input[100];
    char input2[100];
    char input3[100];

    if (signal(SIGINT, sigint_handler) == SIG_ERR){
        perror("signal");
        exit(1); }

    if (n1 > 0 && n2 > 0) { //parent process
        close(fd[0]);
        close(fd2[0]);
        close(fd3[0]);
        
		printf("Input 1st string: ");
		scanf("%s",&input);
        printf("Input 2nd string: ");
		scanf("%s",&input2);
        printf("Input 3nd string: ");
		scanf("%s",&input3);
        
        write(fd[1], input, sizeof(input));
        write(fd2[1], input2, sizeof(input));
		write(fd3[1], input3, sizeof(input));
        
		close(fd[1]);
        close(fd2[1]);
        close(fd3[1]);
        
    } else if (n1 == 0 && n2 > 0){ //1st child process
        close(fd[1]);
		read(fd[0], input, sizeof(input));
		printf("\n1st child process: %s ", &input); 
        close(fd[0]);
    } else if (n1 > 0 && n2 == 0) { // 2nd child process
        close(fd2[1]);
		read(fd2[0], input2, sizeof(input2));
		printf("\n2nd child process: %s ", &input2);
        close(fd2[0]);
    } else {           //3rd child process
        close(fd3[1]);
		read(fd3[0], input3, sizeof(input3));
		printf("\n3rd child process: %s \n", &input3);
        close(fd3[0]);
    }
    return 0;
}