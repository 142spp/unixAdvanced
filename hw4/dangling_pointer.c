#include <stdio.h>
#include <stdlib.h>

void task(){
    int *ptr = malloc(sizeof(int));
    *ptr = 10;
    printf("%p\t%d \n",ptr,*ptr);
    free(ptr);
    printf("%p\t%d \n",ptr,*ptr);
    ptr=NULL;
    printf("%p\t%d",ptr,*ptr);
}

int main(){
    task();
    return 0;
}
