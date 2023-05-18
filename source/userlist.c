#include "../include/main.h"

UserList *initUserList() {
    UserList *returnList = (UserList *)malloc(sizeof(UserList));
    UserNode *newNode = (UserNode *)malloc(sizeof(UserNode));

    time(&ltime);
    today = localtime(&ltime);

    strncpy(newNode->name, "root", MAX_NAME);
    strncpy(newNode->dir, "/", MAX_NAME);
    newNode->id.UID = 0;
    newNode->id.GID = 0;
    newNode->date.year = today->tm_year + 1900;
    newNode->date.month = today->tm_mon + 1;
    newNode->date.weekday = today->tm_wday;
    newNode->date.day = today->tm_mday;
    newNode->date.hour = today->tm_hour;
    newNode->date.minute = today->tm_min;
    newNode->date.second = today->tm_sec;
    newNode->nextNode = NULL;

    returnList->head = newNode;
    returnList->tail = newNode;
    returnList->current = newNode;
    returnList->topId.UID = 0;
    returnList->topId.GID = 0;

    return returnList;
}

void saveUserList(UserList *userList) {
    User = fopen("file/User.txt", "w");
    writeUser(userList, userList->head);
    fclose(Dir);
}

UserList *loadUserList() {
    UserList *userList = (UserList *)malloc(sizeof(UserList));
    char tmp[MAX_LENGTH];

    User = fopen("file/User.txt", "r");
    while (fgets(tmp, MAX_LENGTH, User)) {
        readUser(userList, tmp);
    }
    fclose(User);
    userList->current = NULL;
    return userList;
}