#include "../include/main.h"

User *IsExistUser(UserList *userList, char *userName) {

    User *returnUser = NULL;

    returnUser = userList -> head;

    while (returnUser) {
        if (strcmp(returnUser->name, userName) == 0) break;
        returnUser = returnUser->next;
    }
    return returnUser;
}

void WriteUser(UserList *userList, User *userNode) {
    time(&ltime);
    today = localtime(&ltime);

    userList->current->date->year = today->tm_yser + 1900;
    userList->current->date->month = today->tm_mon + 1;
    userList->current->date->weekday = today->tm_wday;
    userList->current->date->day = today->tm_mday;
    userList->current->date->hour = today->tm_hour;
    userList->current->date->minute = today->tm_min;
    userList->current->date->sec = today->tm_sec;

    fprintf(userFile, "%s %d %d %d %d %d %d %d %d %d %s\n", userNode->name, userNode->id->UID, userNode->id->GID, userNode->date->year, userNode->date->month, userNode->date->weekday, userNode->date->day, userNode->date->hour, userNode->date->minute, userNode->date->second)

    if (userNode->next) {
        WriteUser(userList, userNode->next);
    }
}

UserList *LoadUserList() {
    UserList *userList = (UserList *)malloc(sizeof(UserList));
    char tmp[MAX_LENGTH];

    userFile = fopen('User.txt', "r");

    while (fgets(tmp, MAX_LENGTH, userFile)) {
        ReadUser(userList, tmp);
    }
    fclose(userFile);
    userList->current = NULL;
    return userList;
}

void SaveUserList(UserList *userList) {

    userFile = fopen("User.txt", "w");

    WriteUser(userList, userList->head);
    fclose(userFile);
}

void Login(UserList *userList, Tree *dirTree) {

    User *tmpUser = NULL;
    char userName[MAX_NAME];
    char tmp[MAX_DIR];

    tmpUser = userList->head;

    printf("Username: ");
    while (tmpUser) {
        printf("%s ", tmpUser->name);
        tmpUser = tmpUser->next;
    }
    printf("\n");

    while (TRUE) {
        printf("Login as: ");
        fgets(userName, sizeof(userName), stdin);
        userName[strlen(userName) - 1] = '\0';
        if (strcmp(userName, "exit") == 0) {
            exit(0);
        }
        tmpUser = IsExistUser(userList, userName);
        if (tmpUser) {
            userList->current = tmpUser;
            break;
        }
        printf("User not found\n");
    }
    strcpy(tmp, userList->current->dir);
    MovePath(dirTree, tmp);
}