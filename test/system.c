#include "main.h"

void PrintStart() {
    printf("Welcome to Ubuntu 22.04.2 LTS (GNU/Linux 5.15.0-1031-aws x86_64)\n\n");
    printf(" * Documentation:  https://help.ubuntu.com\n");
    printf(" * Management:     https://landscape.canonical.com\n");
    printf(" * Support:        https://ubuntu.com/advantage\n\n");
    printf("  System information as of ");
    // GetWeekday(today->tm_wday);
    // GetMonth(today->tm_mon);
    // printf("%d %02d:%02d:%02d UTC %d\n\n", today->tm_mday, today->tm_hour, today->tm_min, today->tm_sec, today->tm_year + 1900);
    printf("  System load:  0.01708984375     Processes:             96\n");
    printf("  Usage of /:   20.4%% of 7.57GB   Users logged in:       0\n");
    printf("  Memory usage: 24%%               IPv4 address for eth0: 172.31.44.72\n");
    printf("  Swap usage:   0%%\n\n\n");
    printf(" * Introducing Expanded Security Maintenance for Applications.\n");
    printf("   Receive updates to over 25,000 software packages with your\n");
    printf("   Ubuntu Pro subscription. Free for personal use.\n\n");
    printf("     https://ubuntu.com/aws/pro\n\n");
    printf("Expanded Security Maintenance for Applications is not enabled.\n\n");
    printf("0 updates can be applied immediately.\n\n");
    printf("Enable ESM Apps to receive additional future security updates.\n");
    printf("See https://ubuntu.com/esm or run: sudo pro status\n\n\n");
    printf("Last login: ");
    GetWeekday(usrList->current->date.weekday);
    GetMonth(usrList->current->date.month);
    printf("%d %02d:%02d:%02d %d\n", usrList->current->date.day, usrList->current->date.hour, usrList->current->date.minute, usrList->current->date.second, usrList->current->date.year);
}

void PrintHead(DirectoryTree *dirTree, Stack *dirStack) {
    DirectoryNode *tmpNode = NULL;
    char tmp[MAX_DIR] = "";
    char tmp2[MAX_DIR] = "";
    char usr;

    if (usrList->current == usrList->head) {
        usr = '#';
        BOLD;
        printf("%s@Operating-System-Team5", usrList->current->name);
        DEFAULT;
        printf(":");
    }
    else {
        usr = '$';
        BOLD;GREEN;
        printf("%s@Operating-System-Team5", usrList->current->name);
        DEFAULT;
        printf(":");
    }
    tmpNode = dirTree->current;

    if(tmpNode == dirTree->root){
        strcpy(tmp, "/");
    } else {
        while (tmpNode->parent) {
            pushStack(dirStack, tmpNode->name);
            tmpNode = tmpNode->parent;
        }
        while (isEmpty(dirStack) == FALSE) {
            strcat(tmp, "/");
            strcat(tmp ,popStack(dirStack));
        }
    }
    strncpy(tmp2, tmp, strlen(usrList->current->dir));
    if (usrList->current == usrList->head){
        BOLD;BLUE;
        printf("%s", tmp);
    } else if (strcmp(usrList->current->dir, tmp2)) {
	    BOLD;BLUE;
        printf("%s", tmp);
    } else {
        tmpNode = dirTree->current;
        while (tmpNode->parent) {
            pushStack(dirStack, tmpNode->name);
            tmpNode = tmpNode->parent;
        }
        popStack(dirStack);
        popStack(dirStack);
	    BOLD;BLUE;
        printf("~");
        while (isEmpty(dirStack) == FALSE) {
            printf("/");
            printf("%s",popStack(dirStack));
        }
    }
    DEFAULT;
    printf("%c ", usr);
}

void Login(UserList *userList, DirectoryTree *dirTree) {
    UserNode *tmpUser = NULL;
    char userName[MAX_NAME];
    char tmp[MAX_DIR];

    tmpUser = userList->head;

    printf("Users: ");
    while (tmpUser) {
        printf("%s ", tmpUser->name);
        tmpUser = tmpUser->nextNode;
    }
    printf("\n");
    while (TRUE) {
        printf("Login as: ");
        fgets(userName, sizeof(userName), stdin);
        userName[strlen(userName) - 1] = '\0';
        if (!strcmp(userName, "exit")) {
            exit(0);
        }
        tmpUser = IsExistUser(userList, userName);
        if (tmpUser) {
            userList->current = tmpUser;
            break;
        }
        printf("'%s' 유저가 존재하지 않습니다\n", userName);
    }
    strcpy(tmp, userList->current->dir);
    MovePath(dirTree, tmp);
}