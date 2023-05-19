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

void *rmThread(void *arg) {
    ThreadTree *threadTree = (ThreadTree *)arg;
    DirectoryTree *dirTree = threadTree->threadTree;

    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;

    char *command = threadTree->command;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    DirectoryNode *currentNode = dirTree->current;

    strncpy(tmp, command, MAX_DIR);
    int Option = threadTree->option;

    if (Option == 0) {
        if (!strstr(tmp, "/")) {
            tmpNode = dirExistence(dirTree, tmp, 'f');
            tmpNode2 = dirExistence(dirTree, tmp, 'd');
            if (!tmpNode && !tmpNode2) {
                printf("rm: Can not remove '%s': No such file or directory.\n", tmp);
                return NULL;
            }
            if (!tmpNode) {
                printf("rm: Can not remove '%s': No such file or directory.\n", command);
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: Can not remove '%s': Permission denied.\n", command);
                    return NULL;
                }
                removeDir(dirTree, tmp);
            }
        } else {
            strncpy(tmp2, getDir(tmp), MAX_DIR);
            int value = movePath(dirTree, tmp2);
            if (value) {
                printf("rm: Can not remove '%s': No such file or directory.\n", tmp2);
                return NULL;
            }
            char *str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = dirExistence(dirTree, tmp3, 'f');
            tmpNode2 = dirExistence(dirTree, tmp3, 'd');
            if (tmpNode2) {
                printf("rm: Can not remove '%s': Is a directory.\n", tmp3);
                dirTree->current = currentNode;
                return NULL;
            }
            if (!tmpNode) {
                printf("rm: Can not remove '%s' No such file or directory.\n", tmp3);
                dirTree->current = currentNode;
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: Can not remove '%s' Permission denied.\n", tmp3);
                    dirTree->current = currentNode;
                    return NULL;
                }
                removeDir(dirTree, tmp3);
            }
            dirTree->current = currentNode;
        }
    } else if (Option == 1) {
        if (!strstr(tmp, "/")) {
            tmpNode = dirExistence(dirTree, tmp, 'd');
            if (!tmpNode) {
                printf("rm: Can not remove '%s': No such file or directory.\n", tmp);
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: failed to remove '%s'Can not remove directory or file: Permission denied.", tmp);
                    return NULL;
                }
                removeDir(dirTree, tmp);
            }
        } else {
            strncpy(tmp2, getDir(tmp), MAX_DIR);
            int value = movePath(dirTree, tmp2);
            if (value) {
                printf("rm: '%s': No such file or directory.\n", tmp2);
                return NULL;
            }
            char *str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = dirExistence(dirTree, tmp3, 'f');
            tmpNode = dirExistence(dirTree, tmp3, 'd') == NULL ? tmpNode : dirExistence(dirTree, tmp3, 'd');
            if (tmpNode) {
                printf("rm: Can not remove '%s': No such file or directory.\n", tmp3);
                dirTree->current = currentNode;
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: failed to remove '%s' Can not remove directory or file: Permission denied.\n", tmp3);
                    dirTree->current = currentNode;
                    return NULL;
                }
                removeDir(dirTree, tmp3);
            }
            dirTree->current = currentNode;
        }
    } else if (Option == 2) {
        if (!strstr(tmp, "/")) {
            tmpNode = dirExistence(dirTree, tmp, 'f');
            if (!tmpNode) {
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    return NULL;
                }
                removeDir(dirTree, tmp);
            }
        } else {
            strncpy(tmp2, getDir(tmp), MAX_DIR);
            int value = movePath(dirTree, tmp2);
            if (value) return NULL;
            char *str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = dirExistence(dirTree, tmp3, 'f');
            if (!tmpNode) {
                dirTree->current = currentNode;
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    dirTree->current = currentNode;
                    return NULL;
                }
                removeDir(dirTree, tmp3);
            }
            dirTree->current = currentNode;
        }
    } else if (Option == 3) {
        if (!strstr(tmp, "/")) {
            tmpNode = dirExistence(dirTree, tmp, 'f');
            tmpNode = dirExistence(dirTree, tmp, 'd') == NULL ? tmpNode : dirExistence(dirTree, tmp, 'd');
            if (!tmpNode) {
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    return NULL;
                }
                removeDir(dirTree, tmp);
            }
        } else {
            strncpy(tmp2, getDir(tmp), MAX_DIR);
            int value = movePath(dirTree, tmp2);
            if (value) return NULL;
            char *str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = dirExistence(dirTree, tmp3, 'f');
            tmpNode = dirExistence(dirTree, tmp3, 'd') == NULL ? tmpNode : dirExistence(dirTree, tmp3, 'd');
            if (!tmpNode) {
                dirTree->current = currentNode;
                return NULL;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    dirTree->current = currentNode;
                    return NULL;
                }
                removeDir(dirTree, tmp3);
            }
            dirTree->current = currentNode;
        }
    }
    pthread_exit(NULL);
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

    int threadCount = 0;
    pthread_t threadPool[MAX_THREAD];
    ThreadTree threadTree[MAX_THREAD];

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
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].command = str;
                threadTree[threadCount++].option = 1;
                str = strtok(NULL, " ");
            }
        } else if(!strcmp(command, "-f")) {
            str = strtok(NULL, " ");
            if (!str) {
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 2;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "-rf") || !strcmp(command, "-fr")) {
            str = strtok(NULL, " ");
            if (!str) {
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 3;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "--help")) {
            printf("Manual: rm [Option]... [File]...\n");
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
        str = strtok(NULL, " ");
        if (!str) {
            printf("rm: Invalid option\n");
            printf("Try 'rm --help' for more information.\n");
            return FAIL;
        }
        while (str) {
            threadTree[threadCount].threadTree = dirTree;
            threadTree[threadCount].option = 0;
            threadTree[threadCount++].command = str;
            str = strtok(NULL, " ");
        }
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, rmThread, (void *)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return SUCCESS;
}