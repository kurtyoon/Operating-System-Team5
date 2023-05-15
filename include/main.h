#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
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
    struct userNode *LinkNode;
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
	struct stackNode *LinkNode;
} StackNode;

// Stack
typedef struct stack {
	StackNode* TopNode;
	int cnt;
} Stack;


//time
time_t ltime;
struct tm *today;

// user.c
UserList *InitializeUser();
void WriteUser(UserList *userList, UserNode *userNode);
void SaveUserList(UserList *userList);
int ReadUser(UserList *userList, char *tmp);
UserList *LoadUserList();
UserNode *IsExistUser(UserList *userList, char *userName);
char *GetUID(DirectoryNode *dirNode);
char *GetGID(DirectoryNode *dirNode);
int HasPermission(DirectoryNode *dirNode, char o);

// directory.c
void DestroyNode(DirectoryNode *dirNode);
void DestroyDir(DirectoryNode *dirNode);
char *getDir(char *dirPath);
void getPath(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack);
void WriteNode(DirectoryTree *dirTree, DirectoryNode *dirNode, Stack *dirStack);
void SaveDir(DirectoryTree *dirTree, Stack *dirStack);
int ReadNode(DirectoryTree *dirTree, char *tmp);
DirectoryTree *LoadDir();
DirectoryTree *InitializeTree();
int MakeDir(DirectoryTree *dirTree, char *dirName, char type);
int RemoveDir(DirectoryTree *dirTree, char *dirName);
int PrintPath(DirectoryTree *dirTree, Stack *dirStack);
int ListDir(DirectoryTree *dirTree, int a, int l);
int ChangeMode(DirectoryTree *dirTree, int mode, char *dirName);
void ChangeModeAll(DirectoryNode *dirNode, int mode);
int ChangeOwner(DirectoryTree *dirTree, char *userName, char *dirName);
void ChangeOwnerAll(DirectoryNode *dirNode, char *userName);
int ReadDir(DirectoryTree *dirTree, char *tmp, char *dirName, int o);
void FindDir(DirectoryTree *dirTree, char *dirName, int o);
DirectoryNode *IsExistDir(DirectoryTree *dirTree, char *dirName, char type);

// cat.c
int Concatenate(DirectoryTree* dirTree, char* fName, int type);
int ft_cat(DirectoryTree* dirTree, char* cmd);

// cd.c
int cd(DirectoryTree* dirTree, char* cmd);
int MovePath(DirectoryTree* dirTree, char* dirPath);
int Movecurrent(DirectoryTree* dirTree, char* dirPath);

// chmod.c
int chmod(DirectoryTree* dirTree, char* cmd);

// chown.c
int chown_(DirectoryTree* dirTree, char* cmd);

// cp.c
int cp(DirectoryTree* dirTree, char* SourceName, char* ObjectName);

// find.c
int find_(DirectoryTree *dirTree, char *cmd);

// grep.c
void grep(char *search, char *fName);
void grep_a(char *searching_word, char *f_name);

// ls.c
int ls(DirectoryTree *dirTree, char *cmd);

// mkdir.c
int mkdir(DirectoryTree *dirTree, char *cmd);

// pwd.c
int pwd(DirectoryTree *dirTree, Stack *dirStack, char *cmd);

// rm.c
int rm(DirectoryTree *dirTree, char *cmd);

// instruction.c
void Instruction(DirectoryTree* dirTree, char* cmd);
void PrintStart();
void PrintHead(DirectoryTree* dirTree, Stack* dirStack);

// system.c
void PrintStart();
void PrintHead(DirectoryTree *dirTree, Stack *dirStack);
void Login(UserList *userList, DirectoryTree *dirTree);

// permission.c
int Mode2Permission(DirectoryNode *dirNode);
void PrintPermission(DirectoryNode *dirNode);

// stack.c
boolean IsEmpty(Stack *stack);
Stack* InitializeStack();
int Push(Stack* dirStack, char* dirName);
char *Pop(Stack* dirStack);

// time.c
void GetMonth(int i);
void GetWeekday(int i);

//global variable
DirectoryTree* Linux;
Stack* dStack;
UserList* usrList;
FILE* Dir;
FILE* User;
sem_t semp;

#endif 