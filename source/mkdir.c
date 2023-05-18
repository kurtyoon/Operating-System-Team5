#include "../include/main.h"

int makeDir(DirectoryTree *dirTree, char *dirName, char type) {
    DirectoryNode *newNode = (DirectoryNode *)malloc(sizeof(DirectoryNode));
    DirectoryNode *tmpNode = NULL;

    if (checkPermission(dirTree->current, 'w')) {
        printf("mkdir: '%s' Can not create directory: Permission denied.\n", dirName);
        free(newNode);
        return FAIL;
    }
    if (!strcmp(dirName, ".") || !strcmp(dirName, "..")) {
        printf("mkdir: '%s' Can not create directory.\n", dirName);
        free(newNode);
        return FAIL;
    }
    tmpNode = dirExistence(dirTree, dirName, type);
    if (tmpNode && tmpNode->type == 'd'){
        printf("mkdir: : '%s' Can not create directory: File exists.\n", dirName);
        free(newNode);
        return FAIL;
    }
    time(&ltime);
    today = localtime(&ltime);

    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;

    strncpy(newNode->name, dirName, MAX_NAME);
    if (dirName[0] == '.') {
	    newNode->type = 'd';
        newNode->permission.mode = 700;
        newNode->SIZE = 4096;
    } else if(type == 'd'){
        newNode->type = 'd';
        newNode->permission.mode = 755;
        newNode->SIZE = 4096;
    } else {
        newNode->type = 'f';
        newNode->permission.mode = 644;
        newNode->SIZE = 0;
    }
    modeToPermission(newNode);

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
    int t_count = 0;
    pthread_t t_command[MAX_THREAD];
    ThreadTree p_threadArg[MAX_THREAD];
    tmpNode = p_directoryTree->current;
    if (command[0] == '-') {
        if (!strcmp(command, "-p")) {
            str = strtok(NULL, " ");
            if (!str) {
                printf("mkdir: Invalid option\n");
                printf("Try 'mkdir --help' for more information.\n");
                return FAIL;
            }
            while (str) {
                printf("%s : str\n", str);
                p_threadArg[t_count].threadTree = p_directoryTree;
                p_threadArg[t_count].option = 1;
                p_threadArg[t_count++].command = str;
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
    } else {
        str = strtok(NULL, " ");
        p_threadArg[t_count].threadTree = p_directoryTree;
        p_threadArg[t_count].option = 0;
        p_threadArg[t_count++].command = command;
        while (str) {
            p_threadArg[t_count].threadTree = p_directoryTree;
            p_threadArg[t_count].option = 0;
            p_threadArg[t_count++].command = str;
            str = strtok(NULL, " ");
        }
    }
    for (int i = 0; i < t_count; i++) {
        pthread_create(&t_command[i], NULL, thread_routine_make_directory, (void *)&p_threadArg[i]);
        pthread_join(t_command[i], NULL);
    }
    return SUCCESS;
}