#include "../include/main.h"

void grep(char *search, char *fName) {
    int i = 1;
    char output_line[MAX_LENGTH];
    FILE *fp = fopen(fName, "rt");
    if (!fp) printf("파일 읽기 불가!\n");
    while (TRUE) {
        if (feof(fp)) break;
        else fgets(output_line, sizeof(output_line), fp);
        i++;
    }
    FILE *fp2 = NULL;
    fp2 = fopen(fName, "rt");
    for (int j = 1; j < i - 1; j++) {
        fgets(output_line, sizeof(output_line), fp2);
        if (strstr(output_line, search)) printf("%s", output_line);
    }
    fclose(fp);
}

void grep_a(char *searching_word, char *f_name) {
    int i = 1;
    char output_line[MAX_LENGTH];
    FILE *fp = fopen(f_name, "rt");
    if (!fp) printf("파일 읽기 불가!\n");
    while (TRUE) {
        if (feof(fp)) break;
        else fgets(output_line, sizeof(output_line), fp);
        i++;
    }
    FILE *fp2 = NULL;
    fp2 = fopen(f_name, "rt");
    for (int j = 1; j < i - 1; j++) {
        fgets(output_line, sizeof(output_line), fp2);
        if (strstr(output_line, searching_word)) printf("%d:%s", j, output_line);
    }
    fclose(fp);
}
