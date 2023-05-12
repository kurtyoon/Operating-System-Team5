#include "../include/main.h"

int main() {

    char cmd[50];

    Linux = LoadDir();
    usrList = LoadUserList();
    stack = createStack();

    Login(usrList, Linux);
    printStart();
    SaveUserList(usrList);

    while (TRUE) {
        PrintHead(Linux, stack);
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strlen(cmd) - 1] = '\0';
        Instruction(Linux, cmd);
    }
    return 0;
}