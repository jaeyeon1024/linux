#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void write_func(int ch, int* check_first,int option_number, int option_end_maker,int* line_number ){

    if (option_number && *check_first) {
                printf("\t%d ", *line_number);
                (*line_number)++;
                *check_first = 0;
            }

            if (option_end_maker && ch == '\n') {
                    putchar('$');    
            }
            fputc(ch,stdin);


            if (ch =='\n'){
                *check_first = 1;
            }
}

int main(int argc, char *argv[]) {
    int option_number = 0;
    int option_end_maker = 0;
    int check_first = 1;
    int line_number= 1;
    int opt;
    while ((opt = getopt(argc, argv, "En")) != -1) {
        switch (opt) {
            case 'n':
                option_number = 1;
                break;
            case 'E':
                option_end_maker = 1;
                break;
            default:
                fprintf(stderr, "mycat : %s [-n] [-E] [파일1 파일2 ...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (optind == argc) {
        
        char ch;
        
        while ((ch = fgetc(stdin)) != EOF) {
            write_func(ch, &check_first,option_number, option_end_maker,&line_number);
        }
    } else {
        for (int i = optind; i < argc; i++) {

            FILE *file = fopen(argv[i], "r");

            if (file == NULL) {
                perror(argv[i]);
                continue;
            }

            char ch;
            
            while ((ch = fgetc(file)) != EOF) {
                write_func(ch, &check_first,option_number, option_end_maker,&line_number);
            } 
            fclose(file);
        }
    }

    return 0;
}
