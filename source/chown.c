#include "../include/main.h"

void handleColonCase(DirectoryTree *dirTree, char *tmp, char *command) {
    char tmp2[MAX_NAME];
    strncpy(tmp2, tmp, MAX_NAME);
    char *str2 = strtok(tmp, ":");
    if (str2 && strcmp(tmp, tmp2)) {
        changeOwner(dirTree, str2, command, 0);
        str2 = strtok(NULL, " ");
        if (str2) changeOwner(dirTree, str2, command, 1);
    }
    else if (str2 && !strcmp(tmp, tmp2)) changeOwner(dirTree, str2, command, 1);
}

int changeOwner(DirectoryTree *dirTree, char *userName, char *dirName, int flag) {
    DirectoryNode *dirNode = dirExistence(dirTree, dirName, 'd');
    DirectoryNode *fileNode = dirExistence(dirTree, dirName, 'f');
    UserNode *tmpUser = userExistence(usrList, userName);

    if (dirNode) 
        return handleDirectoryNode(dirNode, userName, dirName, flag);
    else if (fileNode) 
        return handleFileNode(fileNode, userName, dirName, flag);
    else 
        return handleNonExistentNode(dirName);
}

int handleDirectoryNode(DirectoryNode *dirNode, char *userName, char *dirName, int flag) {
    UserNode *tmpUser = userExistence(usrList, userName);
    if (checkPermission(dirNode, 'w')) 
        return failPermissionDenied(dirName);
    else if (tmpUser) 
        return changeOwnerID(dirNode, tmpUser, flag);
    else 
        return failInvalidUser(userName);
}

int handleFileNode(DirectoryNode *fileNode, char *userName, char *dirName, int flag) {
    UserNode *tmpUser = userExistence(usrList, userName);
    if (checkPermission(fileNode, 'w')) 
        return failPermissionDenied(dirName);
    else if (tmpUser) 
        return changeOwnerID(fileNode, tmpUser, flag);
    else 
        return failInvalidUser(userName);
}

int handleNonExistentNode(char *dirName) {
    printf("chown: cannot access '%s': No such file or directory\n", dirName);
    return FAIL;
}

int failPermissionDenied(char *dirName) {
    printf("chown: Can not modify file '%s': Permission denied\n", dirName);
    return FAIL;
}

int failInvalidUser(char *userName) {
    printf("chown: Invalid user: '%s'\n", userName);
    printf("Try 'chown --help' for more information.\n");
    return FAIL;
}

int changeOwnerID(DirectoryNode *node, UserNode *tmpUser, int flag) {
    if (!flag) 
        node->id.UID = tmpUser->id.UID;
    else 
        node->id.GID = tmpUser->id.GID;
    return SUCCESS;
}

int failInvalidOption() {
    printf("chown: Invalid option\n");
    printf("Try 'chown --help' for more information.\n");
    return -1;
}

int handleDashOption(char* command) {
    if(!strcmp(command, "--help")) 
        return displayHelp();
    else 
        return handleNonHelpOption(command);
}

int displayHelp() {
    printf("Usage: chown [OPTION]... [OWNER] FILE...\n");
    printf("Change the owner of each FILE to OWNER.\n\n");
    printf("  Options:\n");
    printf("  -R, --recursive        operate on files and directories recursively\n");
    printf("      --help     display this help and exit\n");
    return FAIL;
}

int handleNonHelpOption(char* command) {
    char *str = strtok(command, "-");
    if (!str) 
        return failInvalidOption();
    else 
        return failUnrecognizedOption(str);
}

int failUnrecognizedOption(char *str) {
    printf("chown: Unrecognized option -- '%s'\n", str);
    printf("Try 'chown --help' for more information.\n");
    return FAIL;
}

int handleNormalOption(DirectoryTree* dirTree, char* command, char *tmp, ThreadTree *threadTree, int threadCount, pthread_t *threadPool) {
    strncpy(tmp, command, MAX_NAME);
    char *str = strtok(NULL, " ");
    if (!str) 
        return failInvalidOption();
    else 
        return handleMultipleCommands(dirTree, tmp, str, threadTree, threadCount, threadPool);
}

int handleMultipleCommands(DirectoryTree* dirTree, char *tmp, char *str, ThreadTree *threadTree, int threadCount, pthread_t *threadPool) {
    assignThreadData(threadTree, dirTree, tmp, str, threadCount);
    while (str) {
        assignThreadData(threadTree, dirTree, tmp, str, threadCount++);
        str = strtok(NULL, " ");
    }
    return executeThreads(threadCount, threadPool, threadTree);
}

void assignThreadData(ThreadTree *threadTree, DirectoryTree* dirTree, char *tmp, char *str, int threadCount) {
    threadTree[threadCount].threadTree = dirTree;
    threadTree[threadCount].usrName = tmp;
    threadTree[threadCount].command = str;
}

int executeThreads(int threadCount, pthread_t *threadPool, ThreadTree *threadTree) {
    for (int i = 0; i < threadCount; i++) {
        pthread_create(&threadPool[i], NULL, chownUsedThread, (void*)&threadTree[i]);
        pthread_join(threadPool[i], NULL);
    }
    return SUCCESS;
}

int ft_chown(DirectoryTree* dirTree, char* command) {
    DirectoryNode* tmpNode = NULL;
    ThreadTree threadTree[MAX_THREAD];
    pthread_t threadPool[MAX_THREAD];
    char tmp[MAX_NAME];
    int threadCount = 0;
    UserNode* tmpUser = NULL;
    char *str;

    if (!command) 
        return failInvalidOption();
    else if (command[0] == '-') 
        return handleDashOption(command);
    else 
        return handleNormalOption(dirTree, command, tmp, threadTree, threadCount, threadPool);
}