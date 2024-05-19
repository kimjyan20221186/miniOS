// test2.c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "test.c"

local_mutex_t mutexWater;
pthread_cond_t condWater;
int water = 0;
int context_switches = 0;

void *purifier(void *arg) {
    for (int i = 0; i < 5; i++) {
        local_mutex_lock(&mutexWater);
        water += 70;
        printf("============================물 채움. 현재 물의 양: %d\n", water);
        local_mutex_unlock(&mutexWater);
        pthread_cond_signal(&condWater);
        sleep(1);
    }
    return NULL;
}

void *drink(void *arg) {
    local_mutex_lock(&mutexWater);
    while (water < 40) {
        printf("물이 부족하니 기다릴게.. \n");
        pthread_cond_wait(&condWater, &mutexWater.lock);
    }
    water -= 40;
    printf("고마워. 물을 마시고 나니 남은 양: %d\n", water);
    local_mutex_unlock(&mutexWater);
    return NULL;
}

void measure_performance() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);

    printf("User CPU time used: %ld.%06ld sec\n", usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    printf("System CPU time used: %ld.%06ld sec\n", usage.ru_stime.tv_sec, usage.ru_stime.tv_usec);
    printf("Maximum resident set size: %ld KB\n", usage.ru_maxrss);
    printf("Context switches: voluntary: %ld, involuntary: %ld\n", usage.ru_nvcsw, usage.ru_nivcsw);
}

void water(int argc, char *argv[]) {
    int numThreads;
    printf("생성할 스레드 수를 입력하세요: ");
    scanf("%d", &numThreads);

    pthread_t *th = (pthread_t *)malloc(numThreads * sizeof(pthread_t));

    // 로컬 뮤텍스 초기화
    if (local_mutex_init(&mutexWater) != 0) {
        fprintf(stderr, "Mutex initialization failed.\n");
        return 1;
    }
    pthread_cond_init(&condWater, NULL);

    int numDrinkers = numThreads / 3 * 2;
    int numPurifiers = numThreads - numDrinkers;

    for (int i = 0; i < numThreads; i++) {
        if (i < numDrinkers) {
            if (pthread_create(&th[i], NULL, &drink, NULL) != 0) {
                perror("failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &purifier, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }

    for (int i = 0; i < numThreads; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }

    measure_performance();

    local_mutex_destroy(&mutexWater);
    pthread_cond_destroy(&condWater);
    free(th);

}
