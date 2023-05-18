#include "../include/main.h"

int changeOwner(DirectoryTree *dirTree, char *userName, char *dirName) {
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    UserNode *tmpUser = NULL;

    tmpNode = IsExistDir(dirTree, dirName, 'd');
    tmpNode2 = IsExistDir(dirTree, dirName, 'f');


    if (tmpNode) {
        if (checkPermission(tmpNode, 'w')) {
            printf("chown: Can not modify file '%s': Permission denied\n", dirName);
            return FAIL;
        }
        tmpUser = IsExistUser(usrList, userName);
        if (tmpUser) {
            tmpNode->id.UID = tmpUser->id.UID;
            tmpNode->id.GID = tmpUser->id.GID;
        } else {
            printf("chown: Invalid user: '%s'\n", userName);
            printf("Try 'chown --help' for more information.\n");
            return FAIL;
        }
    } else if (tmpNode2) {
        if (checkPermission(tmpNode2, 'w')) {
            printf("chown: Can not modify file '%s': Permission denied\n", dirName);
            return FAIL;
        }
        tmpUser = IsExistUser(usrList, userName);
        if (tmpUser) {
            tmpNode2->id.UID = tmpUser->id.UID;
            tmpNode2->id.GID = tmpUser->id.GID;
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

void changeOwnerAll(DirectoryNode *dirNode, char *userName) {
    UserNode *tmpUser = NULL;

    tmpUser = IsExistUser(usrList, userName);
    if (dirNode->nextSibling) {
        changeOwnerAll(dirNode->nextSibling, userName);
    }
    if (dirNode->firstChild) {
        changeOwnerAll(dirNode->firstChild, userName);
    }
    dirNode->id.UID = tmpUser->id.UID;
    dirNode->id.GID = tmpUser->id.GID;
}

int ft_chown(DirectoryTree* dirTree, char* cmd) {
    DirectoryNode* tmpNode = NULL;
    UserNode* tmpUser = NULL;
    char* str;
    char tmp[MAX_NAME];

    if (!cmd) {
        printf("chown: Invalid option\n");
        printf("Try 'chown --help' for more information.\n");
        return -1;
    }
    if (cmd[0] == '-') {
        if (!strcmp(cmd, "-R")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("chown: Invalid option\n");
                printf("Try 'chown --help' for more information.\n");
                return FAIL;
            }
            tmpUser = IsExistUser(usrList, str);
            if (tmpUser) strncpy(tmp, str, MAX_NAME);
            else {
                printf("chown: Invalid option: '%s'\n", str);
                printf("Try 'chown --help' for more information.\n");
                return FAIL;
            }
            str = strtok(NULL, " ");
            if (!str) {
                printf("chown: Invalid option\n");
                printf("Try 'chown --help' for more information.\n");
                return FAIL;
            }
            tmpNode = IsExistDir(dirTree, str, 'd');
            if (tmpNode) {
                if (!tmpNode->firstChild) changeOwner(dirTree, tmp, str);
                else {
                    changeOwner(dirTree, tmp, str);
                    changeOwnerAll(tmpNode->firstChild, tmp);
                }
            } else {
                printf("chown: '%s': No such file or directory.\n", str);
                return FAIL;
            }
        } else if(!strcmp(cmd, "--help")) {
            printf("Usage: chown [OPTION]... [OWNER] FILE...\n");
            printf("Change the owner of each FILE to OWNER.\n\n");
            printf("  Options:\n");
            printf("  -R, --recursive        operate on files and directories recursively\n");
            printf("      --help     display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(cmd, "-");
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
        strncpy(tmp, cmd, MAX_NAME);
        str = strtok(NULL, " ");
        if (!str) {
            printf("chown: Invalid option\n");
            printf("Try 'chown --help' for more information.\n");
            return FAIL;
        } else changeOwner(dirTree, tmp, str);
    }
    return SUCCESS;
}