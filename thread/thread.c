#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void *print_message_function(void *ptr);

int main(){

    pthread_t thread1, thread2;

    int thread1Return;
    int thread2Return;

    char *message1 = "Thread1 is runnding!\n";
    char *message2 = "Thread2 is runnding!\n";
    // Create threads that will execute functiom
    thread1Return = pthread_create(&thread1,NULL,print_message_function,(void*) message1);
    thread2Return = pthread_create(&thread2,NULL,print_message_function,(void*) message2);
    
    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);

    printf("Thread1 returns : %d\n",thread1Return);
    printf("Thread2 returns : %d\n",thread2Return);

    printf("main() thread is finished!\n");

    return 0;
}

// Function used for thread
void *print_message_function(void *ptr){
    char *message;

    message = (char *)ptr;
    printf("%s",message);
}