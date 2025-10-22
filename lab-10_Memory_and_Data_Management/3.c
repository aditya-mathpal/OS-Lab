#include<stdio.h>
typedef struct{
    int base,limit;
}Seg;

int main(){
    Seg tbl[5]={{1400,1000},{6300,400},{4300,400},{3200,1100},{4700,1000}};
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

/*
output:
Segment Table:
Seg     Base    Limit
0       1400    1000
1       6300    400
2       4300    400
3       3200    1100
4       4700    1000
53 byte of seg 2: 4353
852 byte of seg 3: 4052
1222 byte of seg 0: Error
*/