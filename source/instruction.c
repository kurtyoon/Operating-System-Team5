#include "../include/main.h"

void instruction(DirectoryTree *dirTree, char *command) {
    char *str;
    char *str1;
    char *str2;
    int value;

    if (!strcmp(command, "") || command[0] == ' ') return;
    str = strtok(command, " ");
    if (!strcmp(str, "clear")) system("clear");
    else if (!strcmp(str, "mkdir")) {
        str = strtok(NULL, " ");
        value = ft_mkdir(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if (!strcmp(str, "touch")) {
        str = strtok(NULL, " ");
        value = ft_touch(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if (!strcmp(str, "cp")) {
        str = strtok(NULL, " ");
        str1 = strtok(NULL, " ");
        value = ft_cp(dirTree, str, str1);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if (!strcmp(str, "rm")) {
        str = strtok(NULL, " ");
        value = ft_rm(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if (!strcmp(str, "cd")) {
        str = strtok(NULL, " ");
        ft_cd(dirTree, str);
    } else if(!strcmp(str, "pwd")) {
        str = strtok(NULL, " ");
        ft_pwd(dirTree, dStack, str);
    } else if (!strcmp(str, "ls")) {
        str = strtok(NULL, " ");
        ft_ls(dirTree, str);
    } else if (!strcmp(str, "cat")) {
        str = strtok(NULL, " ");
        value = ft_cat(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if(!strcmp(str, "chmod")) {
        str = strtok(NULL, " ");
        value = ft_chmod(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if(!strcmp(str, "chown")) {
        str = strtok(NULL, " ");
        value = ft_chown(dirTree, str);
        if (!value) saveDirectoryTree(dirTree, dStack);
    } else if (!strcmp(str, "find")) {
        str = strtok(NULL, " ");
        ft_find(dirTree, str);
    } else if (!strcmp(str, "grep")) {
        str = strtok(NULL, " ");
        str1 = strtok(NULL, " ");
        str2 = strtok(NULL, " ");
        if (!strcmp(str, "-n")) ft_grep(str1, str2, 1);
        else if (!strcmp(str, "-v")) ft_grep(str1, str2, 2);
        else if (!strcmp(str, "-i")) ft_grep(str1, str2, 3);
        else if (!strcmp(str, "-nv") || !strcmp(str, "-vn")) ft_grep(str1, str2, 4);
        else if (!strcmp(str, "-vi") || !strcmp(str, "-iv")) ft_grep(str1, str2, 5);
        else if (!strcmp(str, "-in") || !strcmp(str, "-ni")) ft_grep(str1, str2, 6);
        else if (!strcmp(str, "-nvi") || !strcmp(str, "-niv") || !strcmp(str, "-inv") \
         || !strcmp(str, "-ivn") || !strcmp(str, "-vni") || !strcmp(str, "-vin")) {
            ft_grep(str1, str2, 7);
         } else ft_grep(str, str1, 0);
    } else if(!strcmp(command, "exit")) {
        printf("logout\n");
        exit(0);
    } else printf("Command '%s' not found\n", command);
    return;
}
