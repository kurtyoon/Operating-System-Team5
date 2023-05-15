#include "../include/main.h"

int cp(DirectoryTree* dirTree, char* SourceName, char* ObjectName) {
    char buf[1024];
    int original, new;
    int nread;

    if (access(SourceName, F_OK)) {
        printf("원본 파일이 존재하지 않습니다.\n");
        return -1;
    }
    if (!strcmp(SourceName, ObjectName)) {
        printf("원본과 대상이 같습니다.\n");
        return -1;
    }
    original = open(SourceName, O_RDONLY);
    new = open(ObjectName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    nread = read(original, buf, sizeof(buf));
    write(new, buf, nread);
    MakeDir(dirTree, ObjectName, 'f');
    return 0;
}