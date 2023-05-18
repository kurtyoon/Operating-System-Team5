#include "../include/main.h"

int readDir(DirectoryTree *dirTree, char *tmp, char *dirName, int option) {
    char *str;
    char str2[MAX_NAME];

    str = strtok(tmp, " ");
    strcpy(str2, str);
    for (int i = 0; i < 10; i++) str = strtok(NULL, " ");
    if (str) {
        if (option == 0) {
            if (strstr(str2, dirName)) {
                str[strlen(str) - 1] = '\0';
                if (!strcmp(str, "/")) printf("/%s\n", str2);
                else printf("%s/%s\n", str, str2);
            }
        } else {
            if (strstr(str, dirName)) {
                str[strlen(str) - 1] = '\0';
                if (!strcmp(str, "/")) printf("/%s\n", str2);
                else printf("%s/%s\n", str, str2);
            }
        }
    }
    return SUCCESS;
}

void findDir(DirectoryTree *dirTree, char *dirName, int option) {
    char tmp[MAX_LENGTH];

    Dir = fopen("file/Directory.txt", "r");
    while (fgets(tmp, MAX_LENGTH, Dir)) readDir(dirTree, tmp, dirName, option);
    fclose(Dir);
}

int ft_find(DirectoryTree *dirTree, char *command) {
    char *str;
    if (!command) {
        // 현재 디렉토리의 모든 파일 및 디렉토리 출력
        findDir(dirTree, dirTree->current->name, 1);
        return SUCCESS;
    } else if(command[0] == '-'){
        if (!strcmp(command, "-name")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("find: missing argument to `-name'\n");
                return FAIL;
            }
            findDir(dirTree, str, 0);
        } else if (!strcmp(command, "--help")) {
            printf("Usage: find  [path...] [expression]\n\n");
            printf("  Options:\n");
            printf("      -name\t finds file by name\n");
            printf("      --help     display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("find: ‘-’: No such file or directory\n");
                return FAIL;
            } else {
                printf("find: unknown predicate -- '%s'\n", str);
                printf("Try 'find --help' for more information.\n");
                return FAIL;
            }
        }
    } else findDir(dirTree, command, 1);
    return 0;
}