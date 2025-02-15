#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <semaphore.h>


typedef struct {
    int N;
    unsigned int max_threads;
    float r;
    int m;
    pthread_mutex_t* mutex;

}Data;


void *calculate(void* arg){
    printf("Thread ID: %lu\n", pthread_self());
    Data* data = (Data*)arg;
    int c = 0;
    int count = data->N / data->max_threads;
    for (int i = 0; i < count; i++) {
        float x = ((float)rand()/(float)(RAND_MAX) * data->r);
        float y = ((float)rand()/(float)(RAND_MAX) * data->r);
        if (rand() % 2 == 1) x *= -1;
        if (rand() % 2 == 1) y *= -1;
        if (x * x + y * y <= data->r * data->r) {
            c++;
        }
    }
    pthread_mutex_lock(data->mutex);
    data->m += c;
    pthread_mutex_unlock(data->mutex);
    pthread_exit(0);
}



int main(int argc, char *argv[]){
    if (argc < 2){
        fprintf(stderr,"Usage %s <max threads> <radius>", argv[0]);
        return 1;
    }

    float r = atof(argv[2]);
    unsigned int max_threads = atoi(argv[1]);


    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    int m = 0;
    int N = (int) 1000 * r * r;
    srand(time(NULL));

    pthread_t* threads = malloc(max_threads * sizeof(pthread_t));

    if (!threads){
        fprintf(stderr, "Failed to allocated memory");
        return 1;
    }

    Data data = {N, max_threads, r, m, &mutex};

    clock_t start = clock();

    for (int i = 0; i <max_threads; ++i){
        if(pthread_create(&threads[i], NULL, calculate, &data) != 0){
            fprintf(stderr, "Failed create thread");
            return 1;
        }
    }

    for (int i = 0; i < max_threads; ++i){
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < N % max_threads; i++) {
        float x = ((float)rand()/(float)(RAND_MAX) * r) * (-1 * (rand() % 2));
        float y = ((float)rand()/(float)(RAND_MAX) * r) * (-1 * (rand() % 2));

        if (x * x + y * y <= r * r) {
            data.m++;
        }
    }
    clock_t end = clock();
    double del_time = (double) end-start;
    printf("Square: %f\n", ((float)data.m / (float)N) * 4 * r * r);
    printf("Del time: %f\n", del_time);

    free(threads);
    pthread_mutex_destroy(&mutex);


    return 0;
}