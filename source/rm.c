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
        pthread_create(&threadPool[i], NULL, removeDirUsedThread, (void *)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return SUCCESS;
}