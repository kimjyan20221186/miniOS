#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int arrival_time;
    int remaining_time;
} Process;

void calculateTime(Process *processes, int count, int quantum) {
    int time = 0, i;
    int all_done = 0;
    while (!all_done) {
        all_done = 1;
        for (i = 0; i < count; i++) {
            if (processes[i].remaining_time > 0) {
                all_done = 0; 
                if (processes[i].remaining_time > quantum) {
                    time += quantum;
                    processes[i].remaining_time -= quantum;
                } else {
                    time += processes[i].remaining_time;
                    processes[i].waiting_time = time - processes[i].burst_time - processes[i].arrival_time;
                    processes[i].turnaround_time = time - processes[i].arrival_time;
                    processes[i].remaining_time = 0;
                }
            }
        }
    }
}

void Round_Robin() {
    int i, count, quantum;
    float total_waiting_time = 0, total_turnaround_time = 0;

    printf("프로세스의 수를 입력하세요: ");
    scanf("%d", &count);
    Process *processes = (Process *)malloc(count * sizeof(Process));

    for (i = 0; i < count; i++) {
        printf("\n프로세스 %d의 도착 시간: ", i + 1);
        scanf("%d", &processes[i].arrival_time);
        printf("프로세스 %d의 실행 시간(버스트 타임): ", i + 1);
        scanf("%d", &processes[i].burst_time);
        processes[i].id = i + 1;
        processes[i].remaining_time = processes[i].burst_time;
    }

    printf("\n타임 퀀텀을 입력하세요: ");
    scanf("%d", &quantum);

    calculateTime(processes, count, quantum);

    printf("\n프로세스 ID\t버스트 타임\t대기 시간\t전체 실행 시간\n");
    for (i = 0; i < count; i++) {
        total_waiting_time += processes[i].waiting_time;
        total_turnaround_time += processes[i].turnaround_time;
        printf("%d\t\t%d\t\t%d\t\t%d\n", processes[i].id, processes[i].burst_time, processes[i].waiting_time, processes[i].turnaround_time);
    }

    printf("\n평균 대기 시간 = %.2f", total_waiting_time / count);
    printf("\n평균 전체 실행 시간 = %.2f\n", total_turnaround_time / count);

    free(processes);
}
