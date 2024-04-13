#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> 
#include <sys/wait.h> 

#define BUFFER_SIZE 256
#define READ_END 0
#define WRITE_END 1

void fork_forIPC() {
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE];
    int fd[2];
    pid_t pid;
    int status;

    if (pipe(fd) == -1) {
        fprintf(stderr, "Pipe failed");
        exit(1);
    }

    /* fork a child process */
    pid = fork();
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed");
        exit(1); // 오류 발생 시 종료
    }
    else if (pid == 0) { /* child process */
        close(fd[WRITE_END]);
        
        if (read(fd[READ_END], read_msg, BUFFER_SIZE) == -1) {
            fprintf(stderr, "Read from pipe failed");
        } 
        else {
            printf("read %s\n", read_msg);
        }
        close(fd[READ_END]);
        exit(0);
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
        close(fd[READ_END]);

        if (write(fd[WRITE_END], write_msg, strlen(write_msg) + 1) == -1) {
            fprintf(stderr, "Write to pipe failed");
        }
        close(fd[WRITE_END]); 
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) { // 자식 프로세스 시그널에 의해 종료 확인
            printf("Child terminated by signal %d\n", WTERMSIG(status));
        }
    }
}
