#include "../include/main.h"

int changeMode(DirectoryTree *dirTree, int mode, char *dirName) {
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;

    tmpNode = dirExistence(dirTree, dirName, 'd');
    tmpNode2 = dirExistence(dirTree, dirName, 'f');

    if (tmpNode) {
        if (checkPermission(tmpNode, 'w')) {
            printf("chmod: Can not modify file '%s': Permission denied\n", dirName);
            return FAIL;
        }
        tmpNode->permission.mode = mode;
        modeToPermission(tmpNode);
    } else if (tmpNode2) {
        if (checkPermission(tmpNode2, 'w')) {
            printf("chmod: Can not modify file '%s': Permission denied\n", dirName);
            return FAIL;
        }
        tmpNode2->permission.mode = mode;
        modeToPermission(tmpNode2);
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
    char* str;
    int tmp;

    if (!command) {
        printf("chmod: Invalid option\n");
        printf("Try 'chmod --help' for more information.\n");
        return FAIL;
    }
    if(command[0] == '-') {
        if (!strcmp(command, "-R")) {
            str = strtok(NULL, " ");
            if(!str) {
                printf("chmod: Invalid option\n");
                printf("Try 'chmod --help' for more information.\n");
                return FAIL;
            }
            if (str[0] - '0' < 8 && str[1] - '0' < 8 && str[2] - '0' < 8 && strlen(str) == 3) {
                tmp = atoi(str);
                str = strtok(NULL, " ");
                if (!str) {
                    printf("chmod: Invalid option\n");
                    printf("Try 'chmod --help' for more information.\n");
                    return FAIL;
                }
                tmpNode = dirExistence(dirTree, str, 'd');
                if (tmpNode) {
                    if (!tmpNode->firstChild) changeMode(dirTree, tmp, str);
                    else{
                        changeMode(dirTree, tmp, str);
                        changeModeAll(tmpNode->firstChild, tmp);
                    }
                } else {
                    printf("chmod: '%s': No such file or directory.\n", str);
                    return FAIL;
                }
            } else {
                printf("chmod: Invalid Mode: '%s'\n", str);
                printf("Try 'chmod --help' for more information.\n");
                return FAIL;
            }
        } else if (!strcmp(command, "--help")) {
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
        if (command[0] - '0' < 8 && command[1] - '0' < 8 && command[2] - '0' <8  && strlen(command) == 3) {
            tmp = atoi(command);
            str = strtok(NULL, " ");
            if (!str) {
                printf("chmod: Invalid option\n");
                printf("Try 'chmod --help' for more information.\n");
                return FAIL;
            }
            changeMode(dirTree, tmp, str);
        } else {
            printf("chmod: Invalid Mode: '%s'\n", command);
            printf("Try 'chmod --help' for more information.\n");
            return FAIL;
        }
    }
    return SUCCESS;
}