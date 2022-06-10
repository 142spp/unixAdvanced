#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


void task(){
    int *ptr = (int *) malloc(sizeof(int));
    printf("%p\n",ptr);
    //free(ptr);
    return;
}

int main(){
    int i = 10;
    while(i--){ task();}
    return 0;
}
