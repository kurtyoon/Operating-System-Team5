#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#ifndef MAIN_H_
#define MAIN_H_
#define DEFAULT printf("%c[%dm", 0x1B, 0)
#define BOLD printf("%c[%dm", 0x1B, 1)
#define WHITE printf("\x1b[37m")
#define BLUE printf("\x1b[34m")
#define GREEN printf("\x1b[32m")

#define MAX_BUFFER 512
#define MAX_LENGTH 200
#define MAX_DIR 50
#define MAX_NAME 20
#define MAX_THREAD 50
#define TRUE 1
#define FALSE 0
#define SUCCESS 0
#define FAIL -1

typedef int boolean;

// Date
typedef struct s_date {
    int year;
    int month;
    int weekday;
    int day;
    int hour;
    int minute;
    int second;
} Date;

// ID
typedef struct s_id {
    int UID;
    int GID;
} ID;

// Permission
typedef struct s_permission {
    int mode;
    int permission[9];
} Permission;

// UserNode
typedef struct userNode {
    char name[MAX_NAME];
    char dir[MAX_DIR];
    ID id;
    Date date;
    struct userNode *nextNode;
} UserNode;

// UserList
typedef struct userList {
    ID topId;
    UserNode *head;
    UserNode *tail;
    UserNode *current;
} UserList;

// DirectoryNode
typedef struct directoryNode {
    char name[MAX_NAME];
    char type;
    int SIZE;
    Permission permission;
    ID id;
	Date date;
	struct directoryNode *parent;
	struct directoryNode *firstChild;
	struct directoryNode *nextSibling;
} DirectoryNode;

// DirectoryTree
typedef struct directoryTree {
	DirectoryNode* root;
	DirectoryNode* current;
} DirectoryTree;

// StackNode
typedef struct stackNode {
	char name[MAX_NAME];
	struct stackNode *nextNode;
} StackNode;

// Stack
typedef struct stack {
	StackNode* topNode;
	int cnt;
} Stack;

typedef struct threadTree {
    DirectoryTree *threadTree;
    DirectoryTree *fileName;
    char *command;
    char *copyPath;
    char *additionalValue;
} ThreadTree;

//time
time_t ltime;
struct tm *today;

// user.c
UserList *initUser();
void writeUser(UserList *userList, UserNode *userNode);
void SaveUserList(UserList *userList);
int ReadUser(UserList *userList, char *tmp);
UserList *LoadUserList();
UserNode *IsExistUser(UserList *userList, char *userName);
char *GetUID(DirectoryNode *dirNode);
char *GetGID(DirectoryNode *dirNode);


// directory.c
char *getDir(char *dirPath);
void getPath(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack);
void WriteNode(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack);
void SaveDir(DirectoryTree *dirTree, Stack *dirStack);
int ReadNode(DirectoryTree *dirTree, char *tmp);
DirectoryTree *LoadDir();
DirectoryTree *InitializeTree();
DirectoryNode *IsExistDir(DirectoryTree *dirTree, char *dirName, char type);

// cat.c
int Concatenate(DirectoryTree* dirTree, char* fName, int type);
int ft_cat(DirectoryTree* dirTree, char* cmd);

// cd.c
int MovePath(DirectoryTree* dirTree, char* dirPath);
int Movecurrent(DirectoryTree* dirTree, char* dirPath);
int ft_cd(DirectoryTree* dirTree, char* cmd);

// chmod.c
int changeMode(DirectoryTree *dirTree, int mode, char *dirName);
void changeModeAll(DirectoryNode *dirNode, int mode);
int ft_chmod(DirectoryTree* dirTree, char* cmd);

// chown.c
int changeOwner(DirectoryTree *dirTree, char *userName, char *dirName);
void changeOwnerAll(DirectoryNode *dirNode, char *userName);
int ft_chown(DirectoryTree* dirTree, char* cmd);

// cp.c
int ft_cp(DirectoryTree* dirTree, char* SourceName, char* ObjectName);

// find.c
int readDir(DirectoryTree *dirTree, char *tmp, char *dirName, int o);
void findDir(DirectoryTree *dirTree, char *dirName, int o);
int ft_find(DirectoryTree *dirTree, char *cmd);

// grep.c
void ft_grep(char *search, char *fname, int Option);

// ls.c
int listDir(DirectoryTree *dirTree, int a, int l);
int ft_ls(DirectoryTree *dirTree, char *cmd);

// mkdir.c
int MakeDir(DirectoryTree *dirTree, char *dirName, char type);
int ft_mkdir(DirectoryTree *dirTree, char *cmd);

// pwd.c
int PrintPath(DirectoryTree *dirTree, Stack *dirStack);
int ft_pwd(DirectoryTree *dirTree, Stack *dirStack, char *cmd);

// rm.c
void destroyNode(DirectoryNode *dirNode);
void destroyDir(DirectoryNode *dirNode);
int removeDir(DirectoryTree *dirTree, char *dirName);
int ft_rm(DirectoryTree *dirTree, char *cmd);

// instruction.c
void Instruction(DirectoryTree* dirTree, char* cmd);

// system.c
void getToday(Date *date);
void printStart();
void printPrompt(DirectoryTree *dirTree, Stack *dirStack);
void login(UserList *userList, DirectoryTree *dirTree);

// permission.c
int modeToPermission(DirectoryNode *dirNode);
void printPermission(DirectoryNode *dirNode);
int checkPermission(DirectoryNode *dirNode, char o);

// stack.c
boolean isEmpty(Stack *stack);
Stack* initStack();
int pushStack(Stack* dirStack, char* dirName);
char *popStack(Stack* dirStack);

// time.c
void getMonth(int type);
void getWeekday(int type);

//global variable
DirectoryTree* Linux;
Stack* dStack;
UserList* usrList;
FILE* Dir;
FILE* User;
sem_t semp;

#endif 