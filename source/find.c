#include "../include/main.h"

//readDir
//tmp : directory 정보, dirName : 검색할 directory, option : 검색 옵션.
int readDir(DirectoryTree *dirTree, char *tmp, char *dirName, int option) {
    char *str;
    char str2[MAX_NAME];

    //directory name copy , str = tmp. str2 = copy str
    str = strtok(tmp, " ");
    strcpy(str2, str);
    for (int i = 0; i < 10; i++) str = strtok(NULL, " ");
    if (str) {
        //option == 0,
        if (option == 0) {
            if (strstr(str2, dirName)) {
                str[strlen(str) - 1] = '\0';
                if (!strcmp(str, "/")) printf("/%s\n", str2);
                else printf("%s/%s\n", str, str2);
            }
        //option == 1            
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

//디렉토리 트리에서 파일 검색
void findDir(DirectoryTree *dirTree, char *dirName, int option) {
    char tmp[MAX_LENGTH];

    //디렉토리 정보를 저장한 파일을 읽고, 각 줄에 대해 검색.
    Dir = fopen("file/Directory.txt", "r");
    while (fgets(tmp, MAX_LENGTH, Dir)) readDir(dirTree, tmp, dirName, option);
    fclose(Dir);
}


int ft_find(DirectoryTree *dirTree, char *command) {
    char *str;
    if (!command) {
        //검색할 fileName이 주어지지 않은 경우, current_directory에서 검색.
        findDir(dirTree, dirTree->current->name, 1);
        return SUCCESS;
    } else if(command[0] == '-'){
        // -name : fileName에 따라 검색.
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
    } else {
        findDir(dirTree, command, 0);
        findDir(dirTree, command, 1);
        }
    return 0;
}