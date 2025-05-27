#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>


sem_t agentSemaphore;       // 表示供应商是否可以提供材料
sem_t tobaccoAndPaper;      // 表示有烟草和纸可用
sem_t tobaccoAndGlue;       // 表示有烟草和胶水可用
sem_t paperAndGlue;         // 表示有纸和胶水可用
sem_t smokerDone;           // 表示抽烟者已完成抽烟


void initRandom() {
    srand(time(NULL));
}


void* agent(void* arg) {
    while (1) {
  
        sem_wait(&smokerDone);
        
  
        int choice = rand() % 3;
        
        switch (choice) {
            case 0:
                printf("供应商提供了烟草和纸\n");
                sem_post(&tobaccoAndPaper);
                break;
            case 1:
                printf("供应商提供了烟草和胶水\n");
                sem_post(&tobaccoAndGlue);
                break;
            case 2:
                printf("供应商提供了纸和胶水\n");
                sem_post(&paperAndGlue);
                break;
        }
        

        sem_wait(&agentSemaphore);
    }
    return NULL;
}

void* smokerWithTobacco(void* arg) {
    while (1) {
        
        sem_wait(&paperAndGlue);
        
        printf("有烟草的抽烟者正在卷烟并抽烟\n");
        sleep(1); 
        
        
        sem_post(&agentSemaphore);
        sem_post(&smokerDone);
    }
    return NULL;
}


void* smokerWithPaper(void* arg) {
    while (1) {
        
        sem_wait(&tobaccoAndGlue);
        
        printf("有纸的抽烟者正在卷烟并抽烟\n");
        sleep(1); 
        
        
        sem_post(&agentSemaphore);
        sem_post(&smokerDone);
    }
    return NULL;
}


void* smokerWithGlue(void* arg) {
    while (1) {
       
        sem_wait(&tobaccoAndPaper);
        
        printf("有胶水的抽烟者正在卷烟并抽烟\n");
        sleep(1); 
        
        
        sem_post(&agentSemaphore);
        sem_post(&smokerDone);
    }
    return NULL;
}

int main() {
    
    sem_init(&agentSemaphore, 0, 0);
    sem_init(&tobaccoAndPaper, 0, 0);
    sem_init(&tobaccoAndGlue, 0, 0);
    sem_init(&paperAndGlue, 0, 0);
    sem_init(&smokerDone, 0, 1); 
    
    
    initRandom();
    
    
    pthread_t agentThread, smoker1, smoker2, smoker3;
    
    pthread_create(&agentThread, NULL, agent, NULL);
    pthread_create(&smoker1, NULL, smokerWithTobacco, NULL);
    pthread_create(&smoker2, NULL, smokerWithPaper, NULL);
    pthread_create(&smoker3, NULL, smokerWithGlue, NULL);
    
   
    pthread_join(agentThread, NULL);
    pthread_join(smoker1, NULL);
    pthread_join(smoker2, NULL);
    pthread_join(smoker3, NULL);
    
   
    sem_destroy(&agentSemaphore);
    sem_destroy(&tobaccoAndPaper);
    sem_destroy(&tobaccoAndGlue);
    sem_destroy(&paperAndGlue);
    sem_destroy(&smokerDone);
    
    return 0;
}        
