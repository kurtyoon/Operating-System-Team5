#include "../include/main.h"

void Instruction(DirectoryTree *dirTree, char *command) {
    char *str;
    char *str1;
    char *str2;
    int value;

    if (!strcmp(command, "") || command[0] == ' ') return;
    str = strtok(command, " ");
    if (!strcmp(str, "mkdir")) {
        str = strtok(NULL, " ");
        value = mkdir(dirTree, str);
        if (!value) SaveDir(dirTree, dStack);
    } else if (!strcmp(str, "cp")) {
        str = strtok(NULL, " ");
        str1 = strtok(NULL, " ");
        value = cp(dirTree, str, str1);
        if (!value) SaveDir(dirTree, dStack);
    } else if (!strcmp(str, "rm")) {
        str = strtok(NULL, " ");
        value = rm(dirTree, str);
        if (!value) SaveDir(dirTree, dStack);
    } else if (!strcmp(str, "cd")) {
        str = strtok(NULL, " ");
        cd(dirTree, str);
    } else if(!strcmp(str, "pwd")) {
        str = strtok(NULL, " ");
        pwd(dirTree, dStack, str);
    } else if (!strcmp(str, "ls")) {
        str = strtok(NULL, " ");
        ls(dirTree, str);
    } else if (!strcmp(str, "cat")) {
        str = strtok(NULL, " ");
        value = ft_cat(dirTree, str);
        if (!value) SaveDir(dirTree, dStack);
    } else if(!strcmp(str, "chmod")) {
        str = strtok(NULL, " ");
        value = chmod(dirTree, str);
        if (!value) SaveDir(dirTree, dStack);
    } else if(!strcmp(str, "chown")) {
        str = strtok(NULL, " ");
        value = chown_(dirTree, str);
        if (!value) SaveDir(dirTree, dStack);
    } else if (!strcmp(str, "find")) {
        str = strtok(NULL, " ");
        find_(dirTree, str);
    } else if (!strcmp(str, "grep")) {
        str = strtok(NULL, " ");
        str1 = strtok(NULL, " ");
        str2 = strtok(NULL, " ");
        if (!strcmp(str, "-n")) grep_a(str1, str2);
        else grep(str, str1);
    } else if(!strcmp(command, "exit")) {
        printf("logout\n");
        exit(0);
    } else printf("'%s': 명령을 찾을 수 없습니다\n", command);
    return;
}
