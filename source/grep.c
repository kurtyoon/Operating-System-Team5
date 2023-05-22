#include "../include/main.h"

int printContent(DirectoryTree *dirtree, char *search, char *fileName, int option) {
    DirectoryNode *tmpNode = NULL;
    FILE *fp;
    char buf[MAX_BUFFER];
    int count = 1;

    //주어진 파일 이름을 가진 파일 존재하는지 확인.
    tmpNode = dirExistence(dirtree, fileName, 'f');
    if (!tmpNode) return FAIL;
    
    //파일을 읽기모드로 open.
    fp = fopen(fileName, "r");
    while (!feof(fp)) {
        fgets(buf, sizeof(buf), fp);
        if (feof(fp)) break;
        else if (option == 0) {
            // 검색어가 포함된 줄을 출력.
            if (strstr(buf, search)) printf("%s", buf);
        } else if (option == 1) {
            // 줄 번호와 함께 검색어가 포함된 줄을 출력.
            if (strstr(buf, search)) printf("%d:%s", count, buf);
        } else if (option == 2) {
            // 검색어가 포함되지 않은 줄을 출력.
            if (!strstr(buf, search)) printf("%s", buf);
        } else if (option == 3) {
            // 대소문자를 구분하지 않고 검색어가 포함된 줄을 출력.
            if (strcasestr(buf, search)) printf("%s", buf);
        } else if (option == 4) {
            // 줄 번호와 함께 검색어가 포함되지 않은 줄을 출력.
            if (!strstr(buf, search)) printf("%d:%s", count, buf);
        } else if (option == 5) {
            // 줄 번호와 함께 대소문자를 구분하지 않고 검색어가 포함된 줄을 출력.
            if (strcasestr(buf, search)) printf("%d:%s", count, buf);
        } else if (option == 6) {
            // 대소문자를 구분하지 않고 검색어가 포함되지 않은 줄을 출력.
            if (!strcasestr(buf, search)) printf("%s", buf);
        } else if (option == 7) {
            // 줄 번호와 함께 대소문자를 구분하지 않고 검색어가 포함되지 않은 줄을 출력.
            if (!strcasestr(buf, search)) printf("%d:%s", count, buf);
        }
        count++;
    }
    fclose(fp);
    return SUCCESS;
}

//아래의 option들은 printContent()의 option 1~7에 순서대로 해당됨.
//또한 각각 multithreading 방식으로 작동.

int ft_grep(DirectoryTree *dirTree, char *command) {
    DirectoryNode *currentNode = dirTree->current;
    DirectoryNode *tmpNode = NULL;
    DirectoryNode *tmpNode2 = NULL;
    ThreadTree threadTree[MAX_THREAD];
    pthread_t threadPool[MAX_THREAD];
    int threadCount = 0;
    char *str;
    char content[MAX_BUFFER];
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int value;

    //command == NULL, print guideline.
    if (!command) {
        printf("Try 'grep --help' for more information.\n");
        return FAIL;
    }
    //if option exists,
    if (command[0] == '-') {
        //option : -n
        if (!strcmp(command, "-n")) {
            str = strtok(NULL, " ");
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 1;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        //option : -v
        } else if (!strcmp(command, "-v")) {
            str = strtok(NULL, " ");
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 2;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        //option : -i  
        } else if (!strcmp(command, "-i")) {
            str = strtok(NULL, " ");
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 3;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        //option : -nv or -vn        
        } else if (!strcmp(command, "-nv") || !strcmp(command, "-vn")) {
            str = strtok(NULL, " ");
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 4;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        //option : -ni or -in
        } else if (!strcmp(command, "-ni") || !strcmp(command, "-in")) {
            str = strtok(NULL, " ");
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 5;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        //option : -vi or -iv            
        } else if(!strcmp(command, "-vi") || !strcmp(command, "-iv")) {
            str = strtok(NULL, " ");
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 6;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if (!strcmp(command, "-inv") || !strcmp(command, "-ivn") || !strcmp(command, "-niv") || !strcmp(command, "-nvi") || !strcmp(command, "-vin") || !strcmp(command, "-vni")) {
            str = strtok(NULL, " ");
            strncpy(content, str, MAX_BUFFER);
            str = strtok(NULL, " ");
            if (!str) {
                printf("Try 'grep --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 7;
                threadTree[threadCount].content = content;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        //--help ; print guideline    
        } else if (!strcmp(command, "--help")) {
            printf("Usage: grep [<option>]... [<file>]...\n");
            printf("  FILE(들)을 합쳐서 표준 출력으로 보낸다.\n\n");
            printf("  Options:\n");
            printf("    -n, —number         \t number all output line\n");
            printf("        —help\t 이 도움말을 표시하고 끝냅니다\n");
            return FAIL;
        } else {
            printf("Try 'grep --help' for more information.\n");
            return FAIL;
        }
    } else {
        strncpy(content, command, MAX_BUFFER);
        str = strtok(NULL, " ");
        if (!str) {
            printf("Try 'grep --help' for more information.\n");
            return FAIL;
        }
        while (str) {
            threadTree[threadCount].threadTree = dirTree;
            threadTree[threadCount].option = 0;
            threadTree[threadCount].content = content;
            threadTree[threadCount++].command = str;
            str = strtok(NULL, " ");
        }
    }
    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, grepUsedThread, (void *)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return SUCCESS;
} 