#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2

int readcount = 0, writecount = 0;

sem_t x;    
sem_t y;    
sem_t z;   
sem_t rsem; 
sem_t wsem; 

void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&z);
        sem_wait(&rsem);
        sem_wait(&x);
        readcount++;
        if (readcount == 1)
            sem_wait(&wsem); 
        sem_post(&x);
        sem_post(&rsem);
        sem_post(&z);

        printf("Reader %d is reading...\n", id);

        sem_wait(&x);
        readcount--;
        if (readcount == 0)
            sem_post(&wsem); 
        sem_post(&x);
    }
    return NULL;
}

void* writer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&y);
        writecount++;
        if (writecount == 1)
            sem_wait(&rsem); 
        sem_post(&y);

        sem_wait(&wsem);
        printf("Writer %d is writing...\n", id);
        sem_post(&wsem);

        sem_wait(&y);
        writecount--;
        if (writecount == 0)
            sem_post(&rsem); 
        sem_post(&y);
        
    }
    return NULL;
}

int main() {
    pthread_t rtid[NUM_READERS], wtid[NUM_WRITERS];
    int ids[NUM_READERS > NUM_WRITERS ? NUM_READERS : NUM_WRITERS];

    sem_init(&x, 0, 1);
    sem_init(&y, 0, 1);
    sem_init(&z, 0, 1);
    sem_init(&rsem, 0, 1);
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
    sem_destroy(&y);
    sem_destroy(&z);
    sem_destroy(&rsem);
    sem_destroy(&wsem);

    return 0;
}
