#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    FILE *fp;
    fp = fopen(argv[1],"r");
    printf("Error number: %d >> ",errno);
    printf("%s\n",strerror(errno));
    perror("My msg");
    if(fp) 
    {
        pclose(fp);
        printf("File close\n");
    }
    return 0; 
}