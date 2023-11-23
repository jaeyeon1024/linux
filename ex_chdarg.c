#include <stdio.h>

int main (int argc, char * argv[])
{
    printf("Number of arguments: %d\n",argc);
    if (argc < 2)
        printf("%s : no arguments\n",argv[0]);
    else 
        for (int i = 1; i < argc; ++i){
            if(*argv[i] == '-')
                printf("option: %c\n",argv[i][1]);
        }
    return 0;   
}