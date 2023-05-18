#include "../include/main.h"

UserList *initUser() {
    UserList *returnList = (UserList *)malloc(sizeof(UserList));
    UserNode *newNode = (UserNode *)malloc(sizeof(UserNode));

    time(&ltime);
    today = localtime(&ltime);

    strncpy(newNode->name, "root", MAX_NAME);
    strncpy(newNode->dir, "/", MAX_NAME);
    newNode->id.UID = 0;
    newNode->id.GID = 0;
    newNode->date.year = today->tm_year+1900;
    newNode->date.month = today->tm_mon+1;
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

void writeUser(UserList *userList, UserNode *userNode) {
    time(&ltime);
    today = localtime(&ltime);

    userList->current->date.year = today->tm_year+1900;
    userList->current->date.month = today->tm_mon+1;
    userList->current->date.weekday = today->tm_wday;
    userList->current->date.day = today->tm_mday;
    userList->current->date.hour = today->tm_hour;
    userList->current->date.minute = today->tm_min;
    userList->current->date.second =today->tm_sec;

    fprintf(User, "%s %d %d %d %d %d %d %d %d %d %s\n", userNode->name, userNode->id.UID, userNode->id.GID, userNode->date.year, userNode->date.month, userNode->date.weekday, userNode->date.day, userNode->date.hour, userNode->date.minute, userNode->date.second, userNode->dir);

    if (userNode->nextNode) {
        writeUser(userList, userNode->nextNode);
    }

}

void SaveUserList(UserList *userList) {
    User = fopen("file/User.txt", "w");
    writeUser(userList, userList->head);
    fclose(Dir);
}

int ReadUser(UserList *userList, char *tmp) {
    UserNode *NewNode = (UserNode *)malloc(sizeof(UserNode));
    char *str;

    NewNode->nextNode = NULL;

    str = strtok(tmp, " ");
    strncpy(NewNode->name, str, MAX_NAME);
    str = strtok(NULL, " ");
    NewNode->id.UID = atoi(str);
    str = strtok(NULL, " ");
    NewNode->id.GID = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.year = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.month = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.weekday = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.day = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.hour = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.minute = atoi(str);
    str = strtok(NULL, " ");
    NewNode->date.second = atoi(str);
    str = strtok(NULL, " ");
    str[strlen(str)-1] = '\0';
    strncpy(NewNode->dir, str, MAX_DIR);

    if (strcmp(NewNode->name, "root") == 0) {
        userList->head = NewNode;
        userList->tail = NewNode;
    } else {
        userList->tail->nextNode = NewNode;
        userList->tail = NewNode;
    }
    return 0;
}

UserList *LoadUserList() {
    UserList *userList = (UserList *)malloc(sizeof(UserList));
    char tmp[MAX_LENGTH];

    User = fopen("file/User.txt", "r");
    while (fgets(tmp, MAX_LENGTH, User)) {
        ReadUser(userList, tmp);
    }
    fclose(User);
    userList->current = NULL;
    return userList;
}

UserNode *IsExistUser(UserList *userList, char *userName) {
    UserNode *returnUser = NULL;

    returnUser = userList->head;
    while (returnUser) {
        if (!strcmp(returnUser->name, userName)) break;
        returnUser = returnUser->nextNode;
    }
    return returnUser;
}

char *GetUID(DirectoryNode *dirNode) {
    UserNode *tmpNode = NULL;

    tmpNode = usrList->head;
    while (tmpNode) {
        if (tmpNode->id.UID == dirNode->id.UID) break;
        tmpNode = tmpNode->nextNode;
    }
    return tmpNode->name;
}

char *GetGID(DirectoryNode *dirNode) {
    UserNode *tmpNode = NULL;

    tmpNode = usrList->head;
    while (tmpNode) {
        if(tmpNode->id.GID == dirNode->id.GID) break;
        tmpNode = tmpNode->nextNode;
    }
    return tmpNode->name;
}