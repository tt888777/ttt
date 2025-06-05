#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define BUFFER_SIZE 5

int buffer[BUFFER_SIZE];       //定义缓冲区
int in = 0, out = 0;
int i=0,j=0;

sem_t empty, full;           //定义两个信号量
pthread_mutex_t mutex;
//生产者进程
void* producer(void* arg) {
    int item = 0;
    while (i<BUFFER_SIZE) {
        item++; 
        sem_wait(&empty);        // P（empty）
        pthread_mutex_lock(&mutex);    //上锁
        buffer[in] = item;
        printf("Producer: %d at %d\n", item, in);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);  //解锁
        sem_post(&full);         // V（full)
        i++;  
    }
    return NULL;
}
//消费者进程
void* consumer(void* arg) {
    while (j<BUFFER_SIZE) {
        sem_wait(&full);           // P（full)
        pthread_mutex_lock(&mutex);     //上锁
        int item = buffer[out];
        printf("Consumer: %d from %d\n", item, out);
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex);   //解锁
        sem_post(&empty);          // V（empty）
        j++;
    }
    return NULL;
}
//主函数
int main() {
    pthread_t tid_prod, tid_cons;

    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&tid_prod, NULL, producer, NULL);
    pthread_create(&tid_cons, NULL, consumer, NULL);

    pthread_join(tid_prod, NULL);
    pthread_join(tid_cons, NULL);

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
