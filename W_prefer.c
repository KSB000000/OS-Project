#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

int readcount = 0, writecount = 0;

sem_t x;     // readcount 보호
sem_t y;     // writecount 보호
sem_t z;     // rsem 보호
sem_t rsem;  // reader 접근 제어
sem_t wsem;  // writer 접근 제어

void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&z);
        sem_wait(&rsem);
        sem_wait(&x);
        readcount++;
        if (readcount == 1)
            sem_wait(&wsem); // 첫 번째 reader가 writer 막기
        sem_post(&x);
        sem_post(&rsem);
        sem_post(&z);

        printf("📖 Reader %d is reading...\n", id);
        usleep(100000);  // 읽기 작업

        sem_wait(&x);
        readcount--;
        if (readcount == 0)
            sem_post(&wsem); // 마지막 reader가 writer 허용
        sem_post(&x);

        usleep(rand() % 1000000);
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&y);
        writecount++;
        if (writecount == 1)
            sem_wait(&rsem); // 첫 번째 writer가 reader 막기
        sem_post(&y);

        sem_wait(&wsem);
        printf("✍️  Writer %d is writing...\n", id);
        usleep(200000);  // 쓰기 작업
        sem_post(&wsem);

        sem_wait(&y);
        writecount--;
        if (writecount == 0)
            sem_post(&rsem); // 마지막 writer가 reader 허용
        sem_post(&y);

        usleep(rand() % 1000000);
    }
    return NULL;
}

int main() {
    pthread_t rtid[NUM_READERS], wtid[NUM_WRITERS];
    int ids[NUM_READERS > NUM_WRITERS ? NUM_READERS : NUM_WRITERS];

    // 세마포어 초기화
    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);
    sem_init(&z, 0, 1);
    sem_init(&rsem, 0, 1);
    sem_init(&wsem, 0, 1);

    // reader 스레드 생성
    for (int i = 0; i < NUM_READERS; i++) {
        ids[i] = i + 1;
        pthread_create(&rtid[i], NULL, reader, &ids[i]);
    }

    // writer 스레드 생성
    for (int i = 0; i < NUM_WRITERS; i++) {
        ids[i] = i + 1;
        pthread_create(&wtid[i], NULL, writer, &ids[i]);
    }

    // 스레드 종료 대기
    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(rtid[i], NULL);
    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(wtid[i], NULL);

    // 세마포어 제거
    sem_destroy(&x);
    sem_destroy(&y);
    sem_destroy(&z);
    sem_destroy(&rsem);
    sem_destroy(&wsem);

    return 0;
}
