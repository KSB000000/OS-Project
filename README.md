# Producer Consumer Problem

MAX = 30 이면 lock을 걸고

생상자는 cond_wait을 해서 x가 30이 되면 +를 진행할 수 없게 spin lock을 걸어 놓는다.

그리고 소비자는 0이 되면 x가 -가 되지 않게 spin lock을 걸어 놓는다.

그렇게 0~30 사이를 반복하며 출력하게 된다.
```c
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
```
x에 접근 하기 전에 pthread_mutex_lock(&mutex)를 하여 상호배제 활성화 그리고 끝나면 unlock
```c
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
```

소비자의 경우도 위와 동일


![image](https://github.com/user-attachments/assets/cee95d25-cf65-4ac8-9dcb-b7f87ccb04ed)

<img width="697" alt="PD" src="https://github.com/user-attachments/assets/0e1811e7-5027-4caf-8584-f5e0e6ddd1ab" />
