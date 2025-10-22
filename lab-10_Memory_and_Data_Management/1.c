/*
If you wish to implement Best Fit, First Fit, Next Fit, or Worst Fit memory allocation 
policy, it is probably best to do this by describing the memory as a structure in a linked list: 
struct mab { 
    int offset; 
    int size; 
    int allocated; 
    struct mab * next; 
    struct mab * prev; 
}; 
typedef struct mab Mab; 
typedef Mab * MabPtr;  
Either way, the following set of prototypes give a guide as to the functionality you will need 
to provide: 
MabPtr memChk(MabPtr m, int size);   // check if memory available 
MabPtr memAlloc(MabPtr m, int size); // allocate a memory block 
MabPtr memFree(MabPtr m);            // free memory block 
  
MabPtr memMerge(MabPtr m);           // merge two memory blocks 
MabPtr memSplit(MabPtr m, int size); // split a memory block
*/

#include <stdio.h>
#include <stdlib.h>

struct mab {
    int offset;
    int size;
    int allocated;
    int processSize;
    struct mab *next;
    struct mab *prev;
};
typedef struct mab Mab;
typedef Mab *MabPtr;

MabPtr lastAlloc = NULL;
int policy = 0;

void addBlock(MabPtr *head, int size) {
    MabPtr new = malloc(sizeof(Mab));
    static int offset = 0;
    new->offset = offset;
    offset += size + 10;
    new->size = size;
    new->allocated = 0;
    new->processSize = 0;
    new->next = NULL;
    new->prev = NULL;
    
    if (!*head) {
        *head = new;
    } else {
        MabPtr curr = *head;
        while (curr->next) curr = curr->next;
        curr->next = new;
        new->prev = curr;
    }
}

MabPtr memChk(MabPtr m, int size) {
    MabPtr curr = (policy == 3 && lastAlloc) ? lastAlloc : m;
    MabPtr best = NULL;
    MabPtr start = curr;
    
    do {
        if (!curr->allocated && curr->size >= size) {
            if (policy == 0 || policy == 3) return curr;
            if (!best || (policy == 1 && curr->size < best->size) || 
                (policy == 2 && curr->size > best->size))
                best = curr;
        }
        curr = curr->next;
        if (policy == 3 && !curr) curr = m;
    } while (curr && curr != start);
    
    return best;
}

MabPtr memAlloc(MabPtr m, int size) {
    MabPtr block = memChk(m, size);
    if (!block) return NULL;
    
    block->allocated = 1;
    block->processSize = size;
    lastAlloc = block;
    return block;
}

void printMem(MabPtr m) {
    int blockNum = 1;
    printf("\n%-8s %-12s %-12s %-15s %-15s\n", 
           "Block", "Size", "Status", "Process Size", "Waste");
    printf("---------------------------------------------------------------\n");
    
    while (m) {
        printf("%-8d %-12d %-12s ", blockNum++, m->size, 
               m->allocated ? "Allocated" : "Free");
        
        if (m->allocated) {
            printf("%-15d %-15d\n", m->processSize, m->size - m->processSize);
        } else {
            printf("%-15s %-15s\n", "N/A", "N/A");
        }
        
        m = m->next;
    }
    printf("\n");
}

int main() {
    int numBlocks, blockSize, numProcesses, processSize;
    MabPtr mem = NULL;
    
    printf("How many memory blocks? ");
    scanf("%d", &numBlocks);
    
    printf("\nEnter size for each block:\n");
    for (int i = 0; i < numBlocks; i++) {
        printf("%d: ", i + 1);
        scanf("%d", &blockSize);
        addBlock(&mem, blockSize);
    }
    
    printf("\nSelect allocation policy:\n");
    printf("0 - First Fit\n");
    printf("1 - Best Fit\n");
    printf("2 - Worst Fit\n");
    printf("3 - Next Fit\n");
    printf("Choice: ");
    scanf("%d", &policy);
    
    char *policyNames[] = {"First Fit", "Best Fit", "Worst Fit", "Next Fit"};
    
    printf("How many processes to allocate? ");
    scanf("%d", &numProcesses);
    
    printf("\nEnter memory requirement for each process:\n");
    for (int i = 0; i < numProcesses; i++) {
        printf("%d: ", i + 1);
        scanf("%d", &processSize);
        memAlloc(mem, processSize);
    }
    printMem(mem);    
    return 0;
}

/*
output:
How many memory blocks? 3

Enter size for each block:
1: 200
2: 300
3: 400

Select allocation policy:
0 - First Fit
1 - Best Fit
2 - Worst Fit
3 - Next Fit
Choice: 0
How many processes to allocate? 3

Enter memory requirement for each process:
1: 250
2: 150
3: 200

Block    Size         Status       Process Size    Waste          
---------------------------------------------------------------
1        200          Allocated    150             50             
2        300          Allocated    250             50             
3        400          Allocated    200             200
*/
