# All Resource

sem_t once;
철학자들이 한 번에 하나씩만 포크를 들 수 있도록 제한하는 추가 세마포어를 세팅한다.


```c
void *philosopher(void *arg){
 int philosopher_num;
 philosopher_num = (unsigned long int) arg;
 while(1){
 sem_wait(&once);
 pickup(philosopher_num);
 printf("philosopher %d picks up the fork %d.\n", philosopher_num, philosopher_num);
 pickup(philosopher_num + 1);
 printf("philosopher %d picks up the fork %d.\n", philosopher_num, (philosopher_num + 1) % NUM);
 sem_post(&once);
 eating(philosopher_num);
 putdown(philosopher_num + 1);
 printf("philosopher %d puts down the fork %d.\n", philosopher_num, (philosopher_num + 1) % NUM);
 putdown(philosopher_num);
 printf("philosopher %d puts down the fork %d.\n", philosopher_num, philosopher_num);
 thinking(philosopher_num);
 }
 return NULL;
}
```
한 번에 하나의 철학자만 포크를 들 수 있도록 once 세마포어를 wait한다.
그 후 once 세마포어 해제 → 다음 철학자에게 기회를 줌


# Ordering


모두 왼쪽 포크를 먼저 집음 -> 모두가 오른쪽 포크를 기다림 -> 교착상태

```c
void *philosopher(void *arg){
 int philosopher_num;
 philosopher_num = (unsigned long int) arg;
 while(1){
 if(philosopher_num < 4){
 pickup(philosopher_num);
 printf("philosopher %d picks up the fork %d.\n", philosopher_num, philosopher_num);
 pickup(philosopher_num + 1);
 printf("philosopher %d picks up the fork %d.\n", philosopher_num, (philosopher_num + 1) % NUM);
 } else{
pickup(philosopher_num + 1);
printf("philosopher %d picks up the fork %d.\n", philosopher_num, (philosopher_num + 1) % NUM);
pickup(philosopher_num);
printf("philosopher %d picks up the fork %d.\n", philosopher_num, philosopher_num);
 }
 eating(philosopher_num);
 putdown(philosopher_num + 1);
 printf("philosopher %d puts down the fork %d.\n", philosopher_num, (philosopher_num + 1) % NUM);
 putdown(philosopher_num);
 printf("philosopher %d puts down the fork %d.\n", philosopher_num, philosopher_num);
 thinking(philosopher_num);
 }
 return NULL;
}
```

philosopher_num이 4 미만은 사람들은 왼쪽 포크 먼저 들게 하고 4이상인 philosopher는 오른쪽 먼저 들게 하여 교착 상태 방지.

단 1명의 철학자만 포크 잡는 순서를 반대로 설정함으로써 모든 철학자가 서로를 블로킹하는 상황 방지.



