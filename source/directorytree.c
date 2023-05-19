#include "../include/main.h"

// DirectoryTree *initDirectoryTree() {
//     DirectoryTree *dirTree = (DirectoryTree *)malloc(sizeof(DirectoryTree));
//     DirectoryNode *NewNode = (DirectoryNode *)malloc(sizeof(DirectoryNode));

//     time(&ltime);
//     today = localtime(&ltime);

//     strncpy(NewNode->name, "/", MAX_NAME);
//     NewNode->type ='d';
//     NewNode->permission.mode = 755;
//     modeToPermission(NewNode);
//     NewNode->id.UID = usrList->head->id.UID;
//     NewNode->id.GID = usrList->head->id.GID;
//     NewNode->SIZE = 4096;
//     NewNode->date.month = today->tm_mon+1;
//     NewNode->date.day = today->tm_mday;
//     NewNode->date.hour = today->tm_hour;
//     NewNode->date.minute = today->tm_min;
//     NewNode->parent = NULL;
//     NewNode->firstChild = NULL;
//     NewNode->nextSibling = NULL;

//     dirTree->root = NewNode;
//     dirTree->current = dirTree->root;

//     return dirTree;
// }

DirectoryTree *loadDirectoryTree() {
    DirectoryTree *dirTree = (DirectoryTree *)malloc(sizeof(DirectoryTree));
    char tmp[MAX_LENGTH];

    Dir = fopen("file/Directory.txt", "r");
    while (fgets(tmp, MAX_LENGTH, Dir)) {
        readDirNode(dirTree, tmp);
    }
    fclose(Dir);
    dirTree->current = dirTree->root;
    return dirTree;
}

void saveDirectoryTree(DirectoryTree *dirTree, Stack *dirStack) {
    Dir = fopen("file/Directory.txt", "w");
    writeDirNode(dirTree, dirTree->root, dirStack);
    fclose(Dir);
}
