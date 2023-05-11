#ifndef LINKED_TREE_H
# define LINKED_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER 512
#define MAX_LENGTH 200
#define MAX_DIR 50
#define MAX_NAME 20

typedef struct Date {
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
} Date;

typedef struct Directory {
    char name[MAX_NAME];
    char type;
    int mode;
    int permission[9];
    int SIZE;
} Directory;

typedef struct ID {
    int UID;
    int GID;
} ID;

typedef struct User {
    char name[MAX_NAME];
    char dir[MAX_DIR];
    ID *id;
    Date *date;
    User *next;
} User;

typedef struct UserList {
    ID *id;
    User *head;
    User *tail;
    User *current;
} UserList;

typedef struct TreeNode {
    int data;
    struct TreeNode *parent;
    struct TreeNode *firstChild;
    struct TreeNode *nextSibling;
    Date *date;
    Directory *dir;
    ID *id;
} TreeNode;

typedef struct Tree {
    TreeNode *root;
    TreeNode *current;
} Tree;

TreeNode *createNode(int data);
void addChild(TreeNode *parent, TreeNode *child);
void printTree(TreeNode *node, int level);
TreeNode *findParent(TreeNode *root, TreeNode *nodeToFind);
void deleteChild(TreeNode *parent, TreeNode *childToDelete);

UserList* usrList;

#endif