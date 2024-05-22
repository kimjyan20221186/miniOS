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

struct Queue* createQueue(unsigned capacity) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(queue->capacity * sizeof(int));
    return queue;
}

int isFull(struct Queue* queue) {
    return (queue->size == queue->capacity);
}

int isEmpty(struct Queue* queue) {
    return (queue->size == 0);
}

void enqueue(struct Queue* queue, int item) {
    if (isFull(queue)) {
        printf("큐가 가득 찼습니다. 더 이상 프로세스를 추가할 수 없습니다.\n");
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size += 1;
}

int dequeue(struct Queue* queue) {
    if (isEmpty(queue)) {
        printf("큐가 비어 있습니다. 대기 중인 프로세스가 없습니다.\n");
        return -1;
    }
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size -= 1;
    return item;
}

void calculateTime(struct Process* processes, int count, int quantum) {
    struct Queue* queue = createQueue(count);
    int time = 0;
    int remainingProcesses = count;

    // 도착 시간 순서대로 초기 큐 설정
    for (int i = 0; i < count; i++) {
        enqueue(queue, i);
    }

    while (remainingProcesses > 0) {
        if (!isEmpty(queue)) {
            int currentIndex = dequeue(queue);
            struct Process* currentProcess = &processes[currentIndex];

            printf("현재 시간 %d: 프로세스 p%d 실행 중\n", time, currentProcess->pid);

            int execTime = (currentProcess->remainingTime > quantum) ? quantum : currentProcess->remainingTime;
            time += execTime;
            currentProcess->remainingTime -= execTime;

            if (currentProcess->remainingTime == 0) {
                currentProcess->waitingTime = time - currentProcess->burstTime - currentProcess->arrivalTime;
                currentProcess->turnaroundTime = time - currentProcess->arrivalTime;
                remainingProcesses--;
                printf("프로세스 p%d 종료\n", currentProcess->pid);
            } else {
                enqueue(queue, currentIndex);
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
    struct Process* processes = (struct Process*)malloc(sizeof(struct Process) * count);

    for (int i = 0; i < count; i++) {
        printf("프로세스 %d의 도착 시간과 실행 시간을 입력하세요: ", i + 1);
        scanf("%d %d", &processes[i].arrivalTime, &processes[i].burstTime);
        processes[i].pid = i + 1;
        processes[i].waitingTime = 0;
        processes[i].turnaroundTime = 0;
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
    printf("평균 반환 시간: %.2f\n", (float)totalTurnaroundTime / count);

    free(processes);
}
