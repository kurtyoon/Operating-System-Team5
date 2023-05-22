#include "../include/main.h"

//DirectoryTree에서 directory 생성.
int makeDir(DirectoryTree *dirTree, char *dirName, char type) {
    DirectoryNode *newNode = (DirectoryNode *)malloc(sizeof(DirectoryNode)); //동적 할당.
    DirectoryNode *tmpNode = NULL;

    //current_directory에 쓰기 권한이 있는지 확인.
    if (checkPermission(dirTree->current, 'w')) {
        printf("mkdir: '%s' Can not create directory: Permission denied.\n", dirName);
        free(newNode);
        return FAIL;
    }
    //'.', '..'인 경우 생성X
    if (!strcmp(dirName, ".") || !strcmp(dirName, "..")) {
        printf("mkdir: '%s' Can not create directory.\n", dirName);
        free(newNode);
        return FAIL;
    }
    //해당 directory가 이미 존재하는지 확인.
    tmpNode = dirExistence(dirTree, dirName, type);
    if (tmpNode && tmpNode->type == 'd'){
        printf("mkdir: : '%s' Can not create directory: File exists.\n", dirName);
        free(newNode);
        return FAIL;
    }
    //현재시간 가져오기
    time(&ltime);
    today = localtime(&ltime);

    //Child, Sibling Node 초기화
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;

    //copy dirName
    strncpy(newNode->name, dirName, MAX_NAME);
    //directory : type, permission, dirSize 설정
    //'.'로 시작하는 경우 directory 취급
    if (dirName[0] == '.') {
	    newNode->type = 'd';
        newNode->permission.mode = 700;
        newNode->SIZE = 4096;
    } else if(type == 'd'){
        newNode->type = 'd';
        newNode->permission.mode = 755;
        newNode->SIZE = 4096;
    //else, 'f'(file)로 설정    
    } else {
        newNode->type = 'f';
        newNode->permission.mode = 644;
        newNode->SIZE = 0;
    }
    modeToPermission(newNode);

    //file, directory 생성 정보 설정
    newNode->id.UID = usrList->current->id.UID;
    newNode->id.GID = usrList->current->id.GID;
    newNode->date.month = today->tm_mon + 1;
    newNode->date.day = today->tm_mday;
    newNode->date.hour = today->tm_hour;
    newNode->date.minute = today->tm_min;
	newNode->parent = dirTree->current;


	if (!dirTree->current->firstChild) dirTree->current->firstChild = newNode;
	else {
        tmpNode = dirTree->current->firstChild;
        while (tmpNode->nextSibling) tmpNode = tmpNode->nextSibling;
        tmpNode->nextSibling = newNode;
	}
    return SUCCESS;
}


int ft_mkdir(DirectoryTree *p_directoryTree, char *command) {
    DirectoryNode *tmpNode = NULL;
    char *str;
    int isDirectoryExist;
    int tmpMode;

    if (!command) {
        printf("mkdir: Invalid option\n");
        printf("Try 'mkdir --help' for more information.\n");
        return FAIL;
    }
    int threadCount = 0;
    pthread_t threadPool[MAX_THREAD];   //threadPool
    ThreadTree threadTree[MAX_THREAD];  //threadTree
    tmpNode = p_directoryTree->current; //current_directory
    if (command[0] == '-') {
        //mkdir -p : 다수 directory 생성.
        //using multithreading.
        if (!strcmp(command, "-p")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("mkdir: Invalid option\n");
                printf("Try 'mkdir --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = p_directoryTree;
                threadTree[threadCount].option = 1; //option설정.
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "--help")) {
            printf("Usage: mkdir [option]... [directory]...\n");
            printf("  Create the DIRECTORY(ies), if they do not already exists.\n\n");
            printf("  Options:\n");
            printf("    -p, --parents  \t no error if existing, make parent directories as needed\n");
            printf("      --help\t Display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if (!str) {
                printf("mkdir: Invalid option\n");
                printf("Try 'mkdir --help' for more information.\n");
                return FAIL;
            } else {
                printf("mkdir: Unrecognized option -- '%s'\n", str);
                printf("Try 'mkdir --help' for more information.\n");
                return FAIL;
            }
        }
    } else { //option이 없는 경우,
        str = strtok(NULL, " ");
        threadTree[threadCount].threadTree = p_directoryTree;
        threadTree[threadCount].option = 0;
        threadTree[threadCount++].command = command;
        while (str) {
            threadTree[threadCount].threadTree = p_directoryTree;
            threadTree[threadCount].option = 0;
            threadTree[threadCount++].command = str;
            str = strtok(NULL, " ");
        }
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, makeDirUsedThread, (void *)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return SUCCESS;
}