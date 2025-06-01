#Reader Preference

void* reader(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&x);
        readcount++;
        if (readcount == 1)
            sem_wait(&wsem);
        sem_post(&x);

        printf("Reader %d is reading...\n", id);

        sem_wait(&x);
        readcount--;
        if (readcount == 0)
            sem_post(&wsem);
        sem_post(&x);

	}      
return NULL;
}

먼저 x로 readercount 보호하고 reader가 1이 되었을 대부터 이제 reader가 모두 끝날 때 까지 write는 참여할 수 없게 sem_wait(wsem)을 한다. 그렇게 되면 Reader가 우선순위로 먼저 끝나게 되고 Writer는 Reader가 끝난 후에 참여할 수 있게 된다. 
  
void* writer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&wsem);

        printf("Writer %d is writing...\n", id);

        sem_post(&wsem);

    }
    return NULL;
}

<img width="697" alt="R_prefer" src="https://github.com/user-attachments/assets/5f67af72-fcd2-44bf-b3c7-87b0fbe7a15f" />


#Writer Preference

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

  reader에 z를 추가하여 reader는 z에서 대기하고 writer는 rsem에서 대기하여 Reader보다 Writer가 먼저 작업에 추가되어 Writer가 우선으로 수행되게 된다. 

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

<img width="697" alt="W_prefer" src="https://github.com/user-attachments/assets/2efd4cf4-edfc-45e6-94ac-f72c63f02190" />
