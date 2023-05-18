#include "../include/main.h"

void ft_grep(char *search, char *fname, int Option) {
    char outputLine[MAX_LENGTH];
    int line = 1;
    FILE *fp = fopen(fname, "rt");

    if (!fp) return;
    while (TRUE) {
        if (feof(fp)) break;
        else fgets(outputLine, sizeof(outputLine), fp);
        line++;
    }
    fclose(fp);
    fp = fopen(fname, "rt");
    for (int j = 1; j < line - 1; j++) {
        fgets(outputLine, sizeof(outputLine), fp);
        switch (Option) {
            case 0:
                if (strstr(outputLine, search)) printf("%s", outputLine);
                break;
            case 1:
                if (strstr(outputLine, search)) printf("%d:%s", j, outputLine);
                break;
            case 2:
                if (!strstr(outputLine, search)) printf("%s", outputLine);
                break;
            case 3:
                if (strcasestr(outputLine, search)) printf("%s", outputLine);
                break;
            case 4:
                if (!strstr(outputLine, search)) printf("%d:%s", j, outputLine);
                break;
            case 5:
                if (!strcasestr(outputLine, search)) printf("%s", outputLine);
                break;
            case 6:
                if (strcasestr(outputLine, search)) printf("%d:%s", j, outputLine);
            default:
                break;
        }
    }
}