#include <stdio.h>
#include <stdlib.h>

void task(){
    int* ptr;
    printf("%p\t%d",ptr,*ptr);
    return;
}

int main(){
    task();
    return 0;
}