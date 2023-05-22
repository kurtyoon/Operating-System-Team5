#include "../include/main.h"

//현재 date, time 기반으로 사용자 정보를 입력.
void writeUser(UserList *userList, UserNode *userNode) {
    time(&ltime);
    today = localtime(&ltime);

    //현재 시간 정보를 사용자 정보에 저장.
    userList->current->date.year = today->tm_year+1900;
    userList->current->date.month = today->tm_mon+1;
    userList->current->date.weekday = today->tm_wday;
    userList->current->date.day = today->tm_mday;
    userList->current->date.hour = today->tm_hour;
    userList->current->date.minute = today->tm_min;
    userList->current->date.second =today->tm_sec;

    //사용자 정보를 input.
    fprintf(User, "%s %d %d %d %d %d %d %d %d %d %s\n", \
    userNode->name, userNode->id.UID, userNode->id.GID, userNode->date.year, \
    userNode->date.month, userNode->date.weekday, userNode->date.day, userNode->date.hour, \
    userNode->date.minute, userNode->date.second, userNode->dir);

    //next_userNode가 있을 경우, recursion call.
    if (userNode->nextNode) writeUser(userList, userNode->nextNode);
}

//사용자 정보를 읽어 userList에 추가.
int readUser(UserList *userList, char *tmp) {
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

    //userList에 Node 추가.
    if (!strcmp(NewNode->name, "root")) {
        userList->head = NewNode;
        userList->tail = NewNode;
    } else {
        userList->tail->nextNode = NewNode;
        userList->tail = NewNode;
    }
    return SUCCESS;
}

//userList에서 해당 userName의 사용자가 존재하는지 확인.
UserNode *userExistence(UserList *userList, char *userName) {
    UserNode *returnUser = NULL;

    returnUser = userList->head;
    while (returnUser) {
        if (!strcmp(returnUser->name, userName)) break;
        returnUser = returnUser->nextNode;
    }
    return returnUser;
}

//해당 dirNode의 UID에 해당하는 userName을 return.
char *getUID(DirectoryNode *dirNode) {
    UserNode *tmpNode = NULL;

    tmpNode = usrList->head;
    while (tmpNode) {
        if (tmpNode->id.UID == dirNode->id.UID) break;
        tmpNode = tmpNode->nextNode;
    }
    return tmpNode->name;
}

//해당 dirNode의 GID에 해당하는 userName을 return.
char *getGID(DirectoryNode *dirNode) {
    UserNode *tmpNode = NULL;

    tmpNode = usrList->head;
    while (tmpNode) {
        if(tmpNode->id.GID == dirNode->id.GID) break;
        tmpNode = tmpNode->nextNode;
    }
    return tmpNode->name;
}