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
#include <sys/ioctl.h>


#define option_count 10



typedef struct File_Info
{
    char path[NAME_MAX];
    char name[NAME_MAX];
    char* Is_dir; // 디렉토리인지 아닌지
    char* mod[9]; // 권한
    int hard_c; //하드링크 개수 
    int b_size; // 블럭개수
    char* user_name;
    char* group_name;
    int size; // 크기
    int mon_time;
    int day_time;
    int clo_time;
    int min_time;
    int i_node; // inode
    char* indicator;// 지시자 
    char indicator_name[NAME_MAX+1];
}FI;

typedef struct Info_Length
{
    int path;
    int name;
    int hard_c; //하드링크 개수 
    int b_size; // 블럭개수
    int user_name;
    int group_name;
    int size; // 크기
    int i_node; // inode
    int indicator;
    
}IL;

typedef struct Max_Length
{
    int total;
    int path;
    int name;
    int hard_c; //하드링크 개수 
    int b_size; // 블럭개수
    int user_name;
    int group_name;
    int size; // 크기
    int i_node; // inode
    
}ML;

void mod_setting(FI* fi){
    struct stat statbuf1;

    stat((fi)->path,&statbuf1);

    if (S_ISDIR(statbuf1.st_mode)){
                (fi)->Is_dir = "d";
            }
            else{
                (fi)->Is_dir = "-";
            }

            if ((S_IRUSR&statbuf1.st_mode) != 0){
                (fi)->mod[0] = "r";
            }
            else{
                (fi)->mod[0] = "-";
            }

            if ((S_IWUSR&statbuf1.st_mode) != 0){
                (fi)->mod[1] = "w";
            }
            else{
                (fi)->mod[1] = "-";
            }

            if ((S_IXUSR&statbuf1.st_mode) != 0){
                (fi)->mod[2] = "x";
            }
            else{
                (fi)->mod[2] = "-";
            }

            if ((S_IRGRP&statbuf1.st_mode) != 0){
                (fi)->mod[3] = "r";
            }
            else{
                (fi)->mod[3] = "-";
            }

            if ((S_IWGRP&statbuf1.st_mode) != 0){
                (fi)->mod[4] = "w";
            }
            else{
                (fi)->mod[4] = "-";
            }

            if ((S_IXGRP&statbuf1.st_mode) != 0){
                (fi)->mod[5] = "x";
            }
            else{
                (fi)->mod[5] = "-";
            }
            
             if ((S_IROTH&statbuf1.st_mode) != 0){
                (fi)->mod[6] = "r";
            }
            else{
                (fi)->mod[6] = "-";
            }

            if ((S_IWOTH&statbuf1.st_mode) != 0){
                (fi)->mod[7] = "w";
            }
            else{
                (fi)->mod[7] = "-";
            }

            if ((S_IXOTH&statbuf1.st_mode) != 0){
                (fi)->mod[8] = "x";
            }
            else{
                (fi)->mod[8] = "-";
            }
}

int check_range(int start,int end, int flag){
    if(flag){
        if (end >= start) return 0;
        else return  1;
    }
    if (end <= start) return 0;
    else return  1;
}
int print_blank() {

    int fd, i;

    struct winsize ws;

    fd = fileno(stdin);

    ioctl(fd, TIOCGWINSZ, &ws); // 예외처리 해야함

    return ws.ws_col;

}

void dir_print(FI *p_fi,int options[option_count], int count , ML ml, IL* p_il){

    char * mon[12] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
    //printf("name : %s\n", p_fi[0].path);
    
    

    if (count == 2 && options[1]==0){
        printf("\n");
        return;
    }

    int width_sums = 0;// 가로 확인
    int line_max = 0; // 
    int line_tmp = 0;
    int width = print_blank()- 10;
    int line = 1;
    int wi_flag =1;


    for ( line = 1; line < count; line++){
        width_sums = 0;
        wi_flag = 1;
        for(int j = 0; j < count; j +=line ){
            line_max = 0;
            for(int k = 0; k <= line; k++){
                line_tmp = 0;
                if (options[2]){
                    line_tmp += ml.i_node +1;
                }
                if (options[3]){
                    line_tmp += ml.b_size+1;
                }
                if(options[4]){
                    line_tmp += p_il[j+k].indicator;
                }
                line_tmp += p_il[j+k].name +2;
                if(line_max < line_tmp){
                    line_max = line_tmp;
                }
            }
            width_sums += line_max;
            if(width_sums > width){
                wi_flag = 0;
                continue;
            }
        }
        if(wi_flag && line != 1)
        {
            line--;
            break;
        }
    }

    //printf("width :%d %d \n",width,line);


    int start = 0;
    int end = count;
    int dx = line;
    int flag =0;
    

    int tmp3 =0;
    
    if(options[0] == 0){
        for (int i = 0; i< line ; i++){
            for(int j = 0; j < count;){
                
                
                tmp3 =0;
                if(p_fi[j+i].name[0] == '.' && options[1]==0) {
                    j+= 1;
                    continue;
                }
                int tmp4 = 0;
                
                for(int k = 0;k<line;k++){
                    
                    tmp4 = 0;
                    if(j+k >= count) continue;
                    tmp4 += p_il[j+k].name; 
                    if(tmp4 > tmp3) tmp3 = tmp4;
                }

                if(j+i < count){
                if (options[2]){
                    printf("%*d ",ml.i_node,p_fi[i+j].i_node);
                }
                if (options[3]){
                    printf("%*d ",ml.b_size,p_fi[i+j].b_size);
                }
                
                    if(p_fi[i+j].indicator == "/"){
                        
                        if (options[4]){
                            printf("\033[1;34m");
                            printf("%-*s  ",tmp3,(p_fi)[i+j].indicator_name);
                            printf("\033[0m");
                        }
                        else{
                            printf("\033[1;34m");
                            printf("%-*s  ",tmp3,(p_fi)[i+j].name);
                            printf("\033[0m");
                        }
                    }
                    else if(p_fi[i+j].indicator == "*"){
                        if (options[4]){
                            printf("\033[1;32m");
                            printf("%-*s  ",tmp3,(p_fi)[i+j].indicator_name);
                    
                            printf("\033[0m");
                        }
                        else{
                            printf("\033[1;32m");
                            printf("%-*s  ",tmp3,(p_fi)[i+j].name);
                            printf("\033[0m");
                        }
                    }
                    else if(p_fi[i+j].indicator == "="){
                        if (options[4]){
                            printf("\033[1;35m");
                            printf("%-*s  ",tmp3,(p_fi)[i+j].indicator_name);
                    
                            printf("\033[0m");
                        }
                        else{
                            printf("\033[1;35m");
                            printf("%-*s  ",tmp3,(p_fi)[i+j].name);
                            printf("\033[0m");
                        }
                    }
                    else if(p_fi[i+j].indicator == "@"){
                        if (options[4]){
                            printf("\033[0;35m");
                            printf("%-*s  ",tmp3,(p_fi)[i+j].indicator_name);
                    
                            printf("\033[0m");
                        }
                        else{
                            printf("\033[0;35m");
                            printf("%-*s  ",tmp3,(p_fi)[i+j].name);
                    
                            printf("\033[0m");
                        }
                    }
                    else{
                        printf("%-*s  ",tmp3,(p_fi)[i+j].name);
                    }
                }
                j+= line;
            }
            printf("\n");
            
        }
    }
/*
    for(int s = 0; s < line; s++){
        int i = 0;
        
        for (i; i < count ; i += line){
            if (options[1] == 0 && ((p_fi)[i+s].name[0] == '.')) // -a
            {
                i = i -(line-1);
                continue;
            }
            if(options[2]){
                printf("%*d",ml.i_node,p_fi[i+s].i_node);
            }
            if(options[3]){
                printf("%*d",ml.b_size,p_fi[i+s].b_size);
            }
            tmp3 = 0;
            int tmp4 = 0;
            for (int j = 0; j < line; j++){
                tmp4 = 0;
                if (options[2]){
                    tmp4 += p_il[j+i].i_node;
                }
                if (options[3]){
                    tmp4 += p_il[j+i].b_size;
                }
                if(options[4]){
                    tmp4 += p_il[j+i].indicator;
                }
                tmp4 += p_il[j+i].name;

                if(tmp3 < tmp4){
                    tmp3 = tmp4;
                }
            }
            

            if(p_fi[i+s].indicator == "/"){
                printf("\033[1;34m");
                printf("%*s",tmp3,(p_fi)[i+s].name);
                printf("\033[0m");
                if (options[4]){
                    printf("/ ");
                }
            }
            else if(p_fi[i+s].indicator == "*"){
                printf("\033[1;32m");
                printf("%*s",tmp3,(p_fi)[i+s].name);
                
                printf("\033[0m");
                if (options[4]){
                    printf("*");
                }
            }
            else if(p_fi[i+s].indicator == "="){
                printf("\033[1;35m");
                printf("%*s",tmp3,(p_fi)[i+s].name);
                
                printf("\033[0m");
                if (options[4]){
                    printf("= ");
                }
            }
            else if(p_fi[i+s].indicator == "@"){
                printf("\033[0;35m");
                printf("%*s",tmp3,(p_fi)[i+s].name);
            
                printf("\033[0m");
                if (options[4]){
                    printf("@ ");
                }
            }
            else{
                printf("%*s",tmp3,(p_fi)[i+s].name);
            }
            printf(" ");

        }
    }
*/
    else{
        for(int i = 0; i<count; i++){// 이후에 2중 for문으로 한줄 씩 출력 출력 높이만큼 나누기 -ㅣ?
        
            if (options[1] == 0 && ((p_fi)[i].name[0] == '.')) // -a
            {
                continue;
            }
            if(options[2]){
                printf("%*d ",ml.i_node,p_fi[i].i_node);
            }
            if(options[3]){
                printf("%*d ",ml.b_size,p_fi[i].b_size);
            }

            if (options[0]){
                
                printf("%s",p_fi[i].Is_dir);
                for(int k = 0; k < 9 ; k++)
                    printf("%s",p_fi[i].mod[k]);

                printf("% *d ",ml.hard_c,p_fi[i].hard_c);
                
                printf("%*s ",ml.user_name,p_fi[i].user_name);
                printf("%*s ",ml.group_name,p_fi[i].group_name);
                printf("%*d ",ml.size,p_fi[i].size);
                printf("%3s %2d %2d:%2d ",mon[p_fi[i].mon_time],p_fi[i].day_time,p_fi[i].clo_time,p_fi[i].min_time);
            }

            if(p_fi[i].indicator == "/"){
                        
                if (options[4]){
                    printf("\033[1;34m");
                    printf("%-*s  ",ml.name+1,(p_fi)[i].indicator_name);
                    printf("\033[0m");
                }
                else{
                    printf("\033[1;34m");
                    printf("%-*s  ",ml.name,(p_fi)[i].name);
                    printf("\033[0m");
                    }
                }
            else if(p_fi[i].indicator == "*"){
                if (options[4]){
                    printf("\033[1;32m");
                    printf("%-*s  ",ml.name+1,(p_fi)[i].indicator_name);
                    printf("\033[0m");
                }
                else{
                    printf("\033[1;32m");
                    printf("%-*s  ",ml.name,(p_fi)[i].name);
                    printf("\033[0m");
                }
            }
            else if(p_fi[i].indicator == "="){
                if (options[4]){
                    printf("\033[1;35m");
                    printf("%-*s  ",ml.name+1,(p_fi)[i].indicator_name);
                    printf("\033[0m");
                }
                else{
                    printf("\033[1;35m");
                    printf("%-*s  ",ml.name,(p_fi)[i].name);
                    printf("\033[0m");
                }
            }
            else if(p_fi[i].indicator == "@"){
                if (options[4]){
                    printf("\033[0;35m");
                    printf("%-*s  ",ml.name+1,(p_fi)[i].indicator_name);
                    printf("\033[0m");
                }
                else{
                    printf("\033[0;35m");
                    printf("%-*s  ",ml.name,(p_fi)[i].name);
                    printf("\033[0m");
                }
            }
            else{
                printf("%-*s  ",ml.name,(p_fi)[i].name);
            }
        

            

            
            //printf("%d\n",a);

            printf("\n");
        }
    }
}


int number_of_digits(int n)
{
    if(n == 0){
        return 0;
    }
    while(n != 0)
    {
        return 1 + number_of_digits(n/10);
    }
    return 0;
}

void dir_setting(char* filename, int options[option_count] ){

    DIR* dp;
    
    char file_path[NAME_MAX];
    memset(file_path,0,NAME_MAX);

    struct dirent *dirp; 
    struct stat statbuf1;

    struct passwd *user_info;
	struct group *group_info;
	struct tm *tm_info;
    int dir_check = 0;

    struct  dirent **namelist;
    int     count;
    int     idx;

    FI* p_fi = 0;
    IL* p_il = 0;
    ML ml = {0,};
    

   

    

    // file_path[0] ='\0';
    //strcat(file_path,filename);

    //strcat(file_path,"/");
            
    // strcat(file_path,dirp->d_name);

    //printf("path : < %s >\n",filename);

    if(options[5]){
        printf("%s:\n",filename);
    }

    if((count = scandir(filename, &namelist, NULL, alphasort)) == -1) {
        if(!access(filename,F_OK)){
            printf("%s\n",filename);
            return;
        }
        fprintf(stderr, "myls: %s Directory access Error: %s\n", filename, strerror(errno));
        return;
    }
    
    p_fi = (FI*)malloc(sizeof(FI)*(count+1));
    p_il = (IL*)malloc(sizeof(IL)*(count+1));
    
    
    int i = 0;
    int dx = 1;
    int start = 0;
    int end = count;
    
    if(options[6]){
        dx = -1;
        start = count-1;
        end = -1;
    }

    for (int i = start; check_range(i,end,options[6]); i += dx){
        //printf("%s \n", namelist[i]->d_name);
        //printf("%d \n",i);
        file_path[0] = '\0';
        strcat(file_path,filename);

        strcat(file_path,"/");
        
        if(options[6]){
            strcat(file_path,namelist[count - i-1]->d_name);    
        }
        else{
            strcat(file_path,namelist[i]->d_name);
        }

        if(lstat(file_path,&statbuf1) < 0){
            fprintf(stderr, "myls: cannot open or stat %s: No such file or directory\n",file_path);
            return;
        }

        

        //p_fi[i].path = file_path;
        strcpy(p_fi[i].path,file_path);
        //p_fi[i].name = namelist[i]->d_name;

        if(options[6]){
            strcpy(p_fi[i].name,namelist[count - i -1]->d_name);    
        }
        else
        {
            strcpy(p_fi[i].name,namelist[i]->d_name);
        }

        //printf("paths : %s\n",p_fi[i].path);

        p_il[i].path = strlen(file_path);

        if(options[6])
            p_il[i].name = strlen(namelist[count -i -1]->d_name);
        else
            p_il[i].name = strlen(namelist[i]->d_name);

        mod_setting(&(p_fi[i])); // 권한 
        
        user_info = getpwuid(statbuf1.st_uid);
		group_info = getgrgid(statbuf1.st_gid);
		tm_info = localtime(&statbuf1.st_mtime);


        p_fi[i].size = statbuf1.st_size; // 사이즈 
        p_fi[i].b_size = statbuf1.st_blocks/2; // 블럭수
        p_fi[i].hard_c = statbuf1.st_nlink; // 하드링크수
        p_fi[i].user_name = user_info->pw_name; // 이름
        p_fi[i].group_name = group_info->gr_name; // 이름
        
        if (options[6]) p_fi[i].i_node = namelist[count -i -1]->d_ino; // i넘버
        else p_fi[i].i_node = namelist[i]->d_ino; // i넘버
        

        p_fi[i].clo_time = tm_info->tm_hour; //시간
        p_fi[i].day_time = tm_info->tm_mday;
        p_fi[i].min_time = tm_info->tm_min;
        p_fi[i].mon_time = tm_info->tm_mon;
        

        p_il[i].b_size = number_of_digits(p_fi[i].b_size);
        p_il[i].hard_c = number_of_digits(p_fi[i].hard_c);
        p_il[i].i_node = number_of_digits(p_fi[i].i_node);
        p_il[i].size = number_of_digits(p_fi[i].size);

        p_il[i].group_name = strlen(p_fi[i].group_name);
        p_il[i].user_name = strlen(p_fi[i].user_name);

        if(p_fi[i].name[0] != '.'| options[1])
            ml.total += p_fi[i].b_size;

        p_il[i].indicator = 1;

        if (S_ISDIR(statbuf1.st_mode)){ // 지시자 
            p_fi[i].indicator = "/";
            strcpy(p_fi[i].indicator_name,p_fi[i].name);
            strcat(p_fi[i].indicator_name,"/");
        }
        else if(S_ISLNK(statbuf1.st_mode)){
            p_fi[i].indicator = "@";
            strcpy(p_fi[i].indicator_name,p_fi[i].name);
            strcat(p_fi[i].indicator_name,"@");
        }
        else if(S_ISSOCK(statbuf1.st_mode)){
            p_fi[i].indicator = "=";
            strcpy(p_fi[i].indicator_name,p_fi[i].name);
            strcat(p_fi[i].indicator_name,"=");
        }
        else if((statbuf1.st_mode & S_IXUSR) != 0){
            p_fi[i].indicator = "*";
            strcpy(p_fi[i].indicator_name,p_fi[i].name);
            strcat(p_fi[i].indicator_name,"*");
        }
        else if((statbuf1.st_mode & S_IXGRP) != 0){
            p_fi[i].indicator = "*";
            strcpy(p_fi[i].indicator_name,p_fi[i].name);
            strcat(p_fi[i].indicator_name,"*");
        }
        else if((statbuf1.st_mode & S_IXOTH) != 0){
            p_fi[i].indicator = "*";
            strcpy(p_fi[i].indicator_name,p_fi[i].name);
            strcat(p_fi[i].indicator_name,"*");
        }
        else{
            p_fi[i].indicator = "";
            p_il[i].indicator = 0;
        }
        
        
        
        if(ml.b_size < p_il[i].b_size) ml.b_size = p_il[i].b_size;
        if(ml.group_name < p_il[i].group_name) ml.group_name = p_il[i].group_name;
        if(ml.hard_c <  p_il[i].hard_c) ml.hard_c = p_il[i].hard_c;
        if(ml.i_node < p_il[i].i_node) ml.i_node = p_il[i].i_node;
        if(ml.name < p_il[i].name) ml.name = p_il[i].name;
        if(ml.path < p_il[i].path) ml.path = p_il[i].path;
        if(ml.size < p_il[i].size) ml.size = p_il[i].size;
        if(ml.user_name < p_il[i].user_name) ml.user_name = p_il[i].user_name;
     
    }
    
    
    if(options[0]){
        printf("total %d\n",ml.total);
    }
    dir_print(p_fi,options,count,ml,p_il);

    if(options[5]){
        printf("\n\n");
        for(int i = 0; i < count; i++){
            if(p_fi[i].Is_dir == "d" && p_fi[i].name[0] != '.'&&p_fi[i].indicator != "@"){
                
                //printf("path : %s\n",p_fi[i].path);
                dir_setting(p_fi[i].path,options);
            }
        }
    }

    /*

    if((dp =opendir(filename)) == NULL ){
        fprintf(stderr, "myls: cannot stat %s: No such file or directory\n",filename);
        return;
    }
    
    while(dirp = readdir(dp)){ // 특정 배열에서 하나씩 빼와서



    
    
    이 부분에 대충readdir 할 필요 없이 바로 파일 이름 붙이고 똑같이 처리
    
    
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

    */
}
void check_option(int argc, char *argv[], int options[option_count] ){
    struct stat statbuf1;

    if (optind == argc){
        dir_setting(".",options);
    }
    else if(argc - optind == 1){
        
        dir_setting(argv[optind],options);
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
                    printf("\n%s : \n",argv[i]);
                    dir_setting(argv[i],options);
                }
            }
        }
    }
}

//int argc, char *argv[]
int main(int argc, char *argv[]) {
    // int argc = 2;
    //  char* argv[4] = {"myls","-R"};

    
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
            options[6] = 1;
            break;
            default:
                fprintf(stderr, "myls : %s [-l] [-a] [디렉토리1 디렉토리2 ...]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }


   

    check_option(argc,argv,options);

    

}