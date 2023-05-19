#include "../include/main.h"

//chmod(Change_Mode) : change the permission of File and directory.

int changeMode(DirectoryTree *dirTree, int mode, char *dirName) {
    DirectoryNode *fileNode = NULL;
    DirectoryNode *dirNode = NULL;
    
    fileNode = dirExistence(dirTree, dirName, 'd');
    dirNode = dirExistence(dirTree, dirName, 'f');

    if (fileNode) {
        if (checkPermission(fileNode, 'w')) {
            printf("chmod: Can not modify file '%s': Permission denied\n", dirName);
            return FAIL;
        }
        fileNode->permission.mode = mode;
        modeToPermission(fileNode);
    } else if (dirNode) {
        if (checkPermission(dirNode, 'w')) {
            printf("chmod: Can not modify file '%s': Permission denied\n", dirName);
            return FAIL;
        }
        dirNode->permission.mode = mode;
        modeToPermission(dirNode);
    } else {
        printf("chmod: Can not access to '%s: There is no such file or directory\n", dirName);
        return FAIL;
    }
    return SUCCESS;
}

void changeModeAll(DirectoryNode *dirNode, int mode) {
    if (dirNode->nextSibling) {
        changeModeAll(dirNode->nextSibling, mode);
    }
    if (dirNode->firstChild) {
        changeModeAll(dirNode->firstChild, mode);
    }
    dirNode->permission.mode = mode;
    modeToPermission(dirNode);
}

int ft_chmod(DirectoryTree* dirTree, char* command) {
    DirectoryNode* tmpNode = NULL;
    ThreadTree threadTree[MAX_THREAD];
    pthread_t threadPool[MAX_THREAD];
    int threadCount = 0;
    char* str;
    int tmp;

    if (!command) {
        printf("chmod: Invalid option\n");
        printf("Try 'chmod --help' for more information.\n");
        return FAIL;
    }
    if(command[0] == '-') {
        if (!strcmp(command, "--help")) {
            printf("Usage: chmod [OPTION]... OCTAL-MODE FILE...\n");
            printf("Change the mode of each FILE to MODE.\n\n");
            printf("  Options:\n");
            printf("  -R, --recursive        change files and directories recursively\n");
            printf("      --help\t Display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("chmod: Invalid option\n");
                printf("Try 'chmod --help' for more information.\n");
                return FAIL;
            } else {
                printf("chmod: Unrecognized option -- '%s'\n", str);
                printf("Try 'chmod --help' for more information.\n");
                return FAIL;
            }
        }
    } else {
        if (command[0] - '0' < 8 && command[1] - '0' < 8 && command[2] - '0' < 8 && strlen(command) == 3) {
            tmp = atoi(command);
            str = strtok(NULL, " ");
            if (!str) {
                printf("chmod: Invalid option\n");
                printf("Try 'chmod --help' for more information.\n");
                return FAIL;
            }
            threadTree[threadCount].threadTree = dirTree;
            threadTree[threadCount].command = str;
            threadTree[threadCount++].mode = tmp;
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].command = str;
                threadTree[threadCount++].mode = tmp;
                str = strtok(NULL, " ");
            }
        } else {
            printf("chmod: Invalid Mode: '%s'\n", command);
            printf("Try 'chmod --help' for more information.\n");
            return FAIL;
        }
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, chmodUsedThread, (void*)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return SUCCESS;
}