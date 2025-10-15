/*
Write a multithreaded program for generating prime numbers from a given starting number 
to the given ending number. 
*/

#include<pthread.h>
#include<stdio.h>
int s,e;
void* gen(void* x){
    for(int i=s;i<=e;i++){
        int p=1;
        if(i<2)
            p=0;
        for(int j=2;j*j<=i;j++)
            if(i%j==0){
                p=0;
                break;
            }
        if(p)
            printf("%d ",i);
    }
    printf("\n");
    return NULL;
}
int main(){
    printf("Start: ");
    scanf("%d",&s);
    printf("End: ");
    scanf("%d",&e);
    pthread_t t;
    pthread_create(&t,0,gen,0);
    pthread_join(t,0);
    return 0;
}

/*
output:
Start: 5
End: 20
5 7 11 13 17 19
*/