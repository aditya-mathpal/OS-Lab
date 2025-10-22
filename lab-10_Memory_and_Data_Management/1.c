#include<stdio.h>
#include<stdlib.h>
typedef struct MAB{
    int off,sz,alloc;
    struct MAB*next,*prev;
}MAB;
MAB*head=NULL;

MAB*memChk(MAB*m,int sz){
    MAB*t=m;
    while(t){
        if(!t->alloc&&t->sz>=sz)return t;
        t=t->next;
    }
    return NULL;
}

MAB*memSplit(MAB*b,int sz){
    MAB*n=malloc(sizeof(MAB));
    n->off=b->off+sz;
    n->sz=b->sz-sz;
    n->alloc=0;
    n->next=b->next;
    n->prev=b;
    b->next=n;
    if(n->next)n->next->prev=n;
    b->sz=sz;
    return b;
}

MAB*memAlloc(MAB*m,int sz){
    MAB*b=memChk(m,sz);
    if(!b)return NULL;
    if(b->sz>sz)memSplit(b,sz);
    b->alloc=1;
    return b;
}

void memMerge(MAB*m){
    if(m->next&&!m->next->alloc){
        m->sz+=m->next->sz;
        MAB*t=m->next;
        m->next=t->next;
        if(t->next)t->next->prev=m;
        free(t);
    }
    if(m->prev&&!m->prev->alloc){
        m->prev->sz+=m->sz;
        m->prev->next=m->next;
        if(m->next)m->next->prev=m->prev;
        free(m);
    }
}

void memFree(MAB*m){
    if(!m)return;
    m->alloc=0;
    memMerge(m);
}

void display(){
    MAB*t=head;
    printf("Off\tSz\tAlloc\n");
    while(t){
        printf("%d\t%d\t%d\n",t->off,t->sz,t->alloc);
        t=t->next;
    }
}

int main(){
    int sz,n,i,req;
    printf("Memory size:");
    scanf("%d",&sz);
    head=malloc(sizeof(MAB));
    head->off=0;
    head->sz=sz;
    head->alloc=0;
    head->next=head->prev=NULL;
    printf("Requests:");
    scanf("%d",&n);
    MAB**blk=malloc(n*sizeof(MAB*));
    for(i=0;i<n;i++){
        printf("Size %d:",i+1);
        scanf("%d",&req);
        blk[i]=memAlloc(head,req);
        if(blk[i])printf("Allocated at %d\n",blk[i]->off);
        else printf("Fail\n");
    }
    display();
    printf("Free block:");
    scanf("%d",&i);
    if(i>0&&i<=n&&blk[i-1])memFree(blk[i-1]);
    display();
    free(blk);
    return 0;
}
