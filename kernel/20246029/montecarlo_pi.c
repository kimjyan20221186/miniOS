#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NUM_THREADS 4
#define NUM_POINTS 20000000

long long circle_points = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* gen_points(void* args) {
    long long num_points = *((long long*)args);
    double x, y;
    long long i, cc_points_local = 0;
    unsigned int seed = time(NULL) ^ pthread_self();

    for (i = 0; i < num_points; i++) {
        x = (double)rand_r(&seed) / RAND_MAX * 2 - 1;
        y = (double)rand_r(&seed) / RAND_MAX * 2 - 1;
        if (x * x + y * y <= 1) {
            cc_points_local++;
        }
    }
    // 전역 변수 접근 시 뮤텍스로 보호
    pthread_mutex_lock(&mutex);
    circle_points += cc_points_local;
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

void pi(int argc, char* argv[]) {
    pthread_t threads[NUM_THREADS];
    long long points_per_thread[NUM_THREADS];
    int i;
    long long points = NUM_POINTS / NUM_THREADS;
    long long remainder = NUM_POINTS % NUM_THREADS;

    for (i = 0; i < NUM_THREADS; i++) {
        points_per_thread[i] = points;
        if (remainder > 0) {
            points_per_thread[i]++;
            remainder--;
        }
    // 스레드 종료 대기
    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    // π 추정
    double pi_estimate = 4.0 * circle_points / NUM_POINTS;
    printf("Estimated π = %f\n", pi_estimate);
}
}
