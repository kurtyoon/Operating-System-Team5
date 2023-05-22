#include "../include/main.h"

//loadDirectoryTree
//"file.Directory,txt", read each line and making new Node, and DirectoryTree에 추가
//fclose, 로드된 디렉토리의 트리의 현재 위치를 root로 설정 후, dirTree 반환.
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

//text file로 save.
void saveDirectoryTree(DirectoryTree *dirTree, Stack *dirStack) {
    Dir = fopen("file/Directory.txt", "w");
    writeDirNode(dirTree, dirTree->root, dirStack);
    fclose(Dir);
}
