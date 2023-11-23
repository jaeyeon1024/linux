#include <stdio.h>

int main(int argc, char* argv[]){
    FILE* src, *dest;
    char ch;

    if(argc != 3) {// 인자가 반드시 3개여야 함
        printf("Usage : mycp a b \n");
        return -1;
    }

    src = fopen(argv[1], "r");
    dest = fopen(argv[2], "w");

    while((ch = fgetc(src)) != EOF){
        fputc(ch, dest);
    }
}