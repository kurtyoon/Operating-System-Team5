#include "../include/main.h"

#define BUF_SIZE 1024

int ft_cp(DirectoryTree* dirTree, char* srcName, char* dstName) {
    char buf[BUF_SIZE];
    int srcFile, dstFile;
    ssize_t nread;

    if (access(srcName, F_OK)) {
        printf("Source file does not exist.\n");
        return FAIL;
    }
    if (!strcmp(srcName, dstName)) {
        printf("cp: '%s' and '%s' are the same file.\n", srcName, dstName);
        return FAIL;
    }

    srcFile = open(srcName, O_RDONLY);
    if (srcFile == FAIL) {
        perror("Unable to open source file");
        return FAIL;
    }
    dstFile = open(dstName, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
    if (dstFile == FAIL) {
        perror("Unable to open destination file");
        close(srcFile);
        return FAIL;
    }

    while ((nread = read(srcFile, buf, BUF_SIZE)) > 0) {
        if (write(dstFile, buf, nread) != nread) {
            perror("Error writing to destination file");
            close(srcFile);
            close(dstFile);
            return FAIL;
        }
    }

    if (nread == FAIL) {
        perror("Error reading source file");
    }

    close(srcFile);
    close(dstFile);
    makeDir(dirTree, dstName, 'f');

    return SUCCESS;
}
