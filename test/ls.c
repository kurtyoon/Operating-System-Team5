#include "main.h"

int ListDir(DirectoryTree *dirTree, int a, int l) {
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    char type;
    int cnt;

    tmpNode = dirTree->current->firstChild;

    if (checkPermission(dirTree->current, 'r')) {
        printf("ls: '%s'디렉터리를 열 수 없음: 허가거부\n", dirTree->current->name);
        return -1;
    }
    if (!l) {
        if (!a) {
            if (!tmpNode) return -1;
        }
        if (a == 1) {
	        BOLD;BLUE;
            printf(".\t");
	        DEFAULT;
            if(dirTree->current != dirTree->root){
	     	    BOLD;BLUE;
                printf("..\t");
		        DEFAULT;
            }
        }
        while (tmpNode) {
            if (!a) {
                if (!strncmp(tmpNode->name, ".", 1)) {
                    tmpNode = tmpNode->nextSibling;
                    continue;
                }
            }
            if(tmpNode->type == 'd'){
                BOLD;BLUE;
                printf("%s\t", tmpNode->name);
                DEFAULT;
            } else {
                printf("%s\t", tmpNode->name);
            }
            tmpNode = tmpNode->nextSibling;
        }
        printf("\n");
    }
    else {
        if (!a) {
            if (!tmpNode) {
                printf("합계: 0\n");
                return -1;
            }
        }
        if (a) {
            tmpNode2 = dirTree->current->firstChild;
            if (!tmpNode2) {
                cnt = 2;
            } else {
                if (tmpNode2->type == 'd') {
                    cnt = 3;
                } else {
                    cnt = 2;
                }
                while (tmpNode2->nextSibling) {
                    tmpNode2 = tmpNode2->nextSibling;
                    if(tmpNode2->type == 'd') {
                        cnt += 1;
                    }
                }
            }
            printf("%c", dirTree->current->type);
            printPermission(dirTree->current);
            printf("%3d", cnt);
            printf("   ");
            printf("%-5s%-5s", GetUID(dirTree->current), GetGID(dirTree->current));
            printf("%5d ", dirTree->current->SIZE);
            printf("%d월 %2d %02d:%02d ",dirTree->current->date.month, dirTree->current->date.day, dirTree->current->date.hour, dirTree->current->date.minute);
            BOLD;BLUE;
            printf(".\n");
	        DEFAULT;

            if (dirTree->current != dirTree->root) {
                tmpNode2 = dirTree->current->parent->firstChild;
                if (!tmpNode2) {
                    cnt = 2;
                } else {
                    if(tmpNode2->type == 'd') {
                        cnt = 3;
                    } else {
                        cnt = 2;
                    }
                    while (tmpNode2->nextSibling) {
                        tmpNode2 = tmpNode2->nextSibling;
                        if(tmpNode2->type == 'd') {
                            cnt += 1;
                        }
                    }
                }
                printf("%c", dirTree->current->parent->type);
                printPermission(dirTree->current->parent);
                printf("%3d", cnt);
                printf("   ");
                printf("%-5s%-5s", GetUID(dirTree->current->parent), GetGID(dirTree->current->parent));
                printf("%5d ", dirTree->current->SIZE);
                printf("%d월 %2d %02d:%02d ",dirTree->current->parent->date.month, dirTree->current->parent->date.day, dirTree->current->parent->date.hour, dirTree->current->parent->date.minute);
                BOLD;BLUE;
                printf("..\n");
		        DEFAULT;
            }
        }
        while (tmpNode) {
            if (!a) {
                if (!strncmp(tmpNode->name, ".", 1)) {
                    tmpNode = tmpNode->nextSibling;
                    continue;
                }
            }
            tmpNode2 = tmpNode->firstChild;
            if (!tmpNode2) {
                if(tmpNode->type == 'd') {
                    cnt = 2;
                } else {
                    cnt  =1;
                }
            } else {
                if(tmpNode2->type == 'd') {
                    cnt = 3;
                } else {
                    cnt = 2;
                }
                while (tmpNode2->nextSibling) {
                    tmpNode2 = tmpNode2->nextSibling;
                    if(tmpNode2->type == 'd') {
                        cnt += 1;
                    }
                }
            }
            if (tmpNode->type == 'd') {
                type = 'd';
            } else if(tmpNode->type == 'f') {
                type = '-';
            }
            printf("%c", type);
            printPermission(tmpNode);
            printf("%3d", cnt);
            printf("   ");
            printf("%-5s%-5s", GetUID(tmpNode), GetGID(tmpNode));
            printf("%5d ", tmpNode->SIZE);
            printf("%d월 %2d %02d:%02d ",tmpNode->date.month, tmpNode->date.day, tmpNode->date.hour, tmpNode->date.minute);
            if(tmpNode->type == 'd'){
                BOLD;BLUE;
                printf("%-15s\n", tmpNode->name);
                DEFAULT;
	        } else {
                printf("%-15s\n", tmpNode->name);
            }
            tmpNode = tmpNode->nextSibling;
        }
    }
    return 0;
}

int ft_ls(DirectoryTree *dirTree, char *cmd) {
    DirectoryNode *tmpNode = NULL;
    char *str;
    int val;

    if (!cmd) {
        ListDir(dirTree, 0, 0);
        return 0;
    }
    if (cmd[0] == '-') {
        if (!strcmp(cmd, "-al") || !strcmp(cmd, "-la")) {
            str = strtok(NULL, " ");
            if (str) {
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if (val) return -1;
            }
            ListDir(dirTree, 1, 1);
        } else if (!strcmp(cmd, "-l")) {
            str = strtok(NULL, " ");
            if (str) {
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if (val) return -1;
            }
            ListDir(dirTree, 0, 1);
        } else if (!strcmp(cmd, "-a")) {
            str = strtok(NULL, " ");
            if (str) {
                tmpNode = dirTree->current;
                val = MovePath(dirTree, str);
                if (val) return -1;
            }
            ListDir(dirTree, 1, 0);
        } else if(!strcmp(cmd, "--help")) {
            printf("사용법: ls [<옵션>]... [<파일>]...\n");
            printf("  List information about the FILEs (the current directory by default).\n\n");
            printf("  Options:\n");
            printf("    -a, --all\t do not ignore entries starting with .\n");
            printf("    -l       \t use a long listing format\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if (!str) {
                printf("ls: 잘못된 연산자\n");
                printf("Try 'ls --help' for more information.\n");
                return -1;
            } else {
                printf("ls: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'ls --help' for more information.\n");
                return -1;
            }
        }
    } else {
        tmpNode = dirTree->current;
        val = MovePath(dirTree, cmd);
        if (val) return -1;
        ListDir(dirTree, 0, 0);
        dirTree->current = tmpNode;
    }
    if (str) dirTree->current = tmpNode;
    return 0;
}