#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid; // Process ID
    int arrivalTime; // 도착 시간
    int burstTime; // 실행 시간
    int waitingTime; // 대기 시간
    int turnaroundTime; // 전체 실행 시간
    int remainingTime; // 남은 실행 시간
};

void calculateTime(struct Process *processes, int count, int quantum) {
    int time = 0, completed = 0;
    while (completed < count) {
        for (int i = 0; i < count; i++) {
            if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0) {
                if (processes[i].remainingTime > quantum) {
                    time += quantum;
                    processes[i].remainingTime -= quantum;
                } else {
                    time += processes[i].remainingTime;
                    processes[i].waitingTime = time - processes[i].arrivalTime - processes[i].burstTime;
                    processes[i].turnaroundTime = time - processes[i].arrivalTime;
                    processes[i].remainingTime = 0;
                    completed++;
                }
            }
        }
    }
}

void Round_Robin() {
    int count, quantum;
    printf("프로세스의 개수를 입력하세요: ");
    scanf("%d", &count);
    struct Process *processes = (struct Process*) malloc(sizeof(struct Process) * count);

    for (int i = 0; i < count; i++) {
        processes[i].pid = i + 1;
        printf("프로세스 %d의 도착 시간과 실행 시간을 입력하세요: ", i + 1);
        scanf("%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
        processes[i].remainingTime = processes[i].burstTime;
    }

    printf("타임 퀀텀을 입력하세요: ");
    scanf("%d", &quantum);

    calculateTime(processes, count, quantum);

    printf("\nPID\tBurst Time\tWaiting Time\tTurnaround Time\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%d\t\t%d\t\t%d\n", processes[i].pid, processes[i].burstTime, processes[i].waitingTime, processes[i].turnaroundTime);
    }

    int totalWaitingTime = 0, totalTurnaroundTime = 0;
    for (int i = 0; i < count; i++) {
        totalWaitingTime += processes[i].waitingTime;
        totalTurnaroundTime += processes[i].turnaroundTime;
    }

    printf("평균 대기 시간: %.2f\n", (float)totalWaitingTime / count);
    printf("평균 전체 실행 시간: %.2f\n", (float)totalTurnaroundTime / count);

    free(processes);
}
