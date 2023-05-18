#include "main.h"

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