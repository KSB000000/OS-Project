#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

int readcount = 0;
sem_t x;       // 보호용 세마포어
sem_t wsem;    // writer 접근 제어용

void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&x);
        readcount++;
        if (readcount == 1)
            sem_wait(&wsem);  // 첫 번째 reader가 writer를 막음
        sem_post(&x);

        // 읽기 작업 수행
        printf("📖 Reader %d is reading...\n", id);
        usleep(100000);  // 0.1초

        sem_wait(&x);
        readcount--;
        if (readcount == 0)
            sem_post(&wsem);  // 마지막 reader가 writer 허용
        sem_post(&x);

        usleep(rand() % 1000000);  // 쉬는 시간
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&wsem);  // writer는 reader와 독점적

        // 쓰기 작업 수행
        printf("✍️  Writer %d is writing...\n", id);
        usleep(200000);  // 0.2초

        sem_post(&wsem);

        usleep(rand() % 1000000);
    }
    return NULL;
}

int main() {
    pthread_t rtid[NUM_READERS], wtid[NUM_WRITERS];
    int ids[NUM_READERS > NUM_WRITERS ? NUM_READERS : NUM_WRITERS];

    sem_init(&x, 0, 1);
    sem_init(&wsem, 0, 1);

    for (int i = 0; i < NUM_READERS; i++) {
        ids[i] = i + 1;
        pthread_create(&rtid[i], NULL, reader, &ids[i]);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        ids[i] = i + 1;
        pthread_create(&wtid[i], NULL, writer, &ids[i]);
    }

    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(rtid[i], NULL);
    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(wtid[i], NULL);

    sem_destroy(&x);
    sem_destroy(&wsem);
    return 0;
}
