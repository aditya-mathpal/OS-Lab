/*
Write a program that will list all files in a current directory and all files in subsequent 
subdirectories.
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

void printDir(char *dir,int depth) {
    DIR *dp;
    struct dirent *entry;
    struct stat sb;
    
    dp=opendir(dir);
    if(dp==NULL) {
        fprintf(stderr,"Cannot open directory: %s\n",dir);
        return;
    }
    
    chdir(dir);
    
    while((entry=readdir(dp))!=NULL) {
        lstat(entry->d_name,&sb);
        
        if(S_ISDIR(sb.st_mode)) {
            if(strcmp(".",entry->d_name)==0||
               strcmp("..",entry->d_name)==0)
                continue;
            printf("%*s%s\n",depth,"",entry->d_name);
            printDir(entry->d_name,depth+4);
        } else {
            printf("%*s%s\n",depth,"",entry->d_name);
        }
    }
    
    chdir("..");
    closedir(dp);
}

int main() {
    printf("Directory listing:\n");
    printDir(".",0);
    return 0;
}

/*
output:
Directory listing:
1.c
2.c
2.out
1.out
*/