#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// define Queue size
#define QUEUESIZE 10
#define LOOP 20

int ret_count;

typedef struct{
    int buf[QUEUESIZE];
    int head, tail, full, empty;
    pthread_mutex_t *mut;
    pthread_cond_t *notFull;
    pthread_cond_t *notEmpty;
} queue;

// Queue Operation
queue *queueInit(void){
    queue *q;
    q = (queue *)malloc(sizeof(queue));
    if(q == NULL){
        perror("Queue Init is NULL Error");
    }

    q->empty = 1;
    q->full = 0;
    q->head = 0;
    q->tail = 0;
    
    q->mut = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
    ret_count = pthread_mutex_init(q->mut,NULL);
    
    q->notFull = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->notFull,NULL);
    
    q->notEmpty = (pthread_cond_t *)malloc(sizeof(pthread_cond_t));
    pthread_cond_init(q->notEmpty,NULL);
    
    return q;
}
void queueDelete(queue *q){
    
    ret_count = pthread_mutex_destroy(q->mut);
    free(q->mut);
    
    pthread_cond_destroy(q->notEmpty);
    free(q->notEmpty);
    
    pthread_cond_destroy(q->notFull);
    free(q->notFull);
    
    free(q);
}

void queueAdd(queue *q,int in){
    q->buf[q->tail] = in;
    q->tail ++;
    if(q->tail == QUEUESIZE) q->tail = 0;
    if(q->tail == q->head) q->full = 1;
    q->empty = 0;
}
void queueDel(queue *q,int *out){
    *out = q->buf[q->head];
    q->head += 1;
    if(q->head == QUEUESIZE)
        q->head = 0;
    if(q->head == q->tail) 
        q->empty=1;
    q->full = 0;
}

// Threads
void *producer(void *q){
    queue *fifo;
    int i;
    fifo = (queue *)q;
    
    for(i=0; i<LOOP; i++){
        pthread_mutex_lock(fifo->mut);
        while(fifo->full){
            printf("producer: queue Full.\n");
            pthread_cond_wait(fifo->notFull,fifo->mut);
        }
        queueAdd(fifo,i);
        pthread_mutex_unlock(fifo->mut);
        pthread_cond_signal(fifo->notEmpty);
        printf("producer: add %d\n",i);
        sleep(1);
    }    

    return NULL;
}
void *consumer(void *q){
    queue *fifo;
    int i,d;
    fifo = (queue *)q;
    
    for(i=0; i<LOOP; i++){
        pthread_mutex_lock(fifo->mut);
        while(fifo->empty){
            printf("consumer: queue Empty.\n");
            pthread_cond_wait(fifo->notEmpty,fifo->mut);
        }
        queueDel(fifo,&d);
        pthread_mutex_unlock(fifo->mut);
        pthread_cond_signal(fifo->notFull);
        printf("consumer: delete %d\n",i);
        sleep(1);
    }

    return NULL;
}

int main(){
    queue *fifo;
    pthread_t pro,con;

    // Queue initialize 
    fifo = queueInit();
    if(fifo==NULL) exit(-1);

    // Thread Creation: Pro & Con
    ret_count = pthread_create(&pro,NULL,producer,fifo);
    ret_count = pthread_create(&con,NULL,consumer,fifo);
    
    ret_count = pthread_join(pro,NULL);
    ret_count = pthread_join(con,NULL);
    
    queueDelete(fifo);
    return 0;
}

