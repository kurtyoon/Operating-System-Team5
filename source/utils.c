#include "../include/main.h"

void printStart() {

    printf("Welcome to Ubuntu 18.04.2 LTS (GNU/Linux 4.18.0-17-generic x86_64)\n\n");
    printf(" * Documentation: https://help.ubuntu.com\n");
    printf(" * Management:    https://landscape.canonial.com\n");
    printf(" * Support:       https://ubuntu.com/advantage\n\n\n");
    printf(" * Canonial Livepatch is available for installation.\n");
    printf("   - Reduce system reboots and improve kernel security. Activate at:\n");
    printf("     https://ubuntu.com/livepatch\n\n");
    printf("Your Hardware Enablement Stack(HWE) is supported until April 2023.\n");
    printf("Last login: ");
    GetWeekday(usrList->current->date->weekday);
    GetMonth(usrList->current->date->month);
    printf("%d %02d:%02d:%02d %d\n", usrList->current->date->day, usrList->current->date->hour, usrList->current->date->minute, usrList->current->date->second, usrList->current->date->year);
}

void PrintHead(Tree *dirTree, Stack *stack) {

    TreeNode *tmpNode = NULL;
    char tmp[MAX_DIR] = "";
    char tmp2[MAX_DIR] = "";
    char usr;

    if (usrList->current == usrList->head) usr = '#';
    else usr = '$';

    BOLD; GREEN;
    printf("%s@OS-Virtualbox", usrList->current->name);
    DEFAULT;
    printf(":");
    tmpNode = dirTree->current;

    if (tmpNode == dirTree->root) {
        strcpy(tmp, "/");
    } else {
        while (tmpNode->parent) {
            push(stack, tmpNode);
            tmpNode = tmpNode->parent;
        }
        while (isEmpty(stack) == 0) {
            strcat(tmp, "/");
            strcat(tmp, pop(stack));
        }
    }

    stncpy(tmp2, tmp, strlen(usrList->current->dir));

    if (usrList->current == usrList->head) {
        BOLD; BLUE;
        printf("%s", tmp);
    } else if(strcmp(usrList->current->dir, tmp2)) {
        BOLD; BLUE;
        printf("%s", tmp);
    } else {
        tmpNode = dirTree->current;
        while (tmpNode->parent) {
            push(stack, tmpNode->data);
            tmpNode = tmpNode->parent;
        }
        pop(stack); pop(stack);
        BOLD; BLUE;
        printf("~");
        while (isEmpty(stack) == 0) {
            printf("/");
            printf("%s", pop(stack));
        }
    }
    DEFAULT;
    printf("%c ", usr);
}