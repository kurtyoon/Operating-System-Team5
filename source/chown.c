#include "../include/main.h"

int changeOwner(DirectoryTree *dirTree, char *userName, char *dirName, int flag) {
    DirectoryNode *dirNode = NULL;
    DirectoryNode *fileNode = NULL;
    UserNode *tmpUser = NULL;

    dirNode = dirExistence(dirTree, dirName, 'd');
    fileNode = dirExistence(dirTree, dirName, 'f');

    if (dirNode) {
        if (checkPermission(dirNode, 'w')) {
            printf("chown: Can not modify file '%s': Permission denied\n", dirName);
            return FAIL;
        }
        tmpUser = userExistence(usrList, userName);
        if (tmpUser) {
            if (!flag) dirNode->id.UID = tmpUser->id.UID;
            else dirNode->id.GID = tmpUser->id.GID;
        } else {
            printf("chown: Invalid user: '%s'\n", userName);
            printf("Try 'chown --help' for more information.\n");
            return FAIL;
        }
    } else if (fileNode) {
        if (checkPermission(fileNode, 'w')) {
            printf("chown: Can not modify file '%s': Permission denied\n", dirName);
            return FAIL;
        }
        tmpUser = userExistence(usrList, userName);
        if (tmpUser) {
            if (!flag) fileNode->id.UID = tmpUser->id.UID;
            else fileNode->id.GID = tmpUser->id.GID;
        } else {
            printf("chown: Invalid user: '%s'\n", userName);
            printf("Try 'chown --help' for more information.\n");
            return FAIL;
        }
    } else {
        printf("chown: cannot access '%s': No such file or directory\n", dirName);
        return FAIL;
    }
    return SUCCESS;
}

int ft_chown(DirectoryTree* dirTree, char* command) {
    DirectoryNode* tmpNode = NULL;
    UserNode* tmpUser = NULL;
    char* str;
    char tmp[MAX_NAME];

    if (!command) {
        printf("chown: Invalid option\n");
        printf("Try 'chown --help' for more information.\n");
        return -1;
    }
    if (command[0] == '-') {
        if(!strcmp(command, "--help")) {
            printf("Usage: chown [OPTION]... [OWNER] FILE...\n");
            printf("Change the owner of each FILE to OWNER.\n\n");
            printf("  Options:\n");
            printf("  -R, --recursive        operate on files and directories recursively\n");
            printf("      --help     display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("chown: Invalid option\n");
                printf("Try 'chown --help' for more information.\n");
                return FAIL;
            } else {
                printf("chown: Unrecognized option -- '%s'\n", str);
                printf("Try 'chown --help' for more information.\n");
                return FAIL;
            }
        }
    } else {
        strncpy(tmp, command, MAX_NAME);
        str = strtok(NULL, " ");
        if (!str) {
            printf("chown: Invalid option\n");
            printf("Try 'chown --help' for more information.\n");
            return FAIL;
        } else {
            if (!strstr(tmp, ":")) changeOwner(dirTree, tmp, str, 0);
            else {
                char tmp2[MAX_NAME];
                strncpy(tmp2, tmp, MAX_NAME);
                char *str2 = strtok(tmp, ":");
                if (str2 && strcmp(tmp, tmp2)) {
                    changeOwner(dirTree, str2, str, 0);
                    str2 = strtok(NULL, " ");
                    if (str2) changeOwner(dirTree, str2, str, 1);
                }
                else if (str2 && !strcmp(tmp, tmp2)) changeOwner(dirTree, str2, str, 1);
            }
        }
    }
    return SUCCESS;
}