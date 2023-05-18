#include "../include/main.h"

void ft_grep(char *search, char *fname, int Option) {
    int line = 1;
    char output_line[MAX_LENGTH];
    FILE *fp = fopen(fname, "rt");

    if (!fp) return;
    while (TRUE) {
        if (feof(fp)) break;
        else fgets(output_line, sizeof(output_line), fp);
        line++;
    }
    fclose(fp);
    fp = fopen(fname, "rt");
    for (int j = 1; j < line - 1; j++) {
        fgets(output_line, sizeof(output_line), fp);
        if (strstr(output_line, search) && Option) {
            printf("%d:%s", j, output_line);
        } else if (strstr(output_line, search)) {
            printf("%s", output_line);
        }
    }
    fclose(fp);
}

// grep
void grep(char *searching_word,char *f_name){
    int i=1;
    char output_line[MAX_LENGTH];
    FILE *fp = fopen(f_name,"rt");
    if(fp==NULL)
        printf("cannot read the file\n");
    while(1){
        if(feof(fp))
            break;
        else
            fgets(output_line,sizeof(output_line),fp);//파일 불러오기
        i++;
    }
    FILE *fp2=NULL;
    fp2=fopen(f_name,"rt");
    for(int j=1;j<i-1;j++){//줄 개수만큼 for문 돌리기
        fgets(output_line,sizeof(output_line),fp2);
        if(strstr(output_line,searching_word)!=NULL)
            printf("%s",output_line);
    }
    fclose(fp);
}

void grep_n(char *searching_word,char *f_name){
    int i=1;
    char output_line[MAX_LENGTH];
    FILE *fp = fopen(f_name,"rt");
    if(fp==NULL)
        printf("cannot read the file\n");
    while(1){
        if(feof(fp))
            break;
        else
            fgets(output_line,sizeof(output_line),fp);
        i++;
    }
    FILE *fp2=NULL;
    fp2=fopen(f_name,"rt");
    for(int j=1;j<i-1;j++){//j가 줄 번호
        fgets(output_line,sizeof(output_line),fp2);
        if(strstr(output_line,searching_word)!=NULL)
            printf("%d:%s",j,output_line);
    }
    fclose(fp);
}

void grep_i(char *searching_word, char *f_name) {
    char output_line[MAX_LENGTH];
    FILE *fp = fopen(f_name, "rt");
    if(fp == NULL) {
        printf("cannot read the file\n");
        return;
    }
    while(fgets(output_line, sizeof(output_line), fp) != NULL) {
        if(strcasestr(output_line, searching_word) != NULL)//대소문자 구별 없이 pattern찾는 함수, pattern이 있을때, 찾았을때 출력
            printf("%s", output_line);
    }
    fclose(fp);
}

void grep_v(char *searching_word, char *f_name) {
    char output_line[MAX_LENGTH];
    FILE *fp = fopen(f_name, "rt");
    if(fp == NULL) {
        printf("cannot read the file\n");
        return;
    }
    while(fgets(output_line, sizeof(output_line), fp) != NULL) {
        if(strstr(output_line, searching_word) == NULL)//pattern을 못찾았을때
            printf("%s", output_line);
    }
    fclose(fp);
}

void grep_i_v(char *searching_word, char *f_name) {
    char output_line[MAX_LENGTH];
    FILE *fp = fopen(f_name, "rt");
    if(fp == NULL) {
        printf("cannot read the file\n");
        return;
    }
    while(fgets(output_line, sizeof(output_line), fp) != NULL) {
        if(strcasestr(output_line, searching_word) == NULL)
            printf("%s", output_line);
    }
    fclose(fp);
}