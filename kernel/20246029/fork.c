#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h> // exit 함수 위해 추가
#include <sys/wait.h> // wait 함수 위해 추가

void fork_example() {
    pid_t pid;
    /* fork a child process */
    pid = fork();
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed");
        exit(1); // 오류 발생 시 종료
    }
    else if (pid == 0) { /* child process */
        execlp("/bin/ls", "ls", NULL);
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        printf("Child Complete");
    }
}
