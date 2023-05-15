#include "../include/main.h"

int Concatenate(DirectoryTree* dirTree, char* fName, int type) {

    UserNode* tmpUser = NULL;
    DirectoryNode* tmpNode = NULL;
    FILE* fp;
    char buf[MAX_BUFFER];
    char tmpName[MAX_NAME];
    char* str;
    int tmpSIZE = 0;
    int cnt = 1;

    if (type) {
        if(type == 4){
            tmpUser = usrList->head;
            while (tmpUser){
                printf("%s:x:%d:%d:%s:%s\n", tmpUser->name, tmpUser->id.UID, tmpUser->id.GID, tmpUser->name, tmpUser->dir);
                tmpUser = tmpUser->LinkNode;
            }
            return 0;
        }
        tmpNode = IsExistDir(dirTree,fName, 'f');

        if(!tmpNode) return -1;

        fp = fopen(fName, "r");
        while (!feof(fp)) {
            fgets(buf, sizeof(buf), fp);
            if(feof(fp)) break;
            if(type == 2) {
                if (buf[strlen(buf) - 1] == '\n'){
                    printf("     %d ",cnt);
                    cnt++;
                }
            } else if(type == 3) {
                if(buf[strlen(buf)-1] == '\n' && buf[0] != '\n'){
                    printf("     %d ",cnt);
                    cnt++;
                }
            }
            fputs(buf, stdout);
        }
        fclose(fp);
    } else {
        fp = fopen(fName, "w");

	    while (fgets(buf, sizeof(buf), stdin)) {
            fputs(buf, fp);
            tmpSIZE += strlen(buf) - 1;
        }
        fclose(fp);

        tmpNode = IsExistDir(dirTree, fName, 'f');
        if (tmpNode) {
            time(&ltime);
            today = localtime(&ltime);

            tmpNode->date.month = today->tm_mon + 1;
            tmpNode->date.day = today->tm_mday;
            tmpNode->date.hour = today->tm_hour;
            tmpNode->date.minute = today->tm_min;
        } else MakeDir(dirTree, fName, 'f');
        tmpNode = IsExistDir(dirTree, fName, 'f');
        tmpNode->SIZE = tmpSIZE;
    }
    return 0;
}

int ft_cat(DirectoryTree* dirTree, char* cmd)
{
    DirectoryNode* currentNode = NULL;
    DirectoryNode* tmpNode = NULL;
    DirectoryNode* tmpNode2 = NULL;
    char* str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int val;

    if (!cmd) {
        printf("cat: 잘못된 연산자\n");
        return -1;
    }
    currentNode = dirTree->current;

    if (strcmp(cmd, ">") == 0) {
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("cat: 잘못된 연산자\n");
            printf("Try 'cat --help' for more information.\n");
            return -1;
        }
        strncpy(tmp, str, MAX_DIR);
        if(!strstr(str, "/")) {
            if (HasPermission(dirTree->current, 'w')) {
                printf("cat: '%s'파일을 만들 수 없음: 허가거부\n", dirTree->current->name);
                return -1;
            }
            tmpNode = IsExistDir(dirTree, str, 'd');
            if (tmpNode) {
                printf("cat: '%s': 디렉터리입니다\n", str);
                return -1;
            }
            else Concatenate(dirTree, str, 0);
        } else {
            strncpy(tmp2, getDir(str), MAX_DIR);
            val = MovePath(dirTree, tmp2);
            if (val) {
                printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                return -1;
            }
            str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            if (HasPermission(dirTree->current, 'w')) {
                printf("cat: '%s'파일을 만들 수 없음: 허가거부\n", dirTree->current->name);
                dirTree->current = currentNode;
                return -1;
            }
            tmpNode = IsExistDir(dirTree, tmp3, 'd');
            if (tmpNode) {
                printf("cat: '%s': 디렉터리입니다\n", tmp3);
                dirTree->current = currentNode;
                return -1;
            } else Concatenate(dirTree, tmp3, 0);
            dirTree->current = currentNode;
        }
        return 0;
    } else if(cmd[0] == '-'){
        if (!strcmp(cmd, "-n")) {
            str = strtok(NULL, " ");
            strncpy(tmp, str, MAX_DIR);
            if (!strstr(str, "/")) {
                if (HasPermission(dirTree->current, 'w')) {
                    printf("cat: '%s'파일을 만들 수 없음: 허가거부\n", dirTree->current->name);
                    return -1;
                }
                tmpNode = IsExistDir(dirTree, str, 'd');
                tmpNode2 = IsExistDir(dirTree, str, 'f');
    
                if (!tmpNode && !tmpNode2) {
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", str);
                    return -1;
                } else if (tmpNode && !tmpNode2) {
                    printf("cat: '%s': 디렉터리입니다\n", str);
                    return -1;
                } else if (tmpNode2 && HasPermission(tmpNode2, 'r')){
                    printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                    return -1;
                } else Concatenate(dirTree, str, 2);
            } else {
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if (val) {
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                    return -1;
                }
                str = strtok(tmp, "/");
                while (str) {
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'd');
                tmpNode2 = IsExistDir(dirTree, tmp3, 'f');
    
                if (!tmpNode && !tmpNode2) {
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                else if (tmpNode && !tmpNode2) {
                    printf("cat: '%s': 디렉터리입니다\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                else if (tmpNode2 && HasPermission(tmpNode2, 'r')) {
                    printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                    dirTree->current = currentNode;
                    return -1;
                } else Concatenate(dirTree, tmp3, 2);
                dirTree->current = currentNode;
            }
        } else if(!strcmp(cmd, "-b")) {
            str = strtok(NULL, " ");
            strncpy(tmp, str, MAX_DIR);
            if (!strstr(str, "/")) {
                if (HasPermission(dirTree->current, 'w')) {
                    printf("cat: '%s'파일을 만들 수 없음: 허가거부\n", dirTree->current->name);
                    return -1;
                }
                tmpNode = IsExistDir(dirTree, str, 'd');
                tmpNode2 = IsExistDir(dirTree, str, 'f');
                if(!tmpNode && !tmpNode2) {
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", str);
                    return -1;
                }
                else if (tmpNode && !tmpNode2) {
                    printf("cat: '%s': 디렉터리입니다\n", str);
                    return -1;
                }
                else if (tmpNode2 && HasPermission(tmpNode2, 'r')) {
                    printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                    return -1;
                }
                else Concatenate(dirTree, str, 3);
                
            } else {
                strncpy(tmp2, getDir(str), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if (val) {
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                    return -1;
                }
                str = strtok(tmp, "/");
                while (str) {
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'd');
                tmpNode2 = IsExistDir(dirTree, tmp3, 'f');
                if (!tmpNode && !tmpNode2) {
                    printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                else if (tmpNode && !tmpNode2){
                    printf("cat: '%s': 디렉터리입니다\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                else if (tmpNode2 && HasPermission(tmpNode2, 'r')) {
                    printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                    dirTree->current = currentNode;
                    return -1;
                }
                else Concatenate(dirTree, tmp3, 3);
                dirTree->current = currentNode;
            }
        } else if(strcmp(cmd, "--help") == 0) {
            printf("사용법: cat [<옵션>]... [<파일>]...\n");
            printf("  FILE(들)을 합쳐서 표준 출력으로 보낸다.\n\n");
            printf("  Options:\n");
            printf("    -n, --number         \t number all output line\n");
            printf("    -b, --number-nonblank\t number nonempty output line\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if(!str) {
                printf("cat: 잘못된 연산자\n");
                printf("Try 'cat --help' for more information.\n");
                return -1;
            } else {
                printf("cat: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'cat --help' for more information.\n");
                return -1;
            }
        }
    } else {
        if (!strcmp(cmd, "/etc/passwd")) {
            Concatenate(dirTree, cmd, 4);
            return 0;
        }
        strncpy(tmp, cmd, MAX_DIR);
        if (!strstr(cmd, "/")) {
            if (HasPermission(dirTree->current, 'w')) {
                printf("cat: '%s'파일을 만들 수 없음: 허가거부\n", dirTree->current->name);
                return -1;
            }
            tmpNode = IsExistDir(dirTree, cmd, 'd');
            tmpNode2 = IsExistDir(dirTree, cmd, 'f');
            if (!tmpNode && !tmpNode2) {
                printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", cmd);
                return -1;
            } else if (tmpNode && !tmpNode2) {
                printf("cat: '%s': 디렉터리입니다\n", cmd);
                return -1;
            } else if (tmpNode2 && HasPermission(tmpNode2, 'r')) {
                printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                return -1;
            } else Concatenate(dirTree, cmd, 1);
        } else {
            strncpy(tmp2, getDir(cmd), MAX_DIR);
            val = MovePath(dirTree, tmp2);
            if (val) {
                printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                return -1;
            }
            str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = IsExistDir(dirTree, tmp3, 'd');
            tmpNode2 = IsExistDir(dirTree, tmp3, 'f');
            if(!tmpNode && !tmpNode2) {
                printf("cat: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp3);
                dirTree->current = currentNode;
                return -1;
            } else if (tmpNode && !tmpNode2) {
                printf("cat: '%s': 디렉터리입니다\n", tmp3);
                dirTree->current = currentNode;
                return -1;
            } else if (tmpNode2 && HasPermission(tmpNode2, 'r')) {
                printf("cat: '%s'파일을 열 수 없음: 허가거부\n", tmpNode2->name);
                dirTree->current = currentNode;
                return -1;
            } else Concatenate(dirTree, tmp3, 1);
            dirTree->current = currentNode;
        }
    }
    return 1;
}