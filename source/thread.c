#include "../include/main.h"

void *thread_routine_make_directory(void *arg) {
    ThreadTree *threadTree = ((ThreadTree *)arg);
    DirectoryTree *dirTree = threadTree->threadTree;
    DirectoryTree *p_preTree;
    char *command = threadTree->command;

    DirectoryNode *tmpNode = dirTree->current;
    char tmp[MAX_DIR];
    char pStr[MAX_DIR];
    char tmpStr[MAX_DIR];
    char directoryName[MAX_DIR];
    int directoryNameLength = 0;
    int isDirectoryExist;

    strncpy(tmp, command, MAX_DIR);

    if (strstr(command, "/") == NULL) {
        makeDir(dirTree, command, 'd');
    } else if (threadTree->option == 1) {
        int tmpLen = strlen(tmp), i = 0;
        if (tmp[0] == '/') {
            dirTree->current = dirTree->root;
            i = 1;
        }
        if (tmp[tmpLen - 1] == '/') {
            tmpLen -= 1;
        }
        for (; i < tmpLen; i++) {
            pStr[i] = tmp[i];
            pStr[i + 1] = 0;
            directoryName[directoryNameLength++] = tmp[i];
            if (tmp[i] == '/') {
                directoryName[--directoryNameLength] = 0;
                strncpy(tmpStr, pStr, i - 1);
                isDirectoryExist = moveCurrent(dirTree, directoryName);
                if (isDirectoryExist == -1) {
                    makeDir(dirTree, directoryName, 'd');
                    isDirectoryExist = moveCurrent(dirTree, directoryName);
                }
                directoryNameLength = 0;
            }
        }
        directoryName[directoryNameLength] = 0;
        makeDir(dirTree, directoryName, 'd');
        dirTree->current = tmpNode;
    } else {
        char *p_get_directory = getDir(command);
        isDirectoryExist = movePath(dirTree, p_get_directory);
        if (isDirectoryExist != 0) {
            printf("mkdir: '%s': No such file or directory.\n", p_get_directory);
        } else {
            char *str = strtok(tmp, "/");
            char *p_directory_name;
            while (str != NULL) {
                p_directory_name = str;
                str = strtok(NULL, "/");
            }
            makeDir(dirTree, p_directory_name, 'd');
            dirTree->current = tmpNode;
        }
    }
    pthread_exit(NULL);
}