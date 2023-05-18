#include "../include/main.h"

int main() {
    char command[50];

    Linux = loadDirectoryTree();
    usrList = loadUserList();
    dStack = initStack();

    login(usrList, Linux);
    printStart();
    SaveUserList(usrList);

    while (TRUE) {
        printPrompt(Linux, dStack);
        fgets(command, sizeof(command), stdin);
        command[strlen(command) - 1] = '\0';
        Instruction(Linux, command);
    }
    return 0;
}
