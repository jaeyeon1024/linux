#define _GNU_SOURCE

#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define option_count 10



typedef struct File_Info
{
    char * path;
    char * name;
    int Is_dir; // 디렉토리인지 아닌지
    int mod[8]; // 권한
    int hard_c; //하드링크 개수 
    int b_size; // 블럭개수
    char* user_name;
    char* group_name;
    int size; // 크기
    int mon_time;
    int day_time;
    int clo_time;
    int min_time;
    int i_node;
    int indicator;
}FI;


void dir_print(char* filename, int options[option_count] ){
    DIR* dp;

    char file_path[NAME_MAX];
    memset(file_path,0,NAME_MAX);

    struct dirent *dirp;
    struct stat statbuf1;

    struct passwd *user_info;
	struct group *group_info;
	struct tm *tm_info;
    int dir_check = 0;



    if((dp =opendir(filename)) == NULL ){
        fprintf(stderr, "myls: cannot stat %s: No such file or directory\n",filename);
        return;
    }
    
    while(dirp = readdir(dp)){ // 특정 배열에서 하나씩 빼와서



    /*
    
    이 부분에 대충readdir 할 필요 없이 바로 파일 이름 붙이고 똑같이 처리
    
    */
        file_path[0] ='\0';
        strcat(file_path,filename);

        strcat(file_path,"/");
            
        strcat(file_path,dirp->d_name);
    
        if(stat(file_path,&statbuf1) < 0){
            fprintf(stderr, "myls: cannot open or stat %s: No such file or directory\n",file_path);
            return;
        }

        if((dirp->d_name)[0] == 46 && options[1] == 0){
            continue;
        }

        if(options[5] && S_ISDIR(statbuf1.st_mode)){
            dir_check =1;
        }

        if (options[2]){
            printf("%ld ",dirp->d_ino);
        }
        if(options[3]){ 
            printf("%ld ",statbuf1.st_blocks/2);
        }

        if (options[0]){    
            if (S_ISDIR(statbuf1.st_mode)){
                printf("d");
            }
            else{
                printf("-");
            }

            if ((S_IRUSR&statbuf1.st_mode) != 0){
                printf("r");
            }
            else{
                printf("-");
            }

            if ((S_IWUSR&statbuf1.st_mode) != 0){
                printf("w");
            }
            else{
                printf("-");
            }

            if ((S_IXUSR&statbuf1.st_mode) != 0){
                printf("x");
            }
            else{
                printf("-");
            }

            if ((S_IRGRP&statbuf1.st_mode) != 0){
                printf("r");
            }
            else{
                printf("-");
            }

            if ((S_IWGRP&statbuf1.st_mode) != 0){
                printf("w");
            }
            else{
                printf("-");
            }

            if ((S_IXGRP&statbuf1.st_mode) != 0){
                printf("x");
            }
            else{
                printf("-");
            }
            
             if ((S_IROTH&statbuf1.st_mode) != 0){
                printf("r");
            }
            else{
                printf("-");
            }

            if ((S_IWOTH&statbuf1.st_mode) != 0){
                printf("w");
            }
            else{
                printf("-");
            }

            if ((S_IXOTH&statbuf1.st_mode) != 0){
                printf("x");
            }
            else{
                printf("-");
            }
            printf(" ");

            printf("%ld ",statbuf1.st_nlink);

            user_info = getpwuid(statbuf1.st_uid);
			group_info = getgrgid(statbuf1.st_gid);
			tm_info = localtime(&statbuf1.st_mtime);

            if (user_info->pw_name) printf("%s ",user_info->pw_name);
            else printf("----");

            if (group_info->gr_name) printf("%s ",group_info->gr_name);
            else printf("----");

            if (statbuf1.st_size) printf("%6ld ",statbuf1.st_size);
            else printf("----");

            printf("%2d %2d %2d:%2d ",tm_info->tm_mon,tm_info->tm_mday,tm_info->tm_hour,tm_info->tm_min);
        }

        printf("%s",dirp->d_name);

        if (options[4]){
            if (S_ISDIR(statbuf1.st_mode)){
                printf("/");
            }
            else if((statbuf1.st_mode & S_IXUSR) != 0){
                printf("*");
            }
            else if((statbuf1.st_mode & S_IXGRP) != 0){
                printf("*");
            }
            else if((statbuf1.st_mode & S_IXOTH) != 0){
                printf("*");
            }
            else if(S_ISSOCK(statbuf1.st_mode)){
                printf("=");
            }
            else if(S_ISLNK(statbuf1.st_mode)){
                printf("@");
            }
        }
        printf("\n");
    }

    
    closedir(dp);

    if((dp =opendir(filename)) == NULL && options[5]){
        fprintf(stderr, "mylst: cannot stat %s: No such file or directory\n",filename);
        return;
    }
    else if(options[5]){
        while(dirp = readdir(dp)){
            
            if((dirp->d_name)[0] == 46){
                continue;
            }
            
            file_path[0] ='\0';
            strcat(file_path,filename);
            
            strcat(file_path,"/");
            
            strcat(file_path,dirp->d_name);
                       
            if(stat(file_path,&statbuf1)<0){
                fprintf(stderr, "mylst: cannot stat or open  %s: No such file or directory\n",file_path);
                return;
            }

            if(S_ISDIR(statbuf1.st_mode)){
                
                printf("\n%s: \n",file_path);
                dir_print(file_path,options);
            }
            
            
        }
    }


}
void check_option(int argc, char *argv[], int options[option_count] ){
    struct stat statbuf1;

    if (optind == argc){
        dir_print(".",options);
    }
    else if(argc - optind == 1){
        
        dir_print(argv[optind],options);
    }
    else{
        for(int k = 0; k < 2 ; k++){
            for(int i = optind; i < argc; i ++){
                stat(argv[i],&statbuf1);
                if(S_ISREG(statbuf1.st_mode) ){
                    if (k == 1) continue;
                    printf("\n %s \n",argv[i]);
                }
                else if (k ==0){
                    continue;
                }
                else{
                    printf(" \n\n%s : \n\n",argv[i]);
                    dir_print(argv[i],options);
                }
            }
        }
    }
}

//int argc, char *argv[]
int main(int argc, char *argv[]) {
    // int argc = 4;
    // char* argv[4] = {"myls","-R","-l",".."};

    
    int opt;
    struct stat statbuf1;
    int check;
    
    int options[option_count] = {0,};

    while ((opt = getopt(argc, argv, "laisFRr")) != -1) {
        switch (opt) {
            case 'l':
            options[0] = 1;
            break;
            case 'a':
            options[1] = 1;
            break;
            case 'i':
            options[2] = 1;
            break;
            case 's':
            options[3] = 1;
            break;
            case 'F':
            options[4] = 1;
            break;
            case 'R':
            options[5] = 1;
            break;
            case 'r':
            break;
            default:
                fprintf(stderr, "myls : %s [-l] [-a] [디렉토리1 디렉토리2 ...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }


   

    check_option(argc,argv,options);

    

}