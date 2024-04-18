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
    unsigned int seed = time(NULL) ^ (unsigned int)pthread_self();
    for (i = 0; i < num_points; i++) {
        x = (double)rand_r(&seed) / RAND_MAX * 2 - 1;
        y = (double)rand_r(&seed) / RAND_MAX * 2 - 1;
        if (x * x + y * y <= 1) {
            cc_points_local++;
        }
    }
    pthread_mutex_lock(&mutex);
    circle_points += cc_points_local;
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void pi(int argc, char* argv[]) {
    pthread_t threads[NUM_THREADS];
    long long points_per_thread[NUM_THREADS];
    long long points = NUM_POINTS / NUM_THREADS;
    long long remainder = NUM_POINTS % NUM_THREADS;
    // 스레드 생성
    for (int i = 0; i < NUM_THREADS; i++) {
        points_per_thread[i] = points + (i < remainder ? 1 : 0);
        pthread_create(&threads[i], NULL, gen_points, (void *)&points_per_thread[i]);
    }
    // 스레드 종료 대기
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
    // π 추정
    double pi_estimate = 4.0 * circle_points / NUM_POINTS;
    printf("Estimated π = %f\n", pi_estimate);
}
