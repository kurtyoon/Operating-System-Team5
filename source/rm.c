#include "../include/main.h"

// free directory node
void destroyNode(DirectoryNode *dirNode) {
    free(dirNode);
}

// if directory node has child or sibling, use recursive
void destroyDir(DirectoryNode *dirNode) {
    if (dirNode->nextSibling) {
        destroyDir(dirNode->nextSibling);
    }
    if (dirNode->firstChild) {
        destroyDir(dirNode->firstChild);
    }
    dirNode->firstChild = NULL;
    dirNode->nextSibling = NULL;
    destroyNode(dirNode);
}

int removeDir(DirectoryTree *dirTree, char *dirName) {
    DirectoryNode *DelNode = NULL;
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *prevNode = NULL;

    tmpNode = dirTree->current->firstChild;
    if (!tmpNode) {
        printf("rm: Can not remove '%s': No such file or directory.\n", dirName);
        return FAIL;
    }
    // if tmpNode->name == dirName
    if(!strcmp(tmpNode->name, dirName)) {
        dirTree->current->firstChild = tmpNode->nextSibling;
        DelNode = tmpNode;
        if (DelNode->firstChild) destroyDir(DelNode->firstChild);
        destroyNode(DelNode);
    } else {
        while (tmpNode) {
            if (!strcmp(tmpNode->name, dirName)) {
                DelNode = tmpNode;
                break;
            }
            prevNode = tmpNode;
            tmpNode = tmpNode->nextSibling;
        }
        if (DelNode) {
            prevNode->nextSibling = DelNode->nextSibling;
            if (DelNode->firstChild) destroyDir(DelNode->firstChild);
            destroyNode(DelNode);
        } else {
            printf("rm: Can not remove '%s': No such file or directory.\n", dirName);
            return FAIL;
        }
    }
    return SUCCESS;
}

int ft_rm(DirectoryTree *dirTree, char *command) {
    DirectoryNode *currentNode = NULL;
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    char *str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int val;

    // not command
    if (!command) {
        printf("rm: rm: Invalid option\n");
        printf("Try 'rm --help' for more information.\n");
        return FAIL;
    }
    currentNode = dirTree->current;
    if (command[0] == '-'){
        // command is rm -r
        if (!strcmp(command, "-r")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("rm: Invalid option\n");
                printf("Try 'rm --help' for more information.\n");
                return FAIL;
            }
            strncpy(tmp, str, MAX_DIR); // copy dirName to tmp
            if (!strstr(str, "/")) { // find sutstring '/' in str
                tmpNode = dirExistence(dirTree, str, 'd');
                if (!tmpNode) {
                    printf("rm: Can not remove '%s': No such file or directory.\n", str);
                    return FAIL;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        printf("rm: failed to remove '%s'Can not remove directory or file: Permission denied.", str);
                        return FAIL;
                    }
                    removeDir(dirTree, str);
                }
            } else {
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = movePath(dirTree, tmp2);
                if (val) {
                    printf("rm: '%s': No such file or directory.\n", tmp2);
                    return FAIL;
                }
                str = strtok(tmp, "/");
                while (str) {
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = dirExistence(dirTree, tmp3, 'f');
                tmpNode = dirExistence(dirTree, tmp3, 'd') == NULL ? tmpNode : dirExistence(dirTree, tmp3, 'd');
                if (!tmpNode) {
                    printf("rm: Can not remove '%s': No such file or directory.\n", tmp3);
                    dirTree->current = currentNode;
                    return FAIL;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        printf("rm: failed to remove '%s' Can not remove directory or file: Permission denied.\n", tmp3);
                        dirTree->current = currentNode;
                        return FAIL;
                    }
                    removeDir(dirTree, tmp3);
                }
                dirTree->current = currentNode;
            }
        } else if(!strcmp(command, "-f")) {
            str = strtok(NULL, " ");
            if (!str) {
                return FAIL;
            }
            strncpy(tmp, str, MAX_DIR);
            if (!strstr(str, "/")) {
                tmpNode = dirExistence(dirTree, str, 'f');
                if (!tmpNode2) {
                    return FAIL;
                }
                if (!tmpNode) {
                    return FAIL;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        return FAIL;
                    }
                    removeDir(dirTree, str);
                }
            } else {
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = movePath(dirTree, tmp2);
                if (val) {
                    return FAIL;
                }
                str = strtok(tmp, "/");
                while (str) {
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = dirExistence(dirTree, tmp3, 'f');
                if (!tmpNode) {
                    dirTree->current = currentNode;
                    return FAIL;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        dirTree->current = currentNode;
                        return FAIL;
                    }
                    removeDir(dirTree, tmp3);
                }
                dirTree->current = currentNode;
            }
        } else if (!strcmp(command, "-rf")) {
            str = strtok(NULL, " ");
            if (!str) {
                return FAIL;
            }
            strncpy(tmp, str, MAX_DIR);
            if (!strstr(str, "/")) {
                tmpNode = dirExistence(dirTree, str, 'f');
                tmpNode = dirExistence(dirTree, str, 'd') == NULL ? tmpNode : dirExistence(dirTree, str, 'd');
                if(!tmpNode){
                    return FAIL;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        return FAIL;
                    }
                    removeDir(dirTree, str);
                }
            } else {
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = movePath(dirTree, tmp2);
                if (val) return FAIL;
                str = strtok(tmp, "/");
                while (str) {
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = dirExistence(dirTree, tmp3, 'f');
                tmpNode = dirExistence(dirTree, tmp3, 'd') == NULL ? tmpNode : dirExistence(dirTree, tmp3, 'd');
                if (!tmpNode) {
                    dirTree->current = currentNode;
                    return FAIL;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        dirTree->current = currentNode;
                        return FAIL;
                    }
                    removeDir(dirTree, tmp3);
                }
                dirTree->current = currentNode;
            }
        } else if (!strcmp(command, "--help")) {
            printf("Manual: rm [Option]... [<File>]...\n");
            printf("  Remove the FILE(s).\n\n");
            printf("  Options:\n");
            printf("    -f, --force    \t ignore nonexistent files and arguments, never prompt\n");
            printf("    -r, --recursive\t remove directories and their contents recursively\n");
            printf("        --help\t Display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("rm: Invalid option\n");
                printf("Try 'rm --help' for more information.\n");
                return FAIL;
            } else {
                printf("rm: Unrecognized option -- '%s'\n", str);
                printf("Try 'rm --help' for more information.\n");
                return FAIL;
            }
        }
    } else {
        strncpy(tmp, command, MAX_DIR);
        if (!strstr(command, "/")) {
            tmpNode = dirExistence(dirTree, command, 'f');
            tmpNode2 = dirExistence(dirTree, command, 'd');

            if (tmpNode2) {
                printf("rm:Can not remove '%s': Is a directory.\n", command);
                return FAIL;
            }
            if (!tmpNode) {
                printf("rm: Can not remove '%s': No such file or directory.\n", command);
                return FAIL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: Can not remove '%s': Permission denied.\n", command);
                    return FAIL;
                }
                removeDir(dirTree, command);
            }
        } else {
            strncpy(tmp2, getDir(command), MAX_DIR);
            val = movePath(dirTree, tmp2);
            if (val) {
                printf("rm: Can not remove '%s': No such file or directory.\n", tmp2);
                return FAIL;
            }
            str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = dirExistence(dirTree, tmp3, 'f');
            tmpNode2 = dirExistence(dirTree, tmp3, 'd');

            if (tmpNode2) {
                printf("rm: Can not remove '%s': Is a directory.\n", tmp3);
                dirTree->current = currentNode;
                return FAIL;
            }
            if (!tmpNode) {
                printf("rm: Can not remove '%s' No such file or directory.\n", tmp3);
                dirTree->current = currentNode;
                return FAIL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: Can not remove '%s' Permission denied.\n", tmp3);
                    dirTree->current = currentNode;
                    return FAIL;
                }
                removeDir(dirTree, tmp3);
            }
            dirTree->current = currentNode;
        }
    }
    return SUCCESS;
}