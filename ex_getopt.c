#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char* optarg;
extern int optind,opterr,optopt;


int main(int argc, char* argv[]){
    int flags, opt;
    int nsecs1,nsecs2, tfnd;
    int inx = 0;
    nsecs1 = 0;
    nsecs2 = 0;
    tfnd = 0;
    flags =0;

    while((opt = getopt(argc,argv,"nt:")) != -1){
        switch(opt){
            case 'n':flags =1;
            
            break;
            case 't': tfnd =1;
            inx += 2;
            nsecs1 = atoi(argv[inx]);
            nsecs2 = atoi(argv[++inx]);
            optind++;
            break;
            default:
            if(opterr){
                fprintf(stderr, "Usage: %s [-t nsecs] [-n] name\n",argv[0]);
                opterr =0;
            }
            else{
                fprintf(stderr,"Invalud option: %c\n",optopt);
            }
        }
        inx++;
    }

    printf("flags=%d; tfnd=%d; nsecs1=%d; nsecs2=%d; optind=%d\n",flags,tfnd,nsecs1,nsecs2,optind);

    if (optind >= argc){
        fprintf(stderr,"Expected argument\n");
        exit(EXIT_FAILURE);
    }
    printf("name argument = %s\n",argv[optind]);
    exit(EXIT_SUCCESS);
    
}