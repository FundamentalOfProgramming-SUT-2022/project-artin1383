#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int ctr=0;
char strings[100][100] = {0};
char* MakeDirectoriesPath(char* array , int inputState){

    int i;
    if(inputState==1){
    for (i = strlen(array)-1; i >= 0; i--) if(array[i] == '/') break;
    }
    else {
        int slashindex =0;
        for (int k = 0; k < strlen(array); k++)
            if(array[k] == '/') 
                slashindex = k;
        for (i= slashindex; i < strlen(array); i++)
             if(array[i] == ' '){
                array[i] = '/';
                break;
             }
    }
    char* result = (char*) malloc((i+2) * sizeof(char));
    for (int j = 0; j <= i; j++) result[j] = array[j];
    result[i+1] = '\0';
    return result;
}
void makeNotExistDirectories (char *path)
{
    int j;
	int pathlength = strlen(path);
    int startindex = 0;
    while(startindex!=pathlength){
        for (j = startindex; j < pathlength; j++){
            if (path[j] == '/')break;
        }
        char *dir = malloc((j + 1) * sizeof(char));
        for (int k = 0; k < j; k++)
            dir[k] = path[k];
        dir[j] = '\0';
        startindex = j+1;
        mkdir(dir);
    }
}
void makeFile(FILE *file , char* path){
    if(access(path, F_OK)==0) printf("File is already exist.");
    file  = fopen(path, "w+");
    fclose(file);
}
void handleCreatefile(char line[]){
    //فرض یک : آخرین دایرکتوری اسپیس ندارد
    //فرض 2 : حتما بعد آخرین دایرکتوری یک اسپیس میزنند
    //اگه / نداشت بذار.
    int i;
    for ( i = 0; i < strlen(line); i++)
        if(line[i] == '-') break;
    char fullpath[200];
    for (int i = 0; i < 200; i++) fullpath[i]=0;
    if(line[i+7] == '/') {
        for (int j= i+8; j < strlen(line); j++)
          fullpath[j-i-8] = line[j];
    FILE *file;
    char* path = MakeDirectoriesPath(fullpath,1);
    printf("%s" , fullpath);
    makeNotExistDirectories(path);
    makeFile(file , fullpath);
    }
    else if(line[i+7]=='\"'){
        for (int j= i+9; j < strlen(line)-1; j++)
          fullpath[j-i-9] = line[j];
    FILE *file;
    char* path = MakeDirectoriesPath(fullpath,2);
    printf("%s", fullpath);
    makeNotExistDirectories(path);
    makeFile(file , fullpath);
    }
}
int main (){
    char line[1000];
    char copyline[1000];
    gets(line);
    strcpy(copyline , line);
    // int j=0;
    // for(int i=0;i<=(strlen(line));i++)
    // {
    //     // if space or NULL found, assign NULL into newString[ctr]
    //     if(line[i]==' '||line[i]=='\0')
    //     {
    //         strings[ctr][j]='\0';
    //         ctr++;  //for next word
    //         j=0;    //for next word, init index to 0
    //     }
    //     else
    //     {
    //         strings[ctr][j]=line[i];
    //         j++;
    //     }
    // }
    char space[2] = " ";  

    if(strcmp(strtok(copyline , space),"createfile")==0) handleCreatefile(line);
    return 0;
}
