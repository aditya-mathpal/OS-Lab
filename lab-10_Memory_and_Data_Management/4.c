/*
Write a C program to simulate LRU approximation page replacement using second chance
algorithm. Find the total number of page faults and hit ratio for the algorithm.
*/

#include<stdio.h>

int main(){
    int frames,n,i,j,faults=0,hits=0,ptr=0;
    printf("Frames: ");
    scanf("%d",&frames);
    int pg[frames],ref[frames];
    for(i=0;i<frames;i++){
        pg[i]=-1;
        ref[i]=0;
    }
    printf("Pages: ");
    scanf("%d",&n);
    int seq[n];
    printf("Enter sequence:\n");
    for(i=0;i<n;i++)scanf("%d",&seq[i]);
    
    for(i=0;i<n;i++){
        int found=0;
        for(j=0;j<frames;j++){
            if(pg[j]==seq[i]){
                ref[j]=1;
                hits++;
                found=1;
                break;
            }
        }
        if(!found){
            while(1){
                if(ref[ptr]==0){
                    pg[ptr]=seq[i];
                    ref[ptr]=1;
                    ptr=(ptr+1)%frames;
                    break;
                }
                ref[ptr]=0;
                ptr=(ptr+1)%frames;
            }
            faults++;
        }
        printf("Page %d: ",seq[i]);
        for(j=0;j<frames;j++)
            printf("%d ",pg[j]);
        printf("\n");
    }
    printf("Faults: %d\n",faults);
    printf("Hits: %d\n",hits);
    printf("Hit Ratio: %.2f\n",(float)hits/n);
    return 0;
}

/*
output:
Frames: 4
Pages: 5
Enter sequence:
1 2 3 4 1
Page 1: 1 -1 -1 -1 
Page 2: 1 2 -1 -1 
Page 3: 1 2 3 -1 
Page 4: 1 2 3 4 
Page 1: 1 2 3 4 
Faults: 4
Hits: 1
Hit Ratio: 0.20
*/