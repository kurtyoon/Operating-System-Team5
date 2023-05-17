#include "main.h"

int RemoveDir(DirectoryTree *dirTree, char *dirName) {
    DirectoryNode *DelNode = NULL;
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *prevNode = NULL;

    tmpNode = dirTree->current->firstChild;

    if (!tmpNode) {
        printf("rm: '%s'를 지울 수 없음: 그런 파일이나 디렉터리가 없습니다\n", dirName);
        return -1;
    }
    if(!strcmp(tmpNode->name, dirName)) {
        dirTree->current->firstChild = tmpNode->nextSibling;
        DelNode = tmpNode;
        if (DelNode->firstChild) DestroyDir(DelNode->firstChild);
        DestroyNode(DelNode);
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
            if (DelNode->firstChild) DestroyDir(DelNode->firstChild);
            DestroyNode(DelNode);
        } else {
            printf("rm: '%s'를 지울 수 없음: 그런 파일이나 디렉터리가 없습니다\n", dirName);
            return -1;
        }
    }
    return 0;
}

int ft_rm(DirectoryTree *dirTree, char *cmd) {
    DirectoryNode *currentNode = NULL;
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    char *str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int val;

    if (!cmd) {
        printf("rm: 잘못된 연산자\n");
        printf("Try 'rm --help' for more information.\n");
        return -1;
    }
    currentNode = dirTree->current;
    if (cmd[0] == '-'){
        if (!strcmp(cmd, "-r")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("rm: 잘못된 연산자\n");
                printf("Try 'rm --help' for more information.\n");
                return -1;
            }
            strncpy(tmp, str, MAX_DIR);
            if (!strstr(str, "/")) {
                tmpNode = IsExistDir(dirTree, str, 'd');
                if (!tmpNode) {
                    printf("rm: '%s'를 지울 수 없음: 그런 파일이나 디렉터리가 없습니다\n", str);
                    return -1;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        printf("rm: '%s'디렉터리 또는 파일을 지울 수 없습니다: 허가거부\n", str);
                        return -1;
                    }
                    RemoveDir(dirTree, str);
                }
            } else {
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if (val) {
                    printf("rm: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                    return -1;
                }
                str = strtok(tmp, "/");
                while (str) {
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'd');
                if (!tmpNode) {
                    printf("rm: '%s'를 지울 수 없음: 그런 파일이나 디렉터리가 없습니다\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        printf("rm: '%s'디렉터리 또는 파일을 지울 수 없습니다: 허가거부\n", tmp3);
                        dirTree->current = currentNode;
                        return -1;
                    }
                    RemoveDir(dirTree, tmp3);
                }
                dirTree->current = currentNode;
            }
        } else if(!strcmp(cmd, "-f")) {
            str = strtok(NULL, " ");
            if (!str) {
                return -1;
            }
            strncpy(tmp, str, MAX_DIR);
            if (!strstr(str, "/")) {
                tmpNode = IsExistDir(dirTree, str, 'f');
                tmpNode2 = IsExistDir(dirTree, str, 'd');

                if (!tmpNode2) {
                    return -1;
                }
                if (!tmpNode) {
                    return -1;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        return -1;
                    }
                    RemoveDir(dirTree, str);
                }
            } else {
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if (val) {
                    return -1;
                }
                str = strtok(tmp, "/");
                while (str) {
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'f');
                tmpNode2 = IsExistDir(dirTree, tmp3, 'd');

                if (tmpNode2) {
                    dirTree->current = currentNode;
                    return -1;
                }
                if (!tmpNode) {
                    dirTree->current = currentNode;
                    return -1;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        dirTree->current = currentNode;
                        return -1;
                    }
                    RemoveDir(dirTree, tmp3);
                }
                dirTree->current = currentNode;
            }
        } else if (!strcmp(cmd, "-rf")) {
            str = strtok(NULL, " ");
            if (!str) {
                return -1;
            }
            strncpy(tmp, str, MAX_DIR);
            if (!strstr(str, "/")) {
                tmpNode = IsExistDir(dirTree, str, 'd');
                if(!tmpNode){
                    return -1;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        return -1;
                    }
                    RemoveDir(dirTree, str);
                }
            } else {
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if (val) {
                    return -1;
                }
                str = strtok(tmp, "/");
                while (str) {
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'd');
                if (!tmpNode) {
                    dirTree->current = currentNode;
                    return -1;
                } else {
                    if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                        dirTree->current = currentNode;
                        return -1;
                    }
                    RemoveDir(dirTree, tmp3);
                }
                dirTree->current = currentNode;
            }
        } else if (!strcmp(cmd, "--help")) {
            printf("사용법: rm [<옵션>]... [<파일>]...\n");
            printf("  Remove (unlink) the FILE(s).\n\n");
            printf("  Options:\n");
            printf("    -f, --force    \t ignore nonexistent files and arguments, never prompt\n");
            printf("    -r, --recursive\t remove directories and their contents recursively\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if (!str) {
                printf("rm: 잘못된 연산자\n");
                printf("Try 'rm --help' for more information.\n");
                return -1;
            } else {
                printf("rm: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'rm --help' for more information.\n");
                return -1;
            }
        }
    } else {
        strncpy(tmp, cmd, MAX_DIR);
        if (!strstr(cmd, "/")) {
            tmpNode = IsExistDir(dirTree, cmd, 'f');
            tmpNode2 = IsExistDir(dirTree, cmd, 'd');

            if (tmpNode2) {
                printf("rm:'%s'를 지울 수 없음: 디렉터리입니다\n", cmd);
                return -1;
            }
            if (!tmpNode) {
                printf("rm: '%s'를 지울 수 없음: 그런 파일이나 디렉터리가 없습니다\n", cmd);
                return -1;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: '%s'디렉터리 또는 파일을 지울 수 없습니다: 허가거부\n", cmd);
                    return -1;
                }
                RemoveDir(dirTree, cmd);
            }
        } else {
            strncpy(tmp2, getDir(cmd), MAX_DIR);
            val = MovePath(dirTree, tmp2);
            if (val) {
                printf("rm: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                return -1;
            }
            str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = IsExistDir(dirTree, tmp3, 'f');
            tmpNode2 = IsExistDir(dirTree, tmp3, 'd');

            if (tmpNode2) {
                printf("rm:'%s'를 지울 수 없음: 디렉터리입니다\n", tmp3);
                dirTree->current = currentNode;
                return -1;
            }
            if (!tmpNode) {
                printf("rm: '%s'를 지울 수 없음: 그런 파일이나 디렉터리가 없습니다\n", tmp3);
                dirTree->current = currentNode;
                return -1;
            } else {
                if (checkPermission(dirTree->current, 'w') || checkPermission(tmpNode, 'w')) {
                    printf("rm: '%s'디렉터리 또는 파일을 지울 수 없습니다: 허가거부\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                RemoveDir(dirTree, tmp3);
            }
            dirTree->current = currentNode;
        }
    }
    return 0;
}