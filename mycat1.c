#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <error.h>

int main(int argc, char* argv[]){
    FILE* fp = stdin;
    int ch;
    int line;
    int lineflag, eoln; // 플래그
    int option;

    if(argc == 1){  // 명령어만 실행한 경우
        fp = stdin;
        while((ch = fgetc(fp)) != EOF){
            fputc(ch, stdout);
        }
    }else{
        while ((option = getopt(argc, argv, "nE")) != -1){
            switch (option)
            {
            case 'n':
                line = 0;
                lineflag = 1;
                break;
            case 'E':
                eoln = 1;
                break;
            
            default:
                fprintf(stderr, "Usage: cat -n || cat -E || cat -nE\n");
                return 0;
            }
        }

        // 옵션이 뒤에 있으면 먼저 옵션을 앞으로 몰아줌
        // 시작 지점을 옵션개수 + 명령어(1개) 이후부터
        for(int i = optind; i< argc; i++){
            fp = fopen(argv[i], "r");

            if(error != 0){ // 0은 정상적으로 동작
                // 만약 안된다면 왜 에러인지. 말해주기
                if(error == 13){
                    
                }

            }


            while((ch = fgetc(fp)) != EOF){
                if(lineflag ){
                    printf("\t%d  ", line++);
                    lineflag = 0;
                }

                if(ch == '\n'){ // E 옵션
                    if(eoln){
                        fputc('$', stdout);
                    }
                    lineflag = 1;
                }
                
                fputc(ch, stdout);
            }
        }
    }

    return 0;
}