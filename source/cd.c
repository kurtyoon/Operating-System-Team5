#include "../include/main.h"

//cd(Change_Directory)

int moveCurrent(DirectoryTree *dirTree, char *dirPath) {
    DirectoryNode *tmpNode = NULL;

    if (!strcmp(dirPath,".")) {
    } else if (!strcmp(dirPath, "..")) {
        //"cd .." ; if(current dirTree is not root) go to parent dirTree.
        if (dirTree->current != dirTree->root) dirTree->current = dirTree->current->parent;
    } else {
        //"cd <dirPath>" ; if dirExistence, go to <dirPath>, else Fail.
        tmpNode = dirExistence(dirTree, dirPath, 'd');
        if (tmpNode) dirTree->current = tmpNode;
        else return FAIL;
    }
    return SUCCESS;
}

//"cd / " ; root로 이동. 
//"cd /user/dir1" ; /user/dir1 경로의 디렉토리로 이동.
int movePath(DirectoryTree *dirTree, char *dirPath) {
    DirectoryNode *tmpNode = NULL;
    char tmpPath[MAX_DIR];
    char *str = NULL;
    int value = 0;

    //copy <dirPath>
    strncpy(tmpPath, dirPath, MAX_DIR);
    tmpNode = dirTree->current;

    //if "cd /", go to root.
    if (!strcmp(dirPath, "/")) dirTree->current = dirTree->root;
    else {
        if (!strncmp(dirPath, "/", 1)) {
            if (!strtok(dirPath, "/")) return FAIL;
            dirTree->current = dirTree->root;
        }
        //if "cd /user/dir1" ; '/' = delimiters, moveCurrent until NULL.
        str = strtok(tmpPath, "/");
        while (str) {
            value = moveCurrent(dirTree, str);
            if (value) {
                dirTree->current = tmpNode;
                return FAIL;
            }
            str = strtok(NULL, "/");
        }
    }
    return SUCCESS;
}

//main
int ft_cd(DirectoryTree *dirTree, char *command) {
    DirectoryNode *tmpNode = NULL;
    char *str = NULL;
    char tmp[MAX_DIR];
    int value;


    if (!command) {
        strcpy(tmp, usrList->current->dir);
        movePath(dirTree, tmp);
    } else if(command[0] == '-') {
    //"cd --help"
        if (!strcmp(command, "--help")) {
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
            return FAIL;
        } else {
            str = strtok(command, "-");
            //if, "cd -" command guide 제시
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
    //moveCurrent,movePath  
    } else {
        //directory access
        tmpNode = dirExistence(dirTree, command, 'd');
        if (tmpNode) {
            //Permission 'x' = 실행권한
            if (checkPermission(tmpNode, 'x')) {
                printf("-bash: cd: '%s': Permission denied\n", command);
                return FAIL;
            }
        }
        //file access -> file is not directory, FAIL
        tmpNode = dirExistence(dirTree, command, 'f');
        if (tmpNode) {
            printf("-bash: cd: '%s': Not a directory\n", command);
            return FAIL;
        }
        //if movePath, FAIL
        value = movePath(dirTree, command);
        if (value) printf("-bash: cd: No such file or directory '%s'\n", command);
    }
    return SUCCESS;
}