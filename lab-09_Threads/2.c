/*
Write a multithreaded program that calculates the summation of non-negative integers in a 
separate thread and passes the result to the main thread.
*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
int n,*a,s=0;
void* sum(void* x){
    for(int i=0;i<n;i++)
        s+=a[i];
    return NULL;
}
int main(){
    printf("Enter n: ");
    scanf("%d",&n);
    a=malloc(n*sizeof(int));
    printf("Enter %d numbers: ",n);
    for(int i=0;i<n;i++)
        scanf("%d",&a[i]);
    pthread_t t;
    pthread_create(&t,0,sum,0);
    pthread_join(t,0);
    printf("Sum: %d\n",s);
    free(a);
    return 0;
}

/*
output:
Enter n: 5
Enter 5 numbers: 2 4 1 5 9
Sum: 21
*/