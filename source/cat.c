#include "../include/main.h"

int concatenate(DirectoryTree* dirTree, char* fileName, int type) {
    UserNode* tmpUser = NULL;
    DirectoryNode* tmpNode = NULL;
    FILE* fp;
    char buf[MAX_BUFFER];
    char tmpName[MAX_NAME];
    char* str;
    int tmpSIZE = 0;
    int cnt = 1;

    if (type) {
        if(type == 4) {
            tmpUser = usrList->head;
            while (tmpUser) {
                printf("%s:x:%d:%d:%s:%s\n", tmpUser->name, tmpUser->id.UID, tmpUser->id.GID, tmpUser->name, tmpUser->dir);
                tmpUser = tmpUser->nextNode;
            }
            return SUCCESS;
        }
        tmpNode = dirExistence(dirTree, fileName, 'f');
        if(!tmpNode) return FAIL;
        fp = fopen(fileName, "r");
        while (!feof(fp)) {
            fgets(buf, sizeof(buf), fp);
            if(feof(fp)) break;
            if(type == 2) {
                if (buf[strlen(buf) - 1] == '\n'){
                    printf("     %d ",cnt);
                    cnt++;
                }
            } else if(type == 3) {
                if(buf[strlen(buf) - 1] == '\n' && buf[0] != '\n'){
                    printf("     %d ",cnt);
                    cnt++;
                }
            }
            fputs(buf, stdout);
        }
        fclose(fp);
    } else {
        fp = fopen(fileName, "w");

	    while (fgets(buf, sizeof(buf), stdin)) {
            fputs(buf, fp);
            tmpSIZE += strlen(buf) - 1;
        }
        rewind(stdin);
        fclose(fp);

        tmpNode = dirExistence(dirTree, fileName, 'f');
        if (tmpNode) {
            time(&ltime);
            today = localtime(&ltime);

            tmpNode->date.month = today->tm_mon + 1;
            tmpNode->date.day = today->tm_mday;
            tmpNode->date.hour = today->tm_hour;
            tmpNode->date.minute = today->tm_min;
        } else makeDir(dirTree, fileName, 'f');
        tmpNode = dirExistence(dirTree, fileName, 'f');
        tmpNode->SIZE = tmpSIZE;
    }
    return SUCCESS;
}


int ft_cat(DirectoryTree* dirTree, char* command)
{
    DirectoryNode* currentNode = NULL;
    DirectoryNode* tmpNode = NULL;
    DirectoryNode* tmpNode2 = NULL;
    ThreadTree threadTree[MAX_THREAD];
    pthread_t threadPool[MAX_THREAD];
    int threadCount = 0;
    char* str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int value;

    if (!command) {
        long size;
	    unsigned char c;
        size = read(0, &c, 1);
	    while (size)
	    {
		    write(1, &c, size);
		    size = read(0, &c, 1);
	    }
        rewind(stdin);
        return SUCCESS;
    }
    currentNode = dirTree->current;

    if (strcmp(command, ">") == 0) {
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("cat: Invalid option\n");
            printf("Try 'cat --help' for more information.\n");
            return FAIL;
        }
        strncpy(tmp, str, MAX_DIR);
        if(!strstr(str, "/")) {
            if (checkPermission(dirTree->current, 'w')) {
                printf("cat: Can not create file '%s': No permission.\n", dirTree->current->name);
                return FAIL;
            }
            tmpNode = dirExistence(dirTree, str, 'd');
            if (tmpNode) {
                printf("cat: '%s': Is a directory\n", str);
                return FAIL;
            }
            else concatenate(dirTree, str, 0);
        } else {
            strncpy(tmp2, getDir(str), MAX_DIR);
            value = movePath(dirTree, tmp2);
            if (value) {
                printf("cat: '%s': No such file or directory\n", tmp2);
                return FAIL;
            }
            str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            if (checkPermission(dirTree->current, 'w')) {
                printf("cat: Can not create file '%s': No permission.\n", dirTree->current->name);
                dirTree->current = currentNode;
                return FAIL;
            }
            tmpNode = dirExistence(dirTree, tmp3, 'd');
            if (tmpNode) {
                printf("cat: '%s': Is a directory\n", tmp3);
                dirTree->current = currentNode;
                return FAIL;
            } else concatenate(dirTree, tmp3, 0);
            dirTree->current = currentNode;
        }
        return 0;
    } else if(command[0] == '-'){
        if (!strcmp(command, "-n")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("cat: Invalid option\n");
                printf("Try 'cat --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                threadTree[threadCount].threadTree = dirTree;
                threadTree[threadCount].option = 1;
                threadTree[threadCount++].command = str;
                str = strtok(NULL, " ");
            }
        } else if(strcmp(command, "--help") == 0) {
            printf("Usage: cat [OPTION]... [FILE]...\n");
            printf("Concatenate FILE(s) to standard output.\n\n");
            printf("With no FILE, or when FILE is -, read standard input.\n\n");
            printf("  -n, --number             number all output lines\n");
            printf("  -b, --number-nonblank    number nonempty output lines, overrides -n\n");
            printf("      --help     display this help and exit\n");
            return FAIL;
        } else {
            str = strtok(command, "-");
            if(!str) {
                printf("cat: Invalid option\n");
                printf("Try 'cat --help' for more information.\n");
                return FAIL;
            } else {
                printf("cat: Unrecognized option -- '%s'\n", str);
                printf("Try 'cat --help' for more information.\n");
                return FAIL;
            }
        }
    } else {
        if (!strcmp(command, "/etc/password")) {
            concatenate(dirTree, "password", 4);
            return SUCCESS;
        }
        if (!command) {
            printf("cat: Invalid option\n");
            printf("Try 'rm --help' for more information.\n");
            return FAIL;
        }
        str = strtok(NULL, " ");
        threadTree[threadCount].threadTree = dirTree;
        threadTree[threadCount].option = 0;
        threadTree[threadCount++].command = command;
        while (str) {
            threadTree[threadCount].threadTree = dirTree;
            threadTree[threadCount].option = 0;
            threadTree[threadCount++].command = str;
            str = strtok(NULL, " ");
        }
    }

    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, catUsedThread, (void*)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return 1;
}