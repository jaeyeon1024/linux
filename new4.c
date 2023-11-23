#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#define MAX_FILES 100
#define BUF_SIZE 2048

extern char* optarg;
extern int optind,opterr,optopt;

void write_file(char* fp1, char* fp2){

    int ip_file, op_file;
    char buf[BUF_SIZE];

    ip_file = open(fp1,O_RDONLY);
    op_file = open(fp2,O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);

    int n;
    while((n = read(ip_file,buf,BUF_SIZE))){
        write(op_file,buf,n);
    }
    close(ip_file);
    close(op_file);
}


int main(int argc, char *argv[]) 
{
    int opt;
    struct stat statbuf1;
    struct stat statbuf2;

    int option_r = 0;
    int option_i = 0;
    int option_b = 0;
    
    while ((opt = getopt(argc, argv, "rRib")) != -1) {
        switch (opt)
        {
        case 'r': case 'R':
            option_r = 1;
            break;
        case 'i':
            option_i = 1;
            break;
        case 'b':
            option_b = 1;
        default:
            break;
        }
    }

    printf("optind : %d argc: %d \n",optind,argc);

    if (argc < 3 ){
        fprintf(stderr, "error: mycp : [파일1 파일2 ...]\n");
        exit(EXIT_FAILURE);
    }
    else if (argc-optind > MAX_FILES){
        fprintf(stderr, "mycp: Files cannot exceed 100.\n");
        exit(EXIT_FAILURE);
    }
    else if(argc-optind > 3){
        stat(argv[argc-1],&statbuf1);
        
        if (!S_ISDIR(statbuf1.st_mode)){
            fprintf(stderr, "mycp: target %s is not a directory\n",argv[argc-1]);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = optind; i < argc;i++){
        if(stat(argv[i],&statbuf1) < 0 && i != argc-1){
            fprintf(stderr, "mycp: cannot stat %s: No such file or directory\n",argv[i]);
            exit(EXIT_FAILURE);
        }
    }

    // 각 파일을 받아서 돌려보면서 존재하는 파일인지 아닌지 확인
    // 가장 마지막 파일을 제외하고는 존재해야함
    // 인자수가 3개보다 많으면 가장 마지막은 디렉토리
    // 
    printf("=======================\n");

    stat(argv[optind],&statbuf1);
    
    if (argc ==  3 && S_ISREG (statbuf1.st_mode)){
        if(access(argv[optind+1],F_OK) && option_i){      
            printf("cp: overwrite %s?(y/n)  ",argv[optind+1]);
            char ch = getchar();
            switch (ch)
            {
            case 'y':case 'Y':            
                break;
            case 'n':case'N':
            exit(EXIT_FAILURE);
            default:
                break;
            }
        }
        if(access(argv[optind+1],F_OK) && option_b){ // 백업 파일 만들기
    
        }

        write_file(argv[optind],argv[optind+1]);
    }




    

    return 0;
}
