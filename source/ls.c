#include "../include/main.h"

//ls -a : 숨긴파일도 표현
//.  ..  Directory.txt  User.txt
//ls -l : file, directory의 생성 정보들 출력 (main.h)
//-rwxrwxr-x 1 ubuntu ubuntu 1052 May 22 05:34 Directory.txt
//ls -al,-la : '-a' + '-l' 


//list Dir ; 디렉토리의 내용을 나열하는 역할
//dirTree->current로 현재 directory를 가져온 후, optionA,L에 따라 file, directory를 출력함.
//optionA = 0: 숨겨진 파일 무시, optionA = 1: 숨겨진 파일도 출력
//optionL = 0: 간단한 형식, optionL = 1: 상세한 형식
int listDir(DirectoryTree *dirTree, int optionA, int optionL) {
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    char type;
    int cnt;

    tmpNode = dirTree->current->firstChild;
    if (checkPermission(dirTree->current, 'r')) {
        printf("ls: cannot open directory '%s'': Permission denied\n", dirTree->current->name);
        return FAIL;
    }
    //optionA
    if (!optionL) {
        if (!optionA) {
            if (!tmpNode) return FAIL;
        }
        if (optionA) {
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
            if (!optionA) {
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
    //optionL
    else {
        if (!optionA) {
            if (!tmpNode) {
                printf("total: 0\n");
                return FAIL;
            }
        }
        if (optionA) {
            tmpNode2 = dirTree->current->firstChild;
            if (!tmpNode2) cnt = 2;
            else {
                if (tmpNode2->type == 'd') cnt = 3;
                else cnt = 2;
                while (tmpNode2->nextSibling) {
                    tmpNode2 = tmpNode2->nextSibling;
                    if(tmpNode2->type == 'd') cnt += 1;
                }
            }
            printf("%c", dirTree->current->type);
            printPermission(dirTree->current);
            printf("%3d", cnt);
            printf("   ");
            printf("%-5s%-5s", getUID(dirTree->current), getGID(dirTree->current));
            printf("%5d ", dirTree->current->SIZE);
            getMonth(dirTree->current->date.month);
            printf("%2d %02d:%02d ", dirTree->current->date.day, dirTree->current->date.hour, dirTree->current->date.minute);
            BOLD;BLUE;
            printf(".\n");
	        DEFAULT;

            if (dirTree->current != dirTree->root) {
                tmpNode2 = dirTree->current->parent->firstChild;
                if (!tmpNode2) cnt = 2;
                else {
                    if (tmpNode2->type == 'd') cnt = 3;
                    else cnt = 2;
                    while (tmpNode2->nextSibling) {
                        tmpNode2 = tmpNode2->nextSibling;
                        if (tmpNode2->type == 'd') cnt += 1;
                    }
                }
                printf("%c", dirTree->current->parent->type);
                printPermission(dirTree->current->parent);
                printf("%3d", cnt);
                printf("   ");
                printf("%-5s%-5s", getUID(dirTree->current->parent), getGID(dirTree->current->parent));
                printf("%5d ", dirTree->current->SIZE);
                getMonth(dirTree->current->parent->date.month);
                printf("%2d %02d:%02d ", dirTree->current->parent->date.day, dirTree->current->parent->date.hour, dirTree->current->parent->date.minute);
                BOLD;BLUE;
                printf("..\n");
		        DEFAULT;
            }
        }
        while (tmpNode) {
            if (!optionA) {
                if (!strncmp(tmpNode->name, ".", 1)) {
                    tmpNode = tmpNode->nextSibling;
                    continue;
                }
            }
            tmpNode2 = tmpNode->firstChild;
            if (!tmpNode2) {
                if(tmpNode->type == 'd') cnt = 2;
                else cnt = 1;
            } else {
                if(tmpNode2->type == 'd') cnt = 3;
                else cnt = 2;
                while (tmpNode2->nextSibling) {
                    tmpNode2 = tmpNode2->nextSibling;
                    if (tmpNode2->type == 'd') cnt += 1;
                }
            }
            if (tmpNode->type == 'd') type = 'd';
            else if(tmpNode->type == 'f') type = '-';
            printf("%c", type);
            printPermission(tmpNode);
            printf("%3d", cnt);
            printf("   ");
            printf("%-5s%-5s", getUID(tmpNode), getGID(tmpNode));
            printf("%5d ", tmpNode->SIZE);
            getMonth(tmpNode->date.month);
            printf("%2d %02d:%02d ", tmpNode->date.day, tmpNode->date.hour, tmpNode->date.minute);
            if(tmpNode->type == 'd'){
                BOLD;BLUE;
                printf("%-15s\n", tmpNode->name);
                DEFAULT;
	        } else printf("%-15s\n", tmpNode->name);
            tmpNode = tmpNode->nextSibling;
        }
    }
    return SUCCESS;
}


int ft_ls(DirectoryTree *dirTree, char *command) {
    DirectoryNode *tmpNode = NULL;
    char *str;
    int value;

    //ls
    if (!command) {
        listDir(dirTree, 0, 0);
        return 0;
    }
    //ls option exists.
    if (command[0] == '-') {
        //ls -al, -la
        if (!strcmp(command, "-al") || !strcmp(command, "-la")) {
            str = strtok(NULL, " ");
            if (str) {
                tmpNode = dirTree->current;
                value = movePath(dirTree, str);
                if (value) return FAIL;
            }
            listDir(dirTree, 1, 1);
        //ls -l            
        } else if (!strcmp(command, "-l")) {
            str = strtok(NULL, " ");
            if (str) {
                tmpNode = dirTree->current;
                value = movePath(dirTree, str);
                if (value) return FAIL;
            }
            listDir(dirTree, 0, 1);
        //ls -a            
        } else if (!strcmp(command, "-a")) {
            str = strtok(NULL, " ");
            if (str) {
                tmpNode = dirTree->current;
                value = movePath(dirTree, str);
                if (value) return FAIL;
            }
            listDir(dirTree, 1, 0);
        //ls --help : print guideline    
        } else if(!strcmp(command, "--help")) {
            printf("Usage: ls [OPTION]... [FILE]...\n");
            printf("List information about the FILEs (the current directory by default).\n\n");
            printf("  Options:\n");
            printf("  -a, --all                  do not ignore entries starting with .\n");
            printf("  -l                         use a long listing format\n");
            printf("      --help     display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            //ls - : print guideline
            if (!str) {
                printf("ls: ls: cannot access '-': No such file or directory\n");
                return FAIL;
            //ls -- : print guideline    
            } else {
                printf("ls: invalid option -- '%s'\n", str);
                printf("Try 'ls --help' for more information.\n");
                return FAIL;
            }
        }

    } else {
        tmpNode = dirTree->current;
        value = movePath(dirTree, command);
        if (value) return FAIL;
        listDir(dirTree, 0, 0);
        dirTree->current = tmpNode;
    }
    if (str) dirTree->current = tmpNode;
    return SUCCESS;
}
