#include "main.h"

int MakeDir(DirectoryTree *dirTree, char *dirName, char type) {
    DirectoryNode *NewNode = (DirectoryNode *)malloc(sizeof(DirectoryNode));
    DirectoryNode *tmpNode = NULL;

    if (checkPermission(dirTree->current, 'w')) {
        printf("mkdir: '%s' Can not create directory: Permission denied.\n", dirName);
        free(NewNode);
        return FAIL;
    }
    if (!strcmp(dirName, ".") || !strcmp(dirName, "..")) {
        printf("mkdir: '%s' Can not create directory.\n", dirName);
        free(NewNode);
        return FAIL;
    }
    tmpNode = IsExistDir(dirTree, dirName, type);
    if (tmpNode && tmpNode->type == 'd'){
        printf("mkdir: : '%s' Can not create directory: File exists.\n", dirName);
        free(NewNode);
        return FAIL;
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
    modeToPermission(NewNode);

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
        while (tmpNode->nextSibling) tmpNode = tmpNode->nextSibling;
        tmpNode->nextSibling = NewNode;
	}
    return 0;
}

int ft_mkdir(DirectoryTree *dirTree, char *cmd) {
    DirectoryNode *tmpNode = NULL;
    char *str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int val;
    int tmpMode;
    if (!cmd) {
        printf("mkdir: Invalid option\n");
        printf("Try 'mkdir --help' for more information.\n");
        return FAIL;
    }
    tmpNode = dirTree->current;
    if (cmd[0] == '-') {
        if (!strcmp(cmd, "-p")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("mkdir: Invalid option\n");
                printf("Try 'mkdir --help' for more information.\n");
                return FAIL;
            }
            if (!strncmp(str, "/", 1)) dirTree->current = dirTree->root;
            str = strtok(str, "/");
            while (str) {
                val = Movecurrent(dirTree, str);
                if (val) {
                    MakeDir(dirTree, str, 'd');
                    Movecurrent(dirTree, str);
                }
                str = strtok(NULL, "/");
            }
            dirTree->current = tmpNode;
        } else if (!strcmp(cmd, "-m")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("mkdir: Invalid option\n");
                printf("Try 'mkdir --help' for more information.\n");
                return FAIL;
            }
            if (str[0] - '0' < 8 && str[1]-'0' <8  && str[2] - '0' < 8 && strlen(str) == 3) {
                tmpMode = atoi(str);
                str = strtok(NULL, " ");
                if (!str) {
                    printf("mkdir: mkdir: Invalid option\n");
                    printf("Try 'mkdir --help' for more information.\n");
                    return FAIL;
                }
                val = MakeDir(dirTree, str, 'd');
                if (!val) {
                    tmpNode = IsExistDir(dirTree, str, 'd');
                    tmpNode->permission.mode = tmpMode;
                    modeToPermission(tmpNode);
                }
            } else {
                printf("mkdir: 잘못된 모드: '%s'\n", str);
                printf("Try 'mkdir --help' for more information.\n");
                return -1;
            }
        } else if (!strcmp(cmd, "--help")) {
            printf("사용법: mkdir [옵션]... 디렉터리...\n");
            printf("  Create the DIRECTORY(ies), if they do not already exists.\n\n");
            printf("  Options:\n");
            printf("    -m, --mode=MODE\t set file mode (as in chmod)\n");
            printf("    -p, --parents  \t no error if existing, make parent directories as needed\n");
            printf("        --help\t 이 도움말을 표시하고 끝냅니다\n");
            return -1;
        } else {
            str = strtok(cmd, "-");
            if (!str) {
                printf("mkdir: 잘못된 연산자\n");
                printf("Try 'mkdir --help' for more information.\n");
                return -1;
            } else {
                printf("mkdir: 부적절한 옵션 -- '%s'\n", str);
                printf("Try 'mkdir --help' for more information.\n");
                return -1;
            }
        }
    } else {
        str = strtok(NULL, " ");
        if (!str) {
            strncpy(tmp, cmd, MAX_DIR);
            if (!strstr(cmd, "/")) {
                MakeDir(dirTree, cmd, 'd');
                return 0;
            } else {
                strncpy(tmp2, getDir(cmd), MAX_DIR);
                val = MovePath(dirTree, tmp2);
                if (val) {
                    printf("mkdir: '%s': 그런 파일이나 디렉터리가 없습니다\n", tmp2);
                    return -1;
                }
                str = strtok(tmp, "/");
                while (str) {
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                MakeDir(dirTree, tmp3 , 'd');
                dirTree->current = tmpNode;
            }
        } else {
            DirectoryNode *NewNode = (DirectoryNode *)malloc(sizeof(DirectoryNode));
            DirectoryNode *tmpNode2 = NULL;
            int fd[2];
            int state = 0;
            pid_t pid;

            state = pipe(fd);
            if (state == -1) {
                printf("pipe() error\n");
                return -1;
            }

            state = sem_init(&semp, 1, 1);
            write(fd[0], NewNode, sizeof(DirectoryNode));
            pid = fork();
            if (pid == 0){
                sleep(0.1);
                sem_wait(&semp);
                read(fd[1], NewNode, sizeof(DirectoryNode));
                
                time(&ltime);
                today = localtime(&ltime);

                NewNode->firstChild = NULL;
                NewNode->nextSibling = NULL;

                strncpy(NewNode->name, str, MAX_NAME);
                if (str[0] == '.') {
                    NewNode->type = 'd';
                    NewNode->permission.mode = 700;
                    NewNode->SIZE = 4096;
                } else {
                    NewNode->type = 'd';
                    NewNode->permission.mode = 755;
                    NewNode->SIZE = 4096;
                }
                modeToPermission(NewNode);
                NewNode->id.UID = usrList->current->id.UID;
                NewNode->id.GID = usrList->current->id.GID;
                NewNode->date.month = today->tm_mon + 1;
                NewNode->date.day = today->tm_mday;
                NewNode->date.hour = today->tm_hour;
                NewNode->date.minute = today->tm_min;
                NewNode->parent = NULL;

                write(fd[1], NewNode, sizeof(DirectoryNode));
                sem_post(&semp);
                exit(0);
            } else {
                MakeDir(dirTree, cmd, 'd');
                sleep(0.1);
                sem_wait(&semp);
                read(fd[0], NewNode, sizeof(DirectoryNode));
                if (checkPermission(dirTree->current, 'w')) {
                    printf("mkdir: '%s' 디렉터리를 만들 수 없습니다: 허가 거부\n", NewNode->name);
                    free(NewNode);
                    return -1;
                }
                if (!strcmp(NewNode->name, ".") || !strcmp(NewNode->name, "..")) {
                    printf("mkdir: '%s' 디렉터리를 만들 수 없습니다\n", NewNode->name);
                    free(NewNode);
                    return -1;
                }
                tmpNode = IsExistDir(dirTree, NewNode->name, 'd');
                if (tmpNode && tmpNode->type == 'd'){
                    printf("mkdir: '%s' 디렉터리를 만들 수 없습니다: 파일이 존재합니다\n", NewNode->name);
                    free(NewNode);
                    return -1;
                }

                NewNode->parent = dirTree->current;

                if (!dirTree->current->firstChild) dirTree->current->firstChild = NewNode;
                else {
                    tmpNode = dirTree->current->firstChild;
                    while (tmpNode->nextSibling) {
                        tmpNode = tmpNode->nextSibling;
                    }
                    tmpNode->nextSibling = NewNode;
                }
                sem_post(&semp);
            }
            sem_destroy(&semp);
        }
    }
    return 0;
}