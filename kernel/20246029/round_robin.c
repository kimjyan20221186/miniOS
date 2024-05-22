#include <stdio.h>
#include <stdlib.h>

struct Process {
    int pid; // 프로세스 ID
    int arrivalTime; // 도착 시간
    int burstTime; // 실행 시간
    int waitingTime; // 대기 시간
    int turnaroundTime; // 총 실행 시간 (반환 시간)
    int remainingTime; // 남은 실행 시간
};

// 프로세스들의 도착 시간을 기준으로 정렬하는 함수
void sortProcessesByArrival(struct Process* processes, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (processes[j].arrivalTime > processes[j + 1].arrivalTime) {
                struct Process temp = processes[j];
                processes[j] = processes[j + 1];
                processes[j + 1] = temp;
            }
        }
    }
}

void calculateTime(struct Process *processes, int count, int quantum) {
    int time = 0;
    int remainingProcesses = count;
    int currentIndex = -1;

    int completed[count];
    for (int i = 0; i < count; i++) {
        completed[i] = 0;
    }

    while (remainingProcesses > 0) {
        int foundProcess = 0;

        for (int i = 0; i < count; i++) {
            if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0) {
                currentIndex = i;
                foundProcess = 1;
                break;
            }
        }

        if (foundProcess) {
            printf("현재 시간 %d: 프로세스 p%d 실행 중\n", time, processes[currentIndex].pid);

            // 현재 프로세스를 타임 퀀텀만큼 실행
            if (processes[currentIndex].remainingTime > quantum) {
                time += quantum;
                processes[currentIndex].remainingTime -= quantum;
            } else {
                // 남은 시간보다 타임 퀀텀이 더 길거나 같을 때
                time += processes[currentIndex].remainingTime;
                processes[currentIndex].remainingTime = 0;
                processes[currentIndex].waitingTime = time - processes[currentIndex].burstTime - processes[currentIndex].arrivalTime;
                processes[currentIndex].turnaroundTime = time - processes[currentIndex].arrivalTime;
                completed[currentIndex] = 1;
                remainingProcesses--;
                printf("프로세스 p%d 종료\n", processes[currentIndex].pid);
            }

            // 다음 타임 퀀텀에 선점될 프로세스 찾기
            for (int i = 0; i < count; i++) {
                if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0 && !completed[i]) {
                    printf("다음 타임 퀀텀에 프로세스 p%d 선점\n", processes[i].pid);
                    break;
                }
            }
        } else {
            time++;
        }
    }
}

void Round_Robin() {
    int count, quantum;
    printf("프로세스의 수를 입력하세요: ");
    scanf("%d", &count);
    struct Process *processes = (struct Process*) malloc(sizeof(struct Process) * count);

    for (int i = 0; i < count; i++) {
        printf("프로세스 %d의 도착 시간과 실행 시간을 입력하세요: ", i + 1);
        scanf("%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
        processes[i].pid = i + 1;
        processes[i].waitingTime = 0;
        processes[i].turnaroundTime = 0;
        processes[i].remainingTime = processes[i].burstTime;
    }

    printf("타임 퀀텀을 입력하세요: ");
    scanf("%d", &quantum); // 타임 퀀텀 입력

    sortProcessesByArrival(processes, count); // 프로세스를 도착 시간 기준으로 정렬
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
    printf("평균 반환 시간: %.2f\n", (float)totalTurnaroundTime / count);

    free(processes);
}
