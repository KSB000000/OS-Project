#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define ITER 1000
#define MAX 30

void *thread_increment(void *arg);
void *thread_decrement(void *arg);

int x = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_empty = PTHREAD_COND_INITIALIZER;

int main()
{
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, thread_increment, NULL);
    pthread_create(&tid2, NULL, thread_decrement, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    if (x != 0)
        printf("BOOM! counter=%d\n", x);
    else
        printf("OK counter=%d\n", x);

    return 0;
}

void *thread_increment(void *arg)
{
    for (int i = 0; i < ITER; i++)
    {
        pthread_mutex_lock(&mutex);

        while (x == MAX)
            pthread_cond_wait(&cond_full, &mutex);

        x++;
        printf("%lu: %d\n", (uintptr_t)pthread_self(), x);


        pthread_cond_signal(&cond_empty);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void *thread_decrement(void *arg)
{
    for (int i = 0; i < ITER; i++)
    {
        pthread_mutex_lock(&mutex);


        while (x == 0)
            pthread_cond_wait(&cond_empty, &mutex);

        x--;
        printf("%lu: %d\n", (uintptr_t)pthread_self(), x);

        pthread_cond_signal(&cond_full);
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}
