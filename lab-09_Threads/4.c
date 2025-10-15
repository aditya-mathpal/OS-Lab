/*
Write a multithreaded program that performs the sum of even numbers and odd numbers 
in  an  input  array.  Create  a  separate  thread  to  perform  the  sum  of  even  numbers  and  odd 
numbers. The parent thread has to wait until both the threads are done.
*/

#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
int n,*a,es=0,os=0;
void* evn(void* x){
    for(int i=0;i<n;i++)
        if(a[i]%2==0)
            es+=a[i];
    return NULL;
}
void* odd(void* x){
    for(int i=0;i<n;i++)
        if(a[i]%2)
            os+=a[i];
    return NULL;
}
int main(){
    printf("Enter n: ");
    scanf("%d",&n);
    a=malloc(n*sizeof(int));
    printf("Enter %d numbers: ",n);
    for(int i=0;i<n;i++)
        scanf("%d",&a[i]);
    pthread_t t1,t2;
    pthread_create(&t1,0,evn,0);
    pthread_create(&t2,0,odd,0);
    pthread_join(t1,0);
    pthread_join(t2,0);
    printf("Even sum: %d\nOdd sum: %d\n",es,os);
    free(a);
    return 0;
}

/*
output:
Enter n: 5
Enter 5 numbers: 1 2 3 4 5
Even sum: 6
Odd sum: 9
*/