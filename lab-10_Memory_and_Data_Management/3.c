#include<stdio.h>
typedef struct{
    int base,limit;
}Seg;

int main(){
    Seg tbl[5]={{1400,2400},{6300,6700},{4300,4700},{3200,4300},{4700,5700}};
    int seg,off,phy;
    printf("Segment Table:\nSeg\tBase\tLimit\n");
    for(int i=0;i<5;i++)
        printf("%d\t%d\t%d\n",i,tbl[i].base,tbl[i].limit);
    int queries[3][2]={{2,53},{3,852},{0,1222}};
    for(int i=0;i<3;i++){
        seg=queries[i][0];
        off=queries[i][1];
        if(off<tbl[seg].limit){
            phy=tbl[seg].base+off;
            printf("%d byte of seg %d: %d\n",off,seg,phy);
        }else{
            printf("%d byte of seg %d: Error\n",off,seg);
        }
    }
    return 0;
}
