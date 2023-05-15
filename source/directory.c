#include "../include/main.h"

DirectoryNode *IsExistDir(DirectoryTree *dirTree, char *dirName, char type);
void DestroyNode(DirectoryNode *dirNode) {
    free(dirNode);
}

void DestroyDir(DirectoryNode *dirNode) {
    if (dirNode->nextSibling) {
        DestroyDir(dirNode->nextSibling);
    }
    if (dirNode->firstChild) {
        DestroyDir(dirNode->firstChild);
    }
    dirNode->firstChild = NULL;
    dirNode->nextSibling = NULL;
    DestroyNode(dirNode);
}

DirectoryNode *IsExistDir(DirectoryTree *dirTree, char *dirName, char type) {
    DirectoryNode *returnNode = NULL;

    returnNode = dirTree->current->firstChild;
    while (returnNode) {
        if(!strcmp(returnNode->name, dirName) && returnNode->type == type) break;
        returnNode = returnNode->nextSibling;
    }
    return returnNode;
}

char *getDir(char *dirPath) {
    char *tmpPath = (char *)malloc(MAX_DIR);
    char *str = NULL;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];

    strncpy(tmp, dirPath, MAX_DIR);
    str = strtok(dirPath, "/");
    while (str) {
        strncpy(tmp2, str, MAX_DIR);
        str  = strtok(NULL, "/");
    }
    strncpy(tmpPath, tmp, strlen(tmp) - strlen(tmp2) - 1);
    tmpPath[strlen(tmp) - strlen(tmp2) - 1] = '\0';
    return tmpPath;
}

void getPath(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack) {
    DirectoryNode *tmpNode = NULL;
    char tmp[MAX_DIR] = "";

    tmpNode = dirNode->parent;
    if (tmpNode == dirTree->root) {
        strcpy(tmp, "/");
    } else {
        while (tmpNode->parent) {
            Push(dirStack, tmpNode->name);
            tmpNode = tmpNode->parent;
        }
        while (!IsEmpty(dirStack)) {
                strcat(tmp, "/");
                strcat(tmp ,Pop(dirStack));
        }
    }
    fprintf(Dir, " %s\n", tmp);
}


void WriteNode(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack) {
    fprintf(Dir, "%s %c %d ", dirNode->name, dirNode->type, dirNode->permission.mode);
    fprintf(Dir, "%d %d %d %d %d %d %d", dirNode->SIZE, dirNode->id.UID, dirNode->id.GID, dirNode->date.month, dirNode->date.day, dirNode->date.hour, dirNode->date.minute);

    if(dirNode == dirTree->root) {
        fprintf(Dir, "\n");
    } else {
        getPath(dirTree, dirNode, dirStack);
    }
    if (dirNode->nextSibling) {
        WriteNode(dirTree, dirNode->nextSibling, dirStack);
    }
    if (dirNode->firstChild) {
        WriteNode(dirTree, dirNode->firstChild, dirStack);
    }
}

void SaveDir(DirectoryTree *dirTree, Stack *dirStack) {
    Dir = fopen("file/Directory.txt", "w");
    WriteNode(dirTree, dirTree->root, dirStack);
    fclose(Dir);
}

int ReadNode(DirectoryTree *dirTree, char *tmp) {
    DirectoryNode *NewNode = (DirectoryNode *)malloc(sizeof(DirectoryNode));
    DirectoryNode *tmpNode = NULL;
    char *str;

    NewNode->firstChild = NULL;
    NewNode->nextSibling = NULL;
    NewNode->parent = NULL;

    str = strtok(tmp, " ");
    strncpy(NewNode->name, str, MAX_NAME);
    str = strtok(NULL, " ");
    NewNode->type = str[0];
    str = strtok(NULL, " ");
    NewNode->permission.mode = atoi(str);
    Mode2Permission(NewNode);
    str = strtok(NULL, " ");
    NewNode->SIZE = atoi(str);
    str = strtok(NULL, " ");
    NewNode->id.UID = atoi(str);
    str = strtok(NULL, " ");
    NewNode->id.GID = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.month = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.day = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.hour = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.minute = atoi(str);

    str = strtok(NULL, " ");
    if (str) {
        str[strlen(str) - 1] = '\0';
        MovePath(dirTree, str);
        NewNode->parent = dirTree->current;
        if (!dirTree->current->firstChild) {
            dirTree->current->firstChild = NewNode;
        } else {
            tmpNode = dirTree->current->firstChild;
            while (tmpNode->nextSibling) {
                tmpNode = tmpNode->nextSibling;
            }
            tmpNode->nextSibling = NewNode;
        }
    } else {
        dirTree->root = NewNode;
        dirTree->current = dirTree->root;
    }
    return 0;
}

DirectoryTree *LoadDir() {
    DirectoryTree *dirTree = (DirectoryTree *)malloc(sizeof(DirectoryTree));
    char tmp[MAX_LENGTH];

    Dir = fopen("file/Directory.txt", "r");
    while (fgets(tmp, MAX_LENGTH, Dir)) {
        ReadNode(dirTree, tmp);
    }
    fclose(Dir);
    dirTree->current = dirTree->root;
    return dirTree;
}

DirectoryTree *InitializeTree() {
    DirectoryTree *dirTree = (DirectoryTree *)malloc(sizeof(DirectoryTree));
    DirectoryNode *NewNode = (DirectoryNode *)malloc(sizeof(DirectoryNode));

    time(&ltime);
    today = localtime(&ltime);

    strncpy(NewNode->name, "/", MAX_NAME);
    NewNode->type ='d';
    NewNode->permission.mode = 755;
    Mode2Permission(NewNode);
    NewNode->id.UID = usrList->head->id.UID;
    NewNode->id.GID = usrList->head->id.GID;
    NewNode->SIZE = 4096;
    NewNode->date.month = today->tm_mon+1;
    NewNode->date.day = today->tm_mday;
    NewNode->date.hour = today->tm_hour;
    NewNode->date.minute = today->tm_min;
    NewNode->parent = NULL;
    NewNode->firstChild = NULL;
    NewNode->nextSibling = NULL;

    dirTree->root = NewNode;
    dirTree->current = dirTree->root;

    return dirTree;
}

int MakeDir(DirectoryTree *dirTree, char *dirName, char type) {
    DirectoryNode *NewNode = (DirectoryNode *)malloc(sizeof(DirectoryNode));
    DirectoryNode *tmpNode = NULL;

    if (HasPermission(dirTree->current, 'w')) {
        printf("mkdir: '%s' 디렉터리를 만들 수 없습니다: 허가 거부\n", dirName);
        free(NewNode);
        return -1;
    }
    if (!strcmp(dirName, ".") || !strcmp(dirName, "..")) {
        printf("mkdir: '%s' 디렉터리를 만들 수 없습니다\n", dirName);
        free(NewNode);
        return -1;
    }
    tmpNode = IsExistDir(dirTree, dirName, type);
    if (tmpNode && tmpNode->type == 'd'){
        printf("mkdir: '%s' 디렉터리를 만들 수 없습니다: 파일이 존재합니다\n", dirName);
        free(NewNode);
        return -1;
    }
    time(&ltime);
    today = localtime(&ltime);

    NewNode->firstChild = NULL;
    NewNode->nextSibling = NULL;

    strncpy(NewNode->name, dirName, MAX_NAME);
    if (dirName[0] == '.') {
	    NewNode->type = 'd';
        NewNode->permission.mode = 700;
        NewNode->SIZE = 4096;
    } else if(type == 'd'){
        NewNode->type = 'd';
        NewNode->permission.mode = 755;
        NewNode->SIZE = 4096;
    } else {
        NewNode->type = 'f';
        NewNode->permission.mode = 644;
        NewNode->SIZE = 0;
    }
    Mode2Permission(NewNode);

    NewNode->id.UID = usrList->current->id.UID;
    NewNode->id.GID = usrList->current->id.GID;
    NewNode->date.month = today->tm_mon + 1;
    NewNode->date.day = today->tm_mday;
    NewNode->date.hour = today->tm_hour;
    NewNode->date.minute = today->tm_min;
	NewNode->parent = dirTree->current;

	if (!dirTree->current->firstChild) dirTree->current->firstChild = NewNode;
	else {
        tmpNode = dirTree->current->firstChild;
        while (tmpNode->nextSibling) {
            tmpNode = tmpNode->nextSibling;
        }
        tmpNode->nextSibling = NewNode;
	}
    return 0;
}

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

int PrintPath(DirectoryTree *dirTree, Stack *dirStack) {
    DirectoryNode *tmpNode = NULL;

    tmpNode = dirTree->current;

    if (tmpNode == dirTree->root) printf("/");
    else {
        while (tmpNode->parent) {
            Push(dirStack, tmpNode->name);
            tmpNode = tmpNode->parent;
        }
        while (!IsEmpty(dirStack)) {
            printf("/");
            printf("%s",Pop(dirStack));
        }
    }
    printf("\n");
    return 0;
}

int ListDir(DirectoryTree *dirTree, int a, int l) {
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    char type;
    int cnt;

    tmpNode = dirTree->current->firstChild;

    if (HasPermission(dirTree->current, 'r')) {
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
            PrintPermission(dirTree->current);
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
                PrintPermission(dirTree->current->parent);
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
            PrintPermission(tmpNode);
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

int ChangeMode(DirectoryTree *dirTree, int mode, char *dirName) {
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;

    tmpNode = IsExistDir(dirTree, dirName, 'd');
    tmpNode2 = IsExistDir(dirTree, dirName, 'f');

    if (tmpNode) {
        if (HasPermission(tmpNode, 'w')) {
            printf("chmod: '%s'파일을 수정할 수 없음: 허가거부\n", dirName);
            return -1;
        }
        tmpNode->permission.mode = mode;
        Mode2Permission(tmpNode);
    } else if (tmpNode2) {
        if (HasPermission(tmpNode2, 'w')) {
            printf("chmod: '%s'파일을 수정할 수 없음: 허가거부\n", dirName);
            return -1;
        }
        tmpNode2->permission.mode = mode;
        Mode2Permission(tmpNode2);
    } else {
        printf("chmod: '%s에 접근할 수 없습니다: 그런 파일이나 디렉터리가 없습니다\n", dirName);
        return -1;
    }
    return 0;
}

void ChangeModeAll(DirectoryNode *dirNode, int mode) {
    if (dirNode->nextSibling) {
        ChangeModeAll(dirNode->nextSibling, mode);
    }
    if (dirNode->firstChild) {
        ChangeModeAll(dirNode->firstChild, mode);
    }
    dirNode->permission.mode = mode;
    Mode2Permission(dirNode);
}

int ChangeOwner(DirectoryTree *dirTree, char *userName, char *dirName) {
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    UserNode *tmpUser = NULL;

    tmpNode = IsExistDir(dirTree, dirName, 'd');
    tmpNode2 = IsExistDir(dirTree, dirName, 'f');


    if (tmpNode) {
        if (HasPermission(tmpNode, 'w')) {
            printf("chown: '%s'파일을 수정할 수 없음: 허가거부\n", dirName);
            return -1;
        }
        tmpUser = IsExistUser(usrList, userName);
        if (tmpUser) {
            tmpNode->id.UID = tmpUser->id.UID;
            tmpNode->id.GID = tmpUser->id.GID;
        } else {
            printf("chown: 잘못된 사용자: '%s'\n", userName);
            printf("Try 'chown --help' for more information.\n");
            return -1;
        }
    } else if (tmpNode2) {
        if (HasPermission(tmpNode2, 'w')) {
            printf("chown: '%s'파일을 수정할 수 없음: 허가거부\n", dirName);
            return -1;
        }
        tmpUser = IsExistUser(usrList, userName);
        if (tmpUser) {
            tmpNode2->id.UID = tmpUser->id.UID;
            tmpNode2->id.GID = tmpUser->id.GID;
        } else {
            printf("chown: 잘못된 사용자: '%s'\n", userName);
            printf("Try 'chown --help' for more information.\n");
            return -1;
        }
    } else {
        printf("chown: '%s'에 접근할 수 없습니다: 그런 파일이나 디렉터리가 없습니다\n", dirName);
        return -1;
    }
    return 0;
}

void ChangeOwnerAll(DirectoryNode *dirNode, char *userName) {
    UserNode *tmpUser = NULL;

    tmpUser = IsExistUser(usrList, userName);
    if (dirNode->nextSibling) {
        ChangeOwnerAll(dirNode->nextSibling, userName);
    }
    if (dirNode->firstChild) {
        ChangeOwnerAll(dirNode->firstChild, userName);
    }
    dirNode->id.UID = tmpUser->id.UID;
    dirNode->id.GID = tmpUser->id.GID;
}

int ReadDir(DirectoryTree *dirTree, char *tmp, char *dirName, int o) {
    char *str;
    char str2[MAX_NAME];
    if (o == 0) {
        str = strtok(tmp, " ");
        strcpy(str2, str);
        for (int i = 0; i < 10; i++) {
            str = strtok(NULL, " ");
        }
        if (str) {
            if (strstr(str2, dirName)) {
                str[strlen(str) - 1] = '\0';
                if (!strcmp(str, "/")) {
                    printf("/%s\n", str2);
                }
                else {
                    printf("%s/%s\n", str, str2);
                }
            }
        }
    } else {
        str = strtok(tmp, " ");
        strcpy(str2, str);
        for (int i = 0; i < 10; i++) {
            str = strtok(NULL, " ");
        }
        if (str) {
            if (strstr(str, dirName)) {
                str[strlen(str) - 1] = '\0';
                if (!strcmp(str, "/")) {
                    printf("/%s\n", str2);
                }   
                else {
                    printf("%s/%s\n", str, str2);
                }
            }
        }
    }
    return 0;
}

void FindDir(DirectoryTree *dirTree, char *dirName, int o) {
    char tmp[MAX_LENGTH];

    Dir = fopen("file/Directory.txt", "r");
    while (fgets(tmp, MAX_LENGTH, Dir)) {
        ReadDir(dirTree, tmp, dirName, o);
    }
    fclose(Dir);
}
