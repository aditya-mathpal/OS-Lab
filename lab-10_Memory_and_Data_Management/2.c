#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
typedef struct{
    int id,bt,pri;
}Proc;
Proc*q[3];
int cnt[3]={0};
pthread_mutex_t lock=PTHREAD_MUTEX_INITIALIZER;

void*worker(void*arg){
    int lvl=*(int*)arg;
    pthread_mutex_lock(&lock);
    for(int i=0;i<cnt[lvl];i++){
        if(q[lvl][i].bt>0){
            printf("Q%d P%d BT=%d\n",lvl,q[lvl][i].id,q[lvl][i].bt);
            q[lvl][i].bt-=2;
            if(q[lvl][i].bt>0&&lvl<2){
                q[lvl+1][cnt[lvl+1]++]=q[lvl][i];
            }
        }
    }
    pthread_mutex_unlock(&lock);
    return NULL;
}

int main(){
    int n,i,id,bt;
    printf("Processes:");
    scanf("%d",&n);
    for(i=0;i<3;i++)q[i]=malloc(50*sizeof(Proc));
    printf("Enter id bt:\n");
    for(i=0;i<n;i++){
        scanf("%d%d",&id,&bt);
        q[0][cnt[0]].id=id;
        q[0][cnt[0]].bt=bt;
        q[0][cnt[0]++].pri=0;
    }
    pthread_t t[3];
    int a[3]={0,1,2};
    for(i=0;i<3;i++)pthread_create(&t[i],NULL,worker,&a[i]);
    for(i=0;i<3;i++)pthread_join(t[i],NULL);
    for(i=0;i<3;i++)free(q[i]);
    return 0;
}
