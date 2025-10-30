/*
Write a C program to emulate the ls -l UNIX command that prints all files in a current 
directory and lists access privileges, etc. DO NOT simply exec ls -l from the program. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

void printPerms(mode_t mode) {
    printf((S_ISDIR(mode))?"d":"-");
    printf((mode&S_IRUSR)?"r":"-");
    printf((mode&S_IWUSR)?"w":"-");
    printf((mode&S_IXUSR)?"x":"-");
    printf((mode&S_IRGRP)?"r":"-");
    printf((mode&S_IWGRP)?"w":"-");
    printf((mode&S_IXGRP)?"x":"-");
    printf((mode&S_IROTH)?"r":"-");
    printf((mode&S_IWOTH)?"w":"-");
    printf((mode&S_IXOTH)?"x":"-");
}

int main() {
    DIR *dp;
    struct dirent *entry;
    struct stat sb;
    struct passwd *pw;
    struct group *gr;
    char timebuf[80];
    
    dp=opendir(".");
    if(dp==NULL) {
        perror("opendir");
        return 1;
    }
    
    while((entry=readdir(dp))!=NULL) {
        if(entry->d_name[0]=='.')
            continue;
        if(lstat(entry->d_name,&sb)==-1) {
            perror("lstat");
            continue;
        }
        
        printPerms(sb.st_mode);
        printf(" %ld",(long)sb.st_nlink);
        
        pw=getpwuid(sb.st_uid);
        printf(" %s",pw?pw->pw_name:"?");
        
        gr=getgrgid(sb.st_gid);
        printf(" %s",gr?gr->gr_name:"?");
        
        printf(" %5ld",(long)sb.st_size);
        
        strftime(timebuf,sizeof(timebuf),"%b %d %H:%M",
                 localtime(&sb.st_mtime));
        printf(" %s",timebuf);
        
        printf(" %s\n",entry->d_name);
    }
    
    closedir(dp);
    return 0;
}

/*
output:
-rw-r--r-- 1 CSE_5C2 CSE_5C2  1543 Oct 30 10:59 1.c
-rwxr-xr-x 1 CSE_5C2 CSE_5C2 16464 Oct 30 10:59 1.out
*/