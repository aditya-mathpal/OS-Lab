/*
A file system uses contiguous allocation of disk space to files. A few blocks on the disk 
are  reserved  as  spare  blocks.  If  some  disk  blocks  is  found  to  be  bad,  the  file  system 
allocates a spare disk block to it and notes the address of the bad block and its allocated 
spare block in a “bad blocks table”. This table is consulted while accessing the disk 
block. Simulate the same.#include <stdio.h>
*/

#include<stdio.h>
#include<stdlib.h>

struct BadBlock {
    int bad;
    int spare;
};

struct File {
    char name[20];
    int start;
    int len;
};

int main() {
    int n_blocks, n_spare, n_bad=0, n_files=0, i;
    struct BadBlock tbl[100];
    struct File files[50];
    
    printf("Total blocks: ");
    scanf("%d", &n_blocks);
    printf("Spare blocks: ");
    scanf("%d", &n_spare);
    
    int ch;
    while(1) {
        printf("\n1.Add file 2.Mark bad 3.Access 4.Display 5.Exit\nChoice: ");
        scanf("%d", &ch);
        
        if(ch == 1) {
            printf("Name: ");
            scanf("%s", files[n_files].name);
            printf("Start: ");
            scanf("%d", &files[n_files].start);
            printf("Length: ");
            scanf("%d", &files[n_files].len);
            n_files++;
        }
        else if(ch == 2) {
            if(n_bad >= n_spare) {
                printf("No spare blocks!\n");
                continue;
            }
            printf("Bad block: ");
            scanf("%d", &tbl[n_bad].bad);
            tbl[n_bad].spare = n_blocks + n_bad;
            printf("Mapped to spare %d\n", tbl[n_bad].spare);
            n_bad++;
        }
        else if(ch == 3) {
            int blk, actual;
            printf("Block: ");
            scanf("%d", &blk);
            actual = blk;
            for(i=0; i<n_bad; i++) {
                if(tbl[i].bad == blk) {
                    actual = tbl[i].spare;
                    printf("Bad! Redirected to %d\n", actual);
                    break;
                }
            }
            if(actual == blk)
                printf("Accessing %d\n", blk);
        }
        else if(ch == 4) {
            printf("\nFiles:\n");
            for(i=0; i<n_files; i++)
                printf("%s: %d-%d\n", files[i].name, files[i].start, files[i].start+files[i].len-1);
            printf("\nBad Blocks:\n");
            for(i=0; i<n_bad; i++)
                printf("%d -> %d\n", tbl[i].bad, tbl[i].spare);
        }
        else if(ch == 5) break;
    }
    return 0;
}

/*
output:
Total blocks: 5
Spare blocks: 1

1.Add file 2.Mark bad 3.Access 4.Display 5.Exit
Choice: 1
Name: f1
Start: 2
Length: 2

1.Add file 2.Mark bad 3.Access 4.Display 5.Exit
Choice: 2
Bad block: 3
Mapped to spare 5

1.Add file 2.Mark bad 3.Access 4.Display 5.Exit
Choice: 3
Block: 3
Bad! Redirected to 5

1.Add file 2.Mark bad 3.Access 4.Display 5.Exit
Choice: 4

Files:
f1: 2-3

Bad Blocks:
3 -> 5

1.Add file 2.Mark bad 3.Access 4.Display 5.Exit
Choice: 5
*/