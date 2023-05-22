#include "../include/main.h"

// cp sourceFile newFile
// cp sourceFile directory
// cp soureceFile direcotory/newFile

int ft_cp(DirectoryTree* dirTree, char* SourceName, char* ObjectName) {
    char buf[1024];
    int original, new;
    int nread;

    //SourceName, Check existing.
    if (access(SourceName, F_OK)) {
        printf("Not Exist Original File.\n");
        return FAIL;
    }
    //SourceName과 ObjectName의 이름이 같은지 확인
    if (!strcmp(SourceName, ObjectName)) {
        printf("cp: '%s' and '%s' are the same file.\n", SourceName, ObjectName);
        return FAIL;
    }
    //Source file, 읽기 전용으로 open
    original = open(SourceName, O_RDONLY);
    //Object file, 쓰기 및 생성 모드로 open
    new = open(ObjectName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    //Source file, read and write
    nread = read(original, buf, sizeof(buf));
    write(new, buf, nread);
    //Object file, DirectoryTree에 추가.
    makeDir(dirTree, ObjectName, 'f');
    return SUCCESS;
}