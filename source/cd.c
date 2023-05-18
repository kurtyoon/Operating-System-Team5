#include "../include/main.h"

int Movecurrent(DirectoryTree* dirTree, char* dirPath) {
    DirectoryNode* tmpNode = NULL;

    if (!strcmp(dirPath,".")) {
    } else if (!strcmp(dirPath, "..")) {
        if (dirTree->current != dirTree->root) dirTree->current = dirTree->current->parent;
    } else {
        tmpNode = IsExistDir(dirTree, dirPath, 'd');
        if (tmpNode) dirTree->current = tmpNode;
        else return FAIL;
    }
    return SUCCESS;
}

int MovePath(DirectoryTree* dirTree, char* dirPath) {
    DirectoryNode* tmpNode = NULL;
    char tmpPath[MAX_DIR];
    char* str = NULL;
    int val = 0;

    strncpy(tmpPath, dirPath, MAX_DIR);
    tmpNode = dirTree->current;

    if (!strcmp(dirPath, "/")) dirTree->current = dirTree->root;
    else {
        if (!strncmp(dirPath, "/", 1)) {
            if (strtok(dirPath, "/")) return -1;
            dirTree->current = dirTree->root;
        }
        str = strtok(tmpPath, "/");
        while (str) {
            val = Movecurrent(dirTree, str);
            if (val) {
                dirTree->current = tmpNode;
                return -1;
            }
            str = strtok( NULL, "/");
        }
    }
    return SUCCESS;
}

int ft_cd(DirectoryTree* dirTree, char* cmd) {
    DirectoryNode* tmpNode = NULL;
    char* str = NULL;
    char tmp[MAX_DIR];
    int val;

    if (!cmd) {
        strcpy(tmp, usrList->current->dir);
        MovePath(dirTree, tmp);
    } else if(cmd[0] == '-') {
        if (!strcmp(cmd, "--help")) {
            printf("cd: cd [dir]\n");
            printf("    Change the shell working directory.\n\n");
            printf("    Change the current directory to DIR.  The default DIR is the value of the\n");
            printf("    HOME shell variable.\n\n");
            printf("    The variable CDPATH defines the search path for the directory containing\n");
            printf("    DIR.  Alternative directory names in CDPATH are separated by a colon (:).\n");
            printf("    A null directory name is the same as the current directory.  If DIR begins\n");
            printf("    with a slash (/), then CDPATH is not used.\n\n");
            printf("    If the directory is not found, and the shell option `cdable_vars' is set,\n");
            printf("    the word is assumed to be  a variable name.  If that variable has a value,\n");
            printf("    its value is used for DIR.\n\n");
            printf("    Options:\n");
            printf("      --help     display this help and exit\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if (!str) {
                printf("cd: Invalid option\n");
                printf("Try 'cd --help' for more information.\n");
                return FAIL;
            } else {
            printf("cd: Unrecognized option -- '%s'\n", str);
            printf("Try 'cd --help' for more information.\n");
            return FAIL;
            }
        }
    } else {
        tmpNode = IsExistDir(dirTree, cmd, 'd');
        if (tmpNode) {
            if (checkPermission(tmpNode, 'r')) {
                printf("-bash: cd: '%s': Permission denied\n", cmd);
                return FAIL;
            }
        }
        tmpNode = IsExistDir(dirTree, cmd,  'f');
        if (tmpNode) {
            printf("-bash: cd: '%s': Not a directory\n", cmd);
            return FAIL;
        }
        val = MovePath(dirTree, cmd);
        if (val) printf("-bash: cd: No such file or directory '%s'\n", cmd);
    }
    return SUCCESS;
}