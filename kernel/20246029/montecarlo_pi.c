#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mutex;
int circle_points = 0, total_points = 0;

typedef struct {
    int points_to_generate;
} ThreadData;

void *generate_points(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int count = 0;
    unsigned int seed = time(NULL) ^ pthread_self();
    
    for(int i = 0; i < data->points_to_generate; ++i) {
        double x = (double)rand_r(&seed) / RAND_MAX;
        double y = (double)rand_r(&seed) / RAND_MAX;
        if(x * x + y * y <= 1) count++;
    }
    
    pthread_mutex_lock(&mutex);
    circle_points += count;
    total_points += data->points_to_generate;
    pthread_mutex_unlock(&mutex);
    
    return NULL;
}

void pi() {
    int num_threads, num_points;
    printf("스레드의 수와 스레드당 생성할 점의 수를 입력하세요: ");
    scanf("%d %d", &num_threads, &num_points);

    pthread_t threads[num_threads];
    ThreadData data[num_threads];
    pthread_mutex_init(&mutex, NULL);
    
    for(int i = 0; i < num_threads; i++) {
        data[i].points_to_generate = num_points;
        pthread_create(&threads[i], NULL, generate_points, (void *)&data[i]);
    }
    
    for(int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("추정된 파이 값 = %f\n", 4.0 * circle_points / total_points);
    pthread_mutex_destroy(&mutex);
}

int main() {
    pi();
    return 0;
}
