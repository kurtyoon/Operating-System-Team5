#include "../include/main.h"

UserList *InitializeUser() {
    UserList *returnList = (UserList *)malloc(sizeof(UserList));
    UserNode *NewNode = (UserNode *)malloc(sizeof(UserNode));

    time(&ltime);
    today = localtime(&ltime);

    strncpy(NewNode->name, "root", MAX_NAME);
    strncpy(NewNode->dir, "/", MAX_NAME);
    NewNode->id.UID = 0;
    NewNode->id.GID = 0;
    NewNode->date.year = today->tm_year+1900;
    NewNode->date.month = today->tm_mon+1;
    NewNode->date.weekday = today->tm_wday;
    NewNode->date.day = today->tm_mday;
    NewNode->date.hour = today->tm_hour;
    NewNode->date.minute = today->tm_min;
    NewNode->date.second = today->tm_sec;
    NewNode->LinkNode = NULL;

    returnList->head = NewNode;
    returnList->tail = NewNode;
    returnList->current = NewNode;
    returnList->topId.UID = 0;
    returnList->topId.GID = 0;

    return returnList;
}

void WriteUser(UserList *userList, UserNode *userNode) {
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

    if (userNode->LinkNode) {
        WriteUser(userList, userNode->LinkNode);
    }

}

void SaveUserList(UserList *userList) {
    User = fopen("file/User.txt", "w");
    WriteUser(userList, userList->head);
    fclose(Dir);
}

int ReadUser(UserList *userList, char *tmp) {
    UserNode *NewNode = (UserNode *)malloc(sizeof(UserNode));
    char *str;

    NewNode->LinkNode = NULL;

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
        userList->tail->LinkNode = NewNode;
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
        if (!strcmp(returnUser->name, userName)) {
            break;
        }
        returnUser = returnUser->LinkNode;
    }
    return returnUser;
}

char *GetUID(DirectoryNode *dirNode) {
    UserNode *tmpNode = NULL;

    tmpNode = usrList->head;
    while (tmpNode) {
        if (tmpNode->id.UID == dirNode->id.UID) {
            break;
        }
        tmpNode = tmpNode->LinkNode;
    }
    return tmpNode->name;
}

char *GetGID(DirectoryNode *dirNode) {
    UserNode *tmpNode = NULL;

    tmpNode = usrList->head;
    while (tmpNode) {
        if(tmpNode->id.GID == dirNode->id.GID) {
            break;
        }
        tmpNode = tmpNode->LinkNode;
    }
    return tmpNode->name;
}

int HasPermission(DirectoryNode *dirNode, char o) {
    if (usrList->current->id.UID == 0) {
        return 0;
    }
    if (usrList->current->id.UID == dirNode->id.UID) {
        if (o == 'r') {
            if(dirNode->permission.permission[0] == 0) {
                return -1;
            }
            else {
                return 0;
            }
        }
        if (o == 'w') {
            if(dirNode->permission.permission[1] == 0) {
                return -1;
            }
            else {
                return 0;
            }
        }
        if (o == 'x') {
            if (dirNode->permission.permission[2] == 0) {
                return -1;
            }
            else {
                return 0;
            }
        }
    } else if (usrList->current->id.GID == dirNode->id.GID) {
        if (o == 'r') {
            if(dirNode->permission.permission[3] == 0) {
                return -1;
            }
            else {
                return 0;
            }
        }
        if(o == 'w'){
            if(dirNode->permission.permission[4] == 0) {
                return -1;
            }
            else {
                return 0;
            }
        }
        if(o == 'x'){
            if(dirNode->permission.permission[5] == 0) {
                return -1;
            }
            else {
                return 0;
            }
        }
    }
    else{
        if(o == 'r'){
            if(dirNode->permission.permission[6] == 0) {
                return -1;
            }
            else {
                return 0;
            }
        }
        if(o == 'w'){
            if(dirNode->permission.permission[7] == 0) {
                return -1;
            }
            else {
                return 0;
            }
        }
        if(o == 'x'){
            if(dirNode->permission.permission[8] == 0) {
                return -1;
            }
            else {
                return 0;
            }
        }
    }
    return -1;
}