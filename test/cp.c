#include "main.h"

int ft_cp(DirectoryTree* dirTree, char* SourceName, char* ObjectName) {
    char buf[1024];
    int original, new;
    int nread;

    if (access(SourceName, F_OK)) {
        printf("Not Exist Original File.\n");
        return FAIL;
    }
    if (!strcmp(SourceName, ObjectName)) {
        printf("cp: '%s' and '%s' are the same file.\n", SourceName, ObjectName);
        return FAIL;
    }
    original = open(SourceName, O_RDONLY);
    new = open(ObjectName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    nread = read(original, buf, sizeof(buf));
    write(new, buf, nread);
    MakeDir(dirTree, ObjectName, 'f');
    return SUCCESS;
}