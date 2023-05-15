#include "../include/main.h"

int main() {
    char command[50];

    Linux = LoadDir();
    usrList = LoadUserList();
    dStack = InitializeStack();

    Login(usrList, Linux);
    PrintStart();
    SaveUserList(usrList);

    while (TRUE) {
        PrintHead(Linux, dStack);
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = '\0';
        Instruction(Linux, command);
    }
    return 0;
}
