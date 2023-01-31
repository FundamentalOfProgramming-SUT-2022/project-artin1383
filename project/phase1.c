#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <winuser.h>
#include <winbase.h>
#include <stdbool.h>
#include <dirent.h>
char* fullstring;
int truelines =0;
int lastlineindex;
int lastindexoffile = 0;
    //fopen / avalesh nabaiad bashe
    //hide kardan undo indent insert remove exportfirstcommand
    //tree arman bog dare
    //khata ha
int countlinechars(char* path , int targetLine){
  FILE* file = fopen(path , "r");
  int currentLine = 1;
  int count = 0;
  char c = getc(file);
  while(1){
    if(currentLine == targetLine){
      count ++;
      while(1){
        c = getc(file);
        count++;
        if(c == '\n' || c == EOF)break;
      }
    }
    if(c == '\n' || c ==EOF) currentLine ++;
    if(c == EOF) break;
    c = getc(file);
  }
  return count;
}
char* deletelaststar(char * pattern){
  char* newpattern = malloc(200);
  int i;
  for (i = 0; i < strlen(pattern); i++)
  {
    if(pattern[i] == '*') break;
    newpattern[i] = pattern[i];
  }
  newpattern[i] = '\0';
  return newpattern;
}
char* deletefirststar(char * pattern){
  char* newpattern = malloc(200);
  for (int i = 1; i < strlen(pattern); i++) newpattern[i-1] = pattern[i];
  newpattern[strlen(pattern) -1] = '\0';
  return newpattern;
}
int chackPattern(char* temp , char*pattern){
  for (int i = strlen(temp) -1; i >= 0; i--){
    int index = i - strlen(temp) + strlen(pattern);
    if(index <0) break;
    if(temp[i] != pattern[index]) return 0;
  }
return 1;
}
int checkWord(char *word, char* pattern)
{
    int* numbers = malloc(2* sizeof(int));
    int count = 0;
    int index = 0;
    int preindex = index;
    int ignore = 0;
    int tempsize = 0;
    char* temp = malloc(200);
    for (int i = 0; i < 200; i++)temp[i] = 0;
    while(word[index] != '\0'){
      temp[preindex] = word[index];
      if(strstr(pattern , "*") != NULL) ignore =1;
      if((ignore ==0) && temp[preindex] != pattern[preindex]) {
      for (int i = 0; i < 200; i++)temp[i] = 0;
      } 
      else preindex++;
      if(strcmp(pattern , temp) == 0){
        preindex = 0;
        count++;
        for (int i = 0; i < 200; i++)temp[i] = 0;
      }
      if (strstr(temp , deletelaststar(pattern)) != NULL &&strlen(deletelaststar(pattern))>0 && ignore ==1 && index == strlen(word) -1) 
        count++;
      if (strstr(temp , deletefirststar(pattern)) != NULL &&strlen(deletefirststar(pattern))>0 && ignore ==1 && chackPattern(temp , deletefirststar(pattern)) ==1) 
        count=1;
      index++;
    }
    return count;
}
void findText(char filename[],char pattern[], int isArman , int iscount, int at , int byword , int isall){
  FILE *file = fopen(filename, "r+");
  char* target  = malloc(200);
  for (int i = 0; i < 200; i++) target[i] = 0;
  int index =0;
  int count = 0;
  int pos =0;
  int istargetom = 1;
  int wordcount = 0;
  char c = getc(file);
  if(byword == 0 && strstr(pattern , "*") == NULL)
  while(c!=EOF){
    if(c!='\n' && c!= '\0' && c!= EOF) target[index] = c;
    if(pattern[index] != target[index]) {
    for (int i = 0; i < 200; i++) target[i] = 0;
    index = 0;
    }
    if(strcmp(pattern , target) == 0){
      if (isall == 1) istargetom == 1 ? printf("%d" , pos - strlen(pattern) + 1) : printf(", %d" , pos - strlen(pattern) + 1); 
      else if(istargetom == at && iscount ==0) printf("%d" , pos - strlen(pattern) + 1);
      istargetom ++;
      count++;
      for (int i = 0; i < 200; i++) target[i] = 0;
      index = 0;
    }
    if(c!='\n' && c!= '\0' && c!= EOF && strcmp(pattern , target) != 0&& pattern[index] == target[index])  index ++;
    pos++;
    c= getc(file);
  }
  else 
  while(1){
    if(c!='\n' && c!= '\0' && c!= ' ' && c!= EOF) target[index] = c;
    if((c == ' ' || c==EOF || c=='\n' )&& strlen(target) > 0){
      wordcount++;
      if (checkWord(target , pattern) != 0){
      for (int i = 0; i < checkWord(target , pattern); i++)
      {
        if(byword ==1){
        if (isall == 1) istargetom == 1 ? printf("%d" , wordcount) : printf(", %d" , wordcount); 
        else if(istargetom == at && iscount ==0) printf("%d" , wordcount);
        }
        else if (pattern[strlen(pattern) -1] == '*'){
        int i;
        for (i = 0; i < strlen(target); i++)
        if(target[i] == deletelaststar(pattern)[0]) break;
        int result = pos - strlen(target) + i;
        if (isall == 1) istargetom == 1 ? printf("%d" ,result ) : printf(", %d" , result); 
        else if(istargetom == at && iscount ==0) printf("%d" , result);
        }
        else{
        int i;
        for (i = strlen(target) -1; i >0 ; i--)
        if(target[i] == deletefirststar(pattern)[0]) break;
        int result = pos - strlen(target) + i;
        if (isall == 1) istargetom == 1 ? printf("%d" ,result ) : printf(", %d" , result); 
        else if(istargetom == at && iscount ==0) printf("%d" , result);
        }
        istargetom ++;
      }
      count+= checkWord(target , pattern);
      }
      for (int i = 0; i < 200; i++) target[i] = 0;
      index = 0;
      if(c == EOF) break;
    }
    if(c!='\n' && c!= '\0' && c!= ' ' && c!= EOF)  index ++;
    pos++;
    if(c == EOF) break;
    c= getc(file);
  }
  if(iscount==1) printf("%d" , count);
  fclose(file);
}
long long countcurrentLinesOfFile(char filename[]){
  FILE *f1 = fopen(filename, "r");
  char c = getc(f1);
  int currentLine = 1;
  while(c!= EOF){
    if(c == '\n') currentLine ++;
    c = getc(f1);
  }
  return currentLine;
}
void compareText(char filename[]  , char filename2[]){
  FILE *f1 = fopen(filename, "r");
    FILE *f2 = fopen(filename2, "r");
    char c1 = fgetc(f1);
    char c2 = fgetc(f2);
    long long currentLine =1;
    char* currentLine1 = malloc(1000000);
    char* currentLine2 = malloc(1000000);
    for (int i = 0; i < 1000000; i++) currentLine1[i]=0;
    for (int i = 0; i < 1000000; i++) currentLine2[i]=0;
    int isexistdiffrence =0;
    int index= 0;
    long long countoflines1 = countcurrentLinesOfFile(filename);
    long long countoflines2 = countcurrentLinesOfFile(filename2);
    while (1)
    {
        if(currentLine1[index-1]!= '\n')currentLine1[index] = c1;
        if(currentLine2[index-1]!= '\n')currentLine2[index] = c2;
        index++;
      if((c1 == '\n' || c1 == EOF) && (c2 == '\n' || c2 == EOF) ) {
          if(strcmp(currentLine1 , currentLine2)!=0 ){
            printf("============ #%lld ============\n%s" , currentLine , currentLine1 );
            if(currentLine1[strlen(currentLine1) - 1] != '\n') printf("\n");
            printf("%s", currentLine2);
            if(currentLine2[strlen(currentLine2) - 1] != '\n') printf("\n");
          }
          for (int i = 0; i < 1000000; i++) currentLine1[i]=0;
          for (int i = 0; i < 1000000; i++) currentLine2[i]=0;
          currentLine++;
          index =0;
          if(c1 != EOF) c1=0;
          if(c2 != EOF) c2 =0;
        }
      if(c1!= '\n' && c1!= EOF) c1 = fgetc(f1);
      if(c2!= '\n' && c2!= EOF)c2 = fgetc(f2);
      if(currentLine > countoflines1 && (c2 == '\n' || c2 == EOF)) {
        if(c1 == EOF && c2 == EOF) break;
          printf(">>>>>>>>>>>> #%lld - #%lld >>>>>>>>>>>>\n" , currentLine , countoflines2);
          printf("%s\n" , currentLine2);
          for (int i = 0; i < 1000000; i++) currentLine2[i]=0;
          int index2 =0;
          while(c2!= EOF){
            c2 = getc(f2);
            currentLine2[index2] = c2;
            index2++;
            if(c2 == '\n'||c2 == EOF){
              printf("%s" , currentLine2);
              for (int i = 0; i < 1000000; i++) currentLine2[i]=0;
              index2 =0;
            } 
          }
        if(c1 == EOF && c2 == EOF) break;
      }
      if(currentLine > countoflines2 && (c1 == '\n' || c1 == EOF)) {
        if(c1 == EOF && c2 == EOF) break;
          printf(">>>>>>>>>>>> #%lld - #%lld >>>>>>>>>>>>\n" , currentLine , countoflines1);
          printf("%s\n" , currentLine1);
          for (int i = 0; i < 1000000; i++) currentLine1[i]=0;
          int index2 =0;
          while(c1!= EOF){
            c1 = getc(f1);
            currentLine1[index2] = c1;
            index2++;
            if(c1 == '\n'||c1 == EOF){
              printf("%s" , currentLine1);
              for (int i = 0; i < 1000000; i++) currentLine1[i]=0;
              index2 =0;
            } 
          }
        if(c1 == EOF && c2 == EOF) break;
      }
    }  
    fclose(f1);
    fclose(f2);
}
char* makefullpath(char* line , int startIndex){
  char* fullpath = malloc(200);
  for (int i = 0; i < 200; i++)fullpath[i] = 0;
if(line[startIndex] == '\"'){
      startIndex+=1;
      if(line[startIndex] == '/') startIndex+=1;
      int baseIndex = startIndex;
      while(line[startIndex] != '\"'){
        fullpath[startIndex-baseIndex] = line[startIndex];
        startIndex++;
      }
    }
else {
    if(line[startIndex] == '/') startIndex+=1;
    int baseIndex = startIndex;
    while(line[startIndex] != ' '&& startIndex != strlen(line)){
    fullpath[startIndex-baseIndex] = line[startIndex];
    startIndex++;
    }
    }

  return fullpath;
}
void indent(char filename[]){
    FILE *f1 = fopen(filename, "r");
    // DWORD attributes = GetFileAttributes("indent.txt");
    // SetFileAttributes("indent.txt" , attributes + FILE_ATTRIBUTE_HIDDEN);
    FILE *f2 = fopen("indent.txt", "w");
    char c = fgetc(f1);
    int spacecount =0;
    int afteracooladspace =0;
    int state =0;
    int existcharbeforeacoolad =0;
    while (c != EOF)
    {
    if(c==' '){    
        spacecount++;   
        while(1){
        c = fgetc(f1);
        if(c == ' ') spacecount++;
        else break;
        }         
    if(c!= '{' && c!='}') {
          if(state ==1) for (int i = 0; i < spacecount; i++) fputc(' ', f2);
          fputc(c, f2);
          state =1;
          spacecount=0;
          existcharbeforeacoolad =1;
      }
    else if (c=='{') {
        if(existcharbeforeacoolad==1)fputc(' ', f2);
        fputc('{', f2);
        fputc('\n', f2);
        afteracooladspace +=4;
        for (int i = 0; i < afteracooladspace; i++) fputc(' ', f2);
        state =0;
        spacecount =0;
        existcharbeforeacoolad =0;
      }
    else {
     fputc('\n', f2);
      afteracooladspace -=4;
      for (int i = 0; i < afteracooladspace; i++) fputc(' ', f2);
      fputc('}', f2);
      while(1){
        c = fgetc(f1);
        if(c!= ' ') break;
      }  
      if(c == '}') afteracooladspace -=4;
        fputc('\n' , f2);
      for (int i = 0; i < afteracooladspace; i++) fputc(' ', f2);
        fputc(c , f2);
    }
      }
    else if(c == '{'){
       if(existcharbeforeacoolad==1) fputc(' ', f2);
      fputc('{', f2);
      fputc('\n', f2);
      afteracooladspace +=4;
      for (int i = 0; i < afteracooladspace; i++) fputc(' ', f2);
      state =0;
      existcharbeforeacoolad =0;
    }
    else if(c == '}'){
      fputc('\n', f2);
      afteracooladspace -=4;
      for (int i = 0; i < afteracooladspace; i++) fputc(' ', f2);
      fputc('}', f2);
      while(1){
        c = fgetc(f1);
        if(c!= ' ') break;
      }  
      if(c == '}') afteracooladspace -=4;
        fputc('\n' , f2);
      for (int i = 0; i < afteracooladspace; i++) fputc(' ', f2);
        fputc(c , f2);
    }
    else{
        fputc(c, f2);
        state =1;
        existcharbeforeacoolad =1;
        } 
    c = fgetc(f1);
    }  
    fclose(f1);
    fclose(f2);
}
void copycontent(char filename[]  , char filename2[]){
    FILE *f1 = fopen(filename, "r");
    FILE *f2 = fopen(filename2, "w");
    // DWORD attributes = GetFileAttributes("undo.txt");
    // SetFileAttributes("undo.txt" , attributes | FILE_ATTRIBUTE_HIDDEN);
    char c = fgetc(f1);
    while (c != EOF)
    {
        fputc(c, f2);
        c = fgetc(f1);
    }  
    fclose(f1);
    fclose(f2);
}
void checkText(char filename[],char pattern[] , int option , int isArman){
    FILE *file = fopen(filename, "r+");
    FILE *exportfirstcommand = fopen("exportfirstcommand.txt", "a+");
    char *currentLine = malloc(1000000);
    for (int i = 0; i < 1000000; i++)currentLine[i] = 0;
    int index = 0;
    int haspattern = 0;
    char c = getc(file);
    while (1)
    {
        if (c != EOF)currentLine[index] = c;
        index++;
        if (strstr(currentLine, pattern) != NULL && (c == '\n' || c == EOF) && option ==0)
        {     
        if (isArman == 1 ){
         fputs(filename, exportfirstcommand);
        fputs(": " , exportfirstcommand);
        }
        else  printf("%s: ", filename);
        if (c == '\n')
        isArman == 1 ? fputs(currentLine, exportfirstcommand) : printf("%s", currentLine);
        else{
        isArman == 1 ? fputs(strcat(currentLine, "\n"), exportfirstcommand) : printf("%s\n", currentLine);
        }
        if(c==EOF) break;
        }
        if ((c == '\n' || c == EOF)){
        if (strstr(currentLine, pattern) != NULL) haspattern=1;
        for (int i = 0; i < 1000000; i++) currentLine[i] = 0;
          if(c != EOF) c=0;
        index =0;
          if(c==EOF) break;
        }
        if (c != '\n' && c != EOF) c = fgetc(file);
        if(option ==1) {
          if (strstr(currentLine, pattern) != NULL)  truelines++;
        }
        if(option ==2){
            if (haspattern==0&&strstr(currentLine, pattern) != NULL) 
            isArman ==1 ? fputs(strcat(filename , "\n"), exportfirstcommand):printf("%s\n", filename);
        }
    }
    fclose(file);
    fclose(exportfirstcommand);
}
void copyText(char path[], int position , int targetline , int size , char direction){
  FILE * file= fopen(path, "r+");
  char* ch = malloc(10000 * sizeof(char));
  char** lines = malloc(10000 * sizeof(lines[0]));
  int lastlineindex=0;
  while (fgets(ch , 10000*sizeof(ch),file))
  {
    lines[lastlineindex] = malloc(10000 * sizeof(char));
    strcpy(lines[lastlineindex] , ch);
    lastlineindex++;
  }
  int countCopiedCharectes =0;
  char copiedString[size+1];
  for (int i = 0; i < size; i++) copiedString[i] = 0;
  copiedString[size] = '\0';
   int len = strlen(lines[targetline-1])-1;
  if(direction == 'f'){
  for (int i = position; i < 10000; i++)
  {
    copiedString[i-position]=lines[targetline-1][i];
    countCopiedCharectes++;
    if(i ==len||countCopiedCharectes == size) break;
  }
    strcpy(fullstring , strcat(fullstring,copiedString));
     file = fopen(path , "w");
     for (int i = 0; i < lastlineindex; i++)
     fputs(lines[i] , file);
     fclose(file);
     if(countCopiedCharectes < size){
        copyText(path , 0 , targetline+1 , size-countCopiedCharectes , 'f');
    }
  }
  else {
    int startIndex = position-size+1;
    if(startIndex <0) startIndex =0;
for (int i = startIndex; i <= position; i++)
  {
    copiedString[i-startIndex] =lines[targetline-1][i];
    countCopiedCharectes++;
    if(countCopiedCharectes == size) break;
  }
  strcpy(fullstring , strcat(copiedString,fullstring));
  file = fopen(path , "w");
  for (int i = 0; i < lastlineindex; i++)
    fputs(lines[i] , file);
    fclose(file);
     if(countCopiedCharectes < size){
    copyText(path , strlen(lines[targetline-2])-1 , targetline-1 , size-countCopiedCharectes , 'b');
    }
  } 
}
void removeText(char path[], int position , int targetline , int size , char direction){
  copycontent(path ,"undo.txt");
  FILE * file= fopen(path, "r+");
  FILE * removefile= fopen("removefile.txt", "w+");
  int countCopiedCharectes =0;
  int currentPosition =0 , currentLine = 1;
  if(direction == 'f'){
    char c = getc(file);
   while (c!= EOF){
    if(currentPosition == position && currentLine == targetline) {
      for (int i = 0; i < size; i++)
          c = getc(file);
    }
    currentPosition ++;
    putc(c , removefile);
    if(c == '\n') {
      currentLine ++;
      currentPosition =0;
    }
    c = getc(file);
  }
  fclose(removefile);
  copycontent("removefile.txt" , path);
  remove("removefile.txt");
  fclose(file);
  }
  else {
  char c = getc(file);
   while (c!= EOF){
    int condition = currentPosition + size;
    if(targetline > 1 && condition > countlinechars(path , targetline-1)){
      condition -= (countlinechars(path , targetline-1));
      currentLine ++;
      if(condition!= position)  currentLine --;
    }
    if(condition == position && currentLine == targetline) {
      for (int i = 0; i < size; i++)
          c = getc(file);
    }
    putc(c , removefile);
    currentPosition ++;
    if(c == '\n') {
      currentLine ++;
      currentPosition =0;
    }
    c = getc(file);
  }
  fclose(removefile);
  copycontent("removefile.txt" , path);
  remove("removefile.txt");
  fclose(file);
  } 
}
void showText(char path[]){
  FILE * file= fopen(path, "r+");
  char c = getc(file);
  while(c!= EOF){
  printf("%c" , c);
  c = getc(file);
  } 
  fclose(file);
}
void showArmanText(char path[]){
  FILE * file= fopen(path, "r+");
  FILE * exportfirstcommand = fopen("exportfirstcommand.txt", "w");
  char c = getc(file);
  while(c!= EOF){
  putc( c , exportfirstcommand);
  c = getc(file);
  } 

  fclose(file);
  fclose(exportfirstcommand);
}
void insertText(char path[] , char str[] , int position , int targetline){
  FILE * file= fopen(path, "r");
  copycontent(path ,"undo.txt");
  FILE * file2 = fopen("insert.txt" , "w");
  int currentPosition =0; 
  int currentLine = 1;
  char c = getc(file);
  while (1){
    if(currentPosition == position && currentLine == targetline) {
      for (int  i = 0; i < strlen(str); i++) putc(str[i] , file2);
    }
    putc(c , file2);
    currentPosition ++;
    if(c == '\n') {
      currentLine ++;
      currentPosition =0;
    }
    if(c == EOF) break;
    c = getc(file);
  }
  fclose(file2);
  copycontent("insert.txt" , path);
  remove("insert.txt");
  fclose(file);
}
void insertArmanText(char path[] , int position , int targetline){
  FILE * file= fopen(path, "r");
  copycontent(path ,"undo.txt");
  FILE * file2 = fopen("insert.txt" , "w");
  FILE * exportfirstcommand = fopen("exportfirstcommand.txt" , "r");
  int currentPosition =0; 
  int currentLine = 1;
  char c = getc(file);
  while (1){
    if(currentPosition == position && currentLine == targetline) {
      char d = getc(exportfirstcommand);
      while(d!= EOF){
        putc(d , file2);
        d = getc(exportfirstcommand);
      }
    }
    putc(c , file2);
    currentPosition ++;
    if(c == '\n') {
      currentLine ++;
      currentPosition =0;
    }
    if(c == EOF) break;
    c = getc(file);
  }
  fclose(file2);
  copycontent("insert.txt" , path);
  remove("insert.txt");
  fclose(file);
  fclose(exportfirstcommand);
  remove("exportfirstcommand.txt");
}
char* MakeDirectoriesPath(char* array){
    int i;
    for (i = strlen(array)-1; i >= 0; i--) if(array[i] == '/') break;
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
    if(access(path, F_OK)==0) {
        printf("File is already exist.");
        return;
    }
    file  = fopen(path, "w+");
    fclose(file);
}
void replaceText(char filename[],char pattern[] , char str[], int isArman, int at, int isall){
  FILE *file = fopen(filename, "r+");
  char* target  = malloc(200);
  for (int i = 0; i < 200; i++) target[i] = 0;
  int index =0;
  int pos =0;
  int istargetom = 1;
  int wordcount = 0;
  char c = getc(file);
  if(strstr(pattern , "*") == NULL)
  while(c!=EOF){
    if(c!='\n' && c!= '\0' && c!= EOF) target[index] = c;
    if(pattern[index] != target[index]) {
    for (int i = 0; i < 200; i++) target[i] = 0;
    index = 0;
    }
    if(strcmp(pattern , target) == 0){
      if (isall == 1) {
        removeText(filename ,  pos - strlen(pattern) + 1 , 1 , strlen(pattern) , 'f');
        insertText(filename , str , pos - strlen(pattern) + 1 , 1);
        pos -= (strlen(pattern) - strlen(str));
      }
      else if(istargetom == at) {
        removeText(filename ,  pos - strlen(pattern) + 1 , 1 , strlen(pattern) , 'f');
        insertText(filename , str , pos - strlen(pattern) + 1 , 1);
      }
      istargetom ++;
      for (int i = 0; i < 200; i++) target[i] = 0;
      index = 0;
    }
    if(c!='\n' && c!= '\0' && c!= EOF && strcmp(pattern , target) != 0&& pattern[index] == target[index])  index ++;
    pos++;
    c= getc(file);
  }
  else 
  while(1){
    if(c!='\n' && c!= '\0' && c!= ' ' && c!= EOF) target[index] = c;
    if((c == ' ' || c==EOF || c=='\n')&& strlen(target) > 0){
      wordcount++;
      if (checkWord(target , pattern) != 0){
      for (int i = 0; i < checkWord(target , pattern); i++)
      {
        if (pattern[strlen(pattern) -1] == '*'){
        int i;
        for (i = 0; i < strlen(target); i++)
        if(target[i] == deletelaststar(pattern)[0]) break;
        int result = pos - strlen(target) + i;
        if (isall == 1) {
          removeText(filename ,  result , 1 , strlen(target) - i , 'f');
          insertText(filename , str , result , 1);
          pos += (strlen(str) -strlen(target) + i) ;
        }
        else if(istargetom == at) {
          removeText(filename ,  result , 1 , strlen(target) - i , 'f');
          insertText(filename , str , result , 1);
        }
        }
        else{
        int i;
        for (i = strlen(target) -1; i >0 ; i--)
        if(target[i] == deletefirststar(pattern)[0]) break;
        int result = pos - strlen(target) + i;
        if (isall == 1) {
          removeText(filename ,  result - i , 1 , i+2 , 'f');
          insertText(filename , str , result - i , 1);
          pos+= (strlen(str) - i -2);
        }
        else if(istargetom == at){
          removeText(filename ,  result - i , 1 , i+2 , 'f');
          insertText(filename , str , result - i , 1);
        }
        }
        istargetom ++;
      }
      }
      for (int i = 0; i < 200; i++) target[i] = 0;
      index = 0;
      if(c == EOF) break;
    }
    if(c!='\n' && c!= '\0' && c!= ' ' && c!= EOF)  index ++;
    pos++;
    if(c == EOF) break;
    c= getc(file);
  }
  fclose(file);
}
void tree(char *basePath, const int root , int depth , int isArman)
{
    FILE *exportfirstcommand;
    if(isArman ==1) exportfirstcommand = fopen("exportfirstcommand.txt", "a+");
    int i;
    char newpath[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            if(root > 0){
            if(root >2){
            isArman ==1 ? fputc(179 , exportfirstcommand) : printf("%c", 179);
            for (i=0; i<root-2; i++) {
             isArman ==1 ? fputs(" " , exportfirstcommand) : printf("%s" , " ");
            }
            }          
            isArman ==1 ? fputc('|' , exportfirstcommand) : printf("%c", 195);
            isArman ==1 ? fputc('-' , exportfirstcommand) : printf("%c", 196);
            isArman ==1 ? fputc('-', exportfirstcommand) : printf("%c", 196);
            }
            isArman ==1 ? fputs(dp->d_name , exportfirstcommand) : printf("%s", dp->d_name);
            if(dp -> d_type ==16) isArman ==1 ? fputc(':' , exportfirstcommand) :printf(":");
            isArman ==1 ? fputs("\n" , exportfirstcommand) :printf("\n");
            strcpy(newpath, basePath);
            strcat(newpath, "/");
            strcat(newpath, dp->d_name);
            if(3*(depth) >= (root+4)) tree(newpath, root + 2 ,depth , isArman);
        }
    }
  if(isArman == 1) fclose(exportfirstcommand);
  closedir(dir);
}
void handleCreatefile(char line[]){
    char* fullpath = (char*) malloc(200);
    for (int i = 0; i < 200; i++) fullpath[i]=0;
    line = strstr(line , "--file");
    fullpath = makefullpath(line , 7);
    FILE *file;
    char* path = MakeDirectoriesPath(fullpath);
    makeNotExistDirectories(path);
    makeFile(file , fullpath);
}
void handleInsert(char line[]){
    char* fullpath = (char*) malloc(200);
    int i;
    char* str = (char*) malloc(200);
    for ( i = 0; i < strlen(line); i++)
        if(line[i] == '-') break;
    for (int i = 0; i < 200; i++) str[i]=0;
    for (int i = 0; i < 200; i++) fullpath[i]=0;
      int j;
        for ( j = 0; j < strlen(line); j++)
          if(line[j] == '-' , line[j+1] == 'p') break;
        char targetline[3] , position[3];
        int k ,x;
        for (k = j+5; k < strlen(line); k++)
        {
          if(line[k] == ':') break;
          targetline[k-j-5] = line[k];
        }
        for (x = k+1; x < strlen(line); x++)
        {
          if(line[x] == ' ') break;
          position[x-k-1] = line[x];
        }
    fullpath = makefullpath(line , i+7);
    line = strstr(line , "--str");
    str = makefullpath(line , 6);
    if(access(fullpath, F_OK!=0)) printf("File Not Found");
    insertText(fullpath , str , atoi(position) , atoi(targetline));
}
void handleCat(char line[] , int isArman){
    char* fullpath = malloc(200);
    int i;
    for ( i = 0; i < strlen(line); i++)
        if(line[i] == '-') break;
    for (int i = 0; i < 200; i++) fullpath[i]=0;
           fullpath = makefullpath(line , i+7);
 if(isArman = 1) showArmanText(fullpath);
 else showText(fullpath);
}
void handleRemove(char line[]){
    int i;
    char* fullpath = malloc(200);
    char str [200];
    for ( i = 0; i < strlen(line); i++)
        if(line[i] == '-') break;
    for (int i = 0; i < 200; i++) str[i]=0;
    for (int i = 0; i < 200; i++) fullpath[i]=0;
         int j;
        for ( j = 0; j < strlen(line); j++)
          if(line[j] == '-' , line[j+1] == 'p') break;
        char targetline[3] , position[3] , size[3];
        int k;
        for (k = j+5; k < strlen(line); k++)
        {
          if(line[k] == ':') break;
          targetline[k-j-5] = line[k];
        }
        int x;
        for (x = k+1; x < strlen(line); x++)
        {
          if(line[x] == ' ') break;
          position[x-k-1] = line[x];
        }
         for (j = x+7; j < strlen(line); j++)
        {
          if(line[j] == ' ') break;
          size[j-x-7] = line[j];
        }
        fullpath = makefullpath(line , i+7);
        removeText(fullpath , atoi(position) , atoi(targetline) , atoi(size) , line[j+2]);
}
void handleCopy(char line[]){
    int i , startIndex;
    char* fullpath = malloc(200);
    char targetline[3],position[3],size[3];
    for (i = 0; i < strlen(line); i++) if(line[i] == '-') break;
    for (int j = 0; j < 3; j++) targetline[j] = position[j] = size[j]=0;
    for (int j = 0; j < 200; j++) fullpath[j]=0;
    for (int j = 0; j < strlen(line); j++)
          if(line[j] == '-' && line[j+1] == 'p') {startIndex =j; break;}
    for (int k = startIndex+5; k < strlen(line); k++)
        {
          if(line[k] == ':') {startIndex = k;break;}
          targetline[k-startIndex-5] = line[k];
        }
        for (int x = startIndex+1; x < strlen(line); x++)
        {
          if(line[x] == ' ') {startIndex = x; break;}
          position[x-startIndex-1] = line[x];
        }
         for (int j = startIndex+7; j < strlen(line); j++)
        {
          if(line[j] == ' ') {startIndex =j; break;};
          size[j-startIndex-7] = line[j];
        }
        fullpath = makefullpath(line , i+7);
        fullstring = malloc(atoi(size) * sizeof(char));
        for (int i = 0; i < strlen(fullstring); i++) fullstring[i] =0;
        copyText(fullpath , atoi(position) , atoi(targetline) , atoi(size) , line[startIndex+2]);
    int len = strlen(fullstring)+1;
    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE , len);
    memccpy(GlobalLock(mem) , fullstring , len , len);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT , mem);
    CloseClipboard();
}
void handleCut(char line[]){
    int i , startIndex;
        char* fullpath = malloc(200);
        char targetline[3],position[3],size[3];
    for (i = 0; i < strlen(line); i++) if(line[i] == '-') break;
    for (int j = 0; j < 3; j++) targetline[j] = position[j] = size[j]=0;
    for (int j = 0; j < 200; j++) fullpath[j]=0;
    for (int j = 0; j < strlen(line); j++)
    if(line[j] == '-' && line[j+1] == 'p') {startIndex =j; break;}
    for (int k = startIndex+5; k < strlen(line); k++)
        {
          if(line[k] == ':') {startIndex = k;break;}
          targetline[k-startIndex-5] = line[k];
        }
        for (int x = startIndex+1; x < strlen(line); x++)
        {
          if(line[x] == ' ') {startIndex = x; break;}
          position[x-startIndex-1] = line[x];
        }
         for (int j = startIndex+7; j < strlen(line); j++)
        {
          if(line[j] == ' ') {startIndex =j; break;};
          size[j-startIndex-7] = line[j];
        }
        fullpath = makefullpath(line , i+7);
        fullstring = malloc(atoi(size) * sizeof(char));
        for (int i = 0; i < strlen(fullstring); i++) fullstring[i] =0;
        copyText(fullpath , atoi(position) , atoi(targetline) , atoi(size) , line[startIndex+2]);
        removeText(fullpath , atoi(position) , atoi(targetline) , atoi(size) , line[startIndex+2]);
    int len = strlen(fullstring)+1;
    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE , len);
    memccpy(GlobalLock(mem) , fullstring , len , len);
    OpenClipboard(0);
    EmptyClipboard();
    SetClipboardData(CF_TEXT , mem);
    CloseClipboard();
}
void handlePaste(char line[]){
    int i;
    char* fullpath = malloc(200);
    for ( i = 0; i < strlen(line); i++)
        if(line[i] == '-') break;
    for (int i = 0; i < 200; i++) fullpath[i]=0;
      int j;
        for ( j = 0; j < strlen(line); j++)
          if(line[j] == '-' , line[j+1] == 'p') break;
        char targetline[3] , position[3];
        int k ,x;
        for (k = j+5; k < strlen(line); k++)
        {
          if(line[k] == ':') break;
          targetline[k-j-5] = line[k];
        }
        for (x = k+1; x < strlen(line); x++)
        {
          if(line[x] == ' ') break;
          position[x-k-1] = line[x];
        }
        fullpath = makefullpath(line , i+7);
        printf("%s" , fullpath);
    OpenClipboard(0);
    HANDLE hData = GetClipboardData(CF_TEXT);
    char* str = (char *)hData;
    CloseClipboard();
    insertText(fullpath , str , atoi(position) , atoi(targetline));
}
void handleGrep(char line[] , int isArman){
   FILE *exportfirstcommand = fopen("exportfirstcommand.txt", "a+");
  char* pattern = malloc(100);
    for (int i = 0; i < 100; i++) pattern[i] =0;
  char* withoutoption = strstr(line , "--str");
  pattern = makefullpath(withoutoption , 6);
  char* files = strstr(line , "--files");
  int startIndex = 7;
  int baseIndex;
  int option =0;
  if(strstr(line , "-c") != NULL) option =1;
  if(strstr(line , "-l") != NULL) option =2;
  while (1)
  {
  char* filename = malloc(20);
  for (int i = 0; i < 20; i++) filename[i] =0;
  while (1)
  {
    if(files[startIndex+1] == '\"'){
      startIndex+=2;
      baseIndex = startIndex;
      while(files[startIndex] != '\"'){
        filename[startIndex-baseIndex] = files[startIndex];
        startIndex++;
      }
      int slashIndex;
      for (int i = 0; i < strlen(filename); i++)if(filename[i] == '/') slashIndex = i;
      for (int i = slashIndex; i < strlen(filename); i++)
        if(filename[i] == ' ') {
          filename[i]='/'; 
          break;
        }
      startIndex++;
    }
    else {
    startIndex++;
    baseIndex = startIndex;
    while(files[startIndex] != ' '&& startIndex != strlen(files)){
    filename[startIndex-baseIndex] = files[startIndex];
    startIndex++;
    }
    }
    break;
  }
  char * newfilename;
  if(filename[0] == '/') 
  newfilename = filename +1;
  else 
  newfilename = filename +0;
  checkText(newfilename , pattern,option , isArman);
  if(startIndex == strlen(files)) break;
  }
  if(option ==1) isArman==1 ? fputc(truelines + '0' , exportfirstcommand) :  printf("%d" , truelines);
  fclose(exportfirstcommand);
}
void handleUndo(char line[]){
    char* fullpath = (char*) malloc(200);
    for (int i = 0; i < 200; i++) fullpath[i]=0;
    line = strstr(line , "--file");
   fullpath = makefullpath(line , 7);
  copycontent("undo.txt",fullpath);
  remove("undo.txt");
}
void handleIndent(char line[]){
    char* fullpath = (char*) malloc(200);
    for (int i = 0; i < 200; i++) fullpath[i]=0;
    line = strstr(line , "--file");
    fullpath = makefullpath(line , 7);
    indent(fullpath);
    copycontent("indent.txt" , fullpath);
}
void handleCompare(char line[]){
    char* fullpath1 = (char*) malloc(200);
    for (int i = 0; i < 200; i++) fullpath1[i]=0;
    char* fullpath2 = (char*) malloc(200);
    for (int i = 0; i < 200; i++) fullpath2[i]=0;
    line = strstr(line , " ");
    fullpath1 = makefullpath(line , 1);
    int start = strlen(fullpath1) +2;
    if(line[1] == '/') start+=1;
    if(line[1] == '\"') start+=2;
    fullpath2 = makefullpath(line , start);
    compareText(fullpath1 , fullpath2);
}
void handleTree(char line[] , int isArman){
  int depth = atoi(strstr(line , " ") +1 );
  if(depth < -1) printf("invalid depth");
  else tree("." , 0 , depth , isArman);
}
void handleInsertArman(char line[]){
    char* fullpath = (char*) malloc(200);
    char targetline[3] , position[3];
    for (int i = 0; i < 200; i++) fullpath[i]=0;
    line = strstr(line , "--file");
    fullpath = makefullpath(line , 7);
    line = strstr(line , "--pos");
    for (int k = 0; k < 5; k++)
        {
          if(line[k+6] == ':') break;
          targetline[k] = line[k+6];
        }
    line = strstr(line , ":");
    for (int k = 0; k < strlen(line); k++) position[k] = line[k+1];
    if(access(fullpath, F_OK!=0)) printf("File Not Found");
    insertArmanText(fullpath, atoi(position) , atoi(targetline));
}
void checkArmanHandles(char line[] , char copyline []){
    char space[2] = " ";  
    if(strcmp(strtok(copyline , space),"insertstr")==0)handleInsertArman(line);
    if(strcmp(strtok(copyline , space),"tree")==0) handleTree(line , 1); 
    if(strcmp(strtok(copyline , space),"cat")==0) handleCat(line , 1);
    if(strcmp(strtok(copyline , space),"grep")==0) handleGrep(line  , 1);
}
void handleArman(char line []){
  char* firstcommand = malloc(200);
  char* secondcommand= malloc(200);
  char copyfirstcommand[200];
  char copysecondcommand[200];
  for (int i = 0; i < 200; i++) firstcommand[i] = secondcommand[i] = copyfirstcommand[i] = copysecondcommand[i] = 0;
  secondcommand = strstr(line , "=D");
  firstcommand = strtok(line , "=D");
  strcpy(copyfirstcommand , firstcommand);
  strcpy(copysecondcommand , secondcommand +3 );
  checkArmanHandles(firstcommand,copyfirstcommand);
  checkArmanHandles(secondcommand+3,copysecondcommand);
//first cat ,find , grep , compare , tree ,  copy , cut , auto-indent
//second insert , find , grep 
}
void handleFind(char line [] , int isArman){
    char* fullpath = (char*) malloc(200);
    char* str = (char*) malloc(200);
    for (int i = 0; i < 200; i++) fullpath[i] = str[i]=0;
    int at = 1 , iscount = 0 , isall =0 , isbyword=0;
    if(strstr(line , "-at") != NULL) at =  strstr(line , "-at")[4] - '0';
    if(strstr(line , "-count") != NULL) iscount = 1;
    if(strstr(line , "-all") != NULL) isall = 1;
    if(strstr(line , "-byword") != NULL) isbyword = 1;
    line = strstr(line , "--str");
    str = makefullpath(line , 6);
    line = strstr(line , "--file");
    fullpath = makefullpath(line , 7);
    findText(fullpath , str , 0,iscount,at,isbyword,isall);
}
void handleReplace(char line [] , int isArman){
    char* fullpath = (char*) malloc(200);
    char* str1 = (char*) malloc(200);
    char* str2 = (char*) malloc(200);
    for (int i = 0; i < 200; i++) fullpath[i] = str1[i] = str2[i]=0;
    int at = 1 ,isall =0;
    if(strstr(line , "-at") != NULL) at =  strstr(line , "-at")[4] - '0';
    if(strstr(line , "-all") != NULL) isall = 1;
    line = strstr(line , "--str1");
    str1 = makefullpath(line , 7);
    line = strstr(line , "--str2");
    str2 = makefullpath(line , 7);
    line = strstr(line , "--file");
    fullpath = makefullpath(line , 7);
    replaceText(fullpath , str1 , str2 ,0,at,isall);
}
int main (){
    char line[1000];
    char copyline[1000];
    gets(line);
    strcpy(copyline , line);
    if(strstr(line , "=D") == NULL){
        char space[2] = " ";  
    if(strcmp(strtok(copyline , space),"createfile")==0) handleCreatefile(line);
    if(strcmp(strtok(copyline , space),"insertstr")==0) handleInsert(line);
    if(strcmp(strtok(copyline , space),"cat")==0) handleCat(line, 0);
    if(strcmp(strtok(copyline , space),"removestr")==0) handleRemove(line);
    if(strcmp(strtok(copyline , space),"copystr")==0) handleCopy(line);
    if(strcmp(strtok(copyline , space),"cutstr")==0) handleCut(line);
    if(strcmp(strtok(copyline , space),"pastestr")==0) handlePaste(line);
    if(strcmp(strtok(copyline , space),"grep")==0) handleGrep(line , 0);
    if(strcmp(strtok(copyline , space),"undo")==0) handleUndo(line); 
    if(strcmp(strtok(copyline , space),"auto-indent")==0) handleIndent(line); 
    if(strcmp(strtok(copyline , space),"compare")==0) handleCompare(line); 
    if(strcmp(strtok(copyline , space),"tree")==0) handleTree(line , 0); 
    if(strcmp(strtok(copyline , space),"find")==0) handleFind(line , 0); 
    if(strcmp(strtok(copyline , space),"replace")==0) handleReplace(line , 0); 
    }
    else  handleArman(line); 
    return 0;
}
