#include "../include/main.h"

//userList에 파일 저장.
void saveUserList(UserList *userList) {
    User = fopen("file/User.txt", "w");
    writeUser(userList, userList->head);
    fclose(Dir);
}

//userList를 파일에서 로드.
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