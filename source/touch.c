#include "../include/main.h"

//touch 
//파일의 생성과  날짜, 시간을 변경.
int ft_touch(DirectoryTree *dirTree, char *command) {
    DirectoryNode *currentNode = dirTree->current;
    ThreadTree threadTree[MAX_THREAD]; 
    pthread_t threadPool[MAX_THREAD];
    int threadCount = 0;    //생성된 thread 수
    char *str;
    int value;
    int mode;

    //command == NULL 예외처리.
    if (!command) {
        printf("touch: touch: Invalid option\n");
        printf("Try 'touch --help' for more information.\n");
        return FAIL;
    }
    // command가 '-'로 시작하는 경우 옵션 처리.
    if (command[0] == '-') {
        if (!strcmp(command, "--help")) {
            printf("Usage: touch [OPTION]... FILE...\n");
            printf("Update the access and modification times of each FILE to the current time.\n\n\n");
            printf("A FILE argument string of - is handled specially and causes touch to\n");
            printf("change the times of the file associated with standard output.\n\n");
            printf("Mandatory arguments to long options are mandatory for short options too.\n");
            printf("      --help     display this help and exit");
        } else {
            // 잘못된 옵션인 경우 예외 처리.
            str = strtok(command, "-");
            if (!str) {
                printf("touch: Invalid option\n");
                printf("Try 'touch --help' for more information.\n");
                return FAIL;
            } else {
                printf("touch: invalid option -- '%s'\n", str);
                printf("Try 'touch --help' for more information.\n");
                return FAIL;
            }
        }
    } else {
        if (!command) {
            printf("touch: missing file operand\n");
            printf("Try 'touch --help' for more information.\n");
            return FAIL;
        }
        //ThreadTree에 command 추가.
        threadTree[threadCount].threadTree = dirTree;
        threadTree[threadCount++].command = command;

        //추가사항 처리.
        str = strtok(NULL, " ");
        while (str) {
            threadTree[threadCount].threadTree = dirTree;
            threadTree[threadCount++].command = str;
            str = strtok(NULL, " ");
        }
    }
    //Thread 생성 및 실행.
    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, touchUsedThread, (void *)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return SUCCESS;
}