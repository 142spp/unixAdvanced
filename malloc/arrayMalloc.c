#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student{
    int id;
    char name[10];
};

int main(){
    int *studentID;

    studentID = (int *)malloc(sizeof(int)*3);

    if(studentID == NULL){
        fprintf(stderr,"Out of memory\n");
        exit(-1);
    }
    studentID[0] = 123;
    studentID[1] = 456;
    studentID[2] = 789;

    printf("%d\n%d\n%d\n",studentID[0],studentID[1],studentID[2]);

    free(studentID);

    struct student *pStudent;
    pStudent = (struct student *)malloc(sizeof(struct student)*3);
    

    return 0;
}