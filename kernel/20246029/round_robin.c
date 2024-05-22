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

struct Queue {
    int front, rear, size; 
    unsigned capacity; 
    int* array;
};

// 큐를 생성하는 함수
struct Queue* createQueue(unsigned capacity) {
    struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue)); // 큐 메모리 할당
    queue->capacity = capacity;
    queue->front = queue->size = 0; // 큐의 프론트와 사이즈 초기화
    queue->rear = capacity - 1; // 큐의 리어 초기화
    queue->array = (int*) malloc(queue->capacity * sizeof(int)); // 큐 배열 메모리 할당
    return queue;
}

// 큐가 가득 찼는지 확인하는 함수
int isFull(struct Queue* queue) {
    return (queue->size == queue->capacity);
}

// 큐가 비었는지 확인하는 함수
int isEmpty(struct Queue* queue) {
    return (queue->size == 0);
}

// 큐에 요소 추가하는 함수
void enqueue(struct Queue* queue, int item) {
    if (isFull(queue))
        return; // 큐가 가득 차면 리턴
    queue->rear = (queue->rear + 1) % queue->capacity; 
    queue->array[queue->rear] = item; // 큐 배열에 요소 추가
    queue->size += 1; // 큐 사이즈 증가
}

// 큐에서 요소를 제거하는 함수
int dequeue(struct Queue* queue) {
    if (isEmpty(queue))
        return -1; // 큐가 비어 있으면 -1 리턴
    int item = queue->array[queue->front]; // 프론트 요소 가져오기
    queue->front = (queue->front + 1) % queue->capacity; // 프론트 포인터 갱신
    queue->size -= 1; // 큐 사이즈 감소
    return item; // 제거한 요소 리턴
}

// 프로세스들의 실행 시간을 계산하는 함수
void calculateTime(struct Process *processes, int count, int quantum) {
    struct Queue* queue = createQueue(count); // 큐 생성
    int time = 0;
    int remainingProcesses = count;

    // 현재 시간에 도착한 프로세스를 큐에 넣음
    for (int i = 0; i < count; i++) {
        if (processes[i].arrivalTime <= time) {
            enqueue(queue, i);
        }
    }

    while (remainingProcesses > 0) {
        int currentIndex = dequeue(queue); // 큐에서 프로세스 인덱스 꺼내기

        if (currentIndex != -1) {
            struct Process *currentProcess = &processes[currentIndex]; // 현재 프로세스 포인터

            printf("현재 시간 %d: 프로세스 p%d 실행 중\n", time, currentProcess->pid);

            if (currentProcess->remainingTime > quantum) {
                time += quantum; 
                currentProcess->remainingTime -= quantum; 
                enqueue(queue, currentIndex); // 타임 퀀텀이 끝나면 프로세스를 큐에 다시 추가
            } else {
                time += currentProcess->remainingTime; 
                currentProcess->remainingTime = 0;
                currentProcess->waitingTime = time - currentProcess->burstTime - currentProcess->arrivalTime; 
                currentProcess->turnaroundTime = time - currentProcess->arrivalTime; 
                remainingProcesses--; 
                printf("프로세스 p%d 종료\n", currentProcess->pid);
            }

            // 현재 시간에 도착한 새로운 프로세스를 큐에 추가
            for (int i = 0; i < count; i++) {
                if (processes[i].arrivalTime > (time - quantum) && processes[i].arrivalTime <= time && processes[i].remainingTime > 0 && i != currentIndex) {
                    enqueue(queue, i);
                    printf("프로세스 p%d 추가됨 (도착 시간: %d)\n", processes[i].pid, processes[i].arrivalTime);
                }
            }
        } else {
            time++;
        }
    }

    free(queue->array);
    free(queue);
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
