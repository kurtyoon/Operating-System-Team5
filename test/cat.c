#include "main.h"

int Concatenate(DirectoryTree* dirTree, char* fName, int type) {

    UserNode* tmpUser = NULL;
    DirectoryNode* tmpNode = NULL;
    FILE* fp;
    char buf[MAX_BUFFER];
    char tmpName[MAX_NAME];
    char* str;
    int tmpSIZE = 0;
    int cnt = 1;

    if (type) {
        if(type == 4){
            tmpUser = usrList->head;
            while (tmpUser){
                printf("%s:x:%d:%d:%s:%s\n", tmpUser->name, tmpUser->id.UID, tmpUser->id.GID, tmpUser->name, tmpUser->dir);
                tmpUser = tmpUser->nextNode;
            }
            return 0;
        }
        tmpNode = IsExistDir(dirTree,fName, 'f');

        if(!tmpNode) return -1;

        fp = fopen(fName, "r");
        while (!feof(fp)) {
            fgets(buf, sizeof(buf), fp);
            if(feof(fp)) break;
            if(type == 2) {
                if (buf[strlen(buf) - 1] == '\n'){
                    printf("     %d ",cnt);
                    cnt++;
                }
            } else if(type == 3) {
                if(buf[strlen(buf)-1] == '\n' && buf[0] != '\n'){
                    printf("     %d ",cnt);
                    cnt++;
                }
            }
            fputs(buf, stdout);
        }
        fclose(fp);
    } else {
        fp = fopen(fName, "w");

	    while (fgets(buf, sizeof(buf), stdin)) {
            fputs(buf, fp);
            tmpSIZE += strlen(buf) - 1;
        }
        fclose(fp);

        tmpNode = IsExistDir(dirTree, fName, 'f');
        if (tmpNode) {
            time(&ltime);
            today = localtime(&ltime);

            tmpNode->date.month = today->tm_mon + 1;
            tmpNode->date.day = today->tm_mday;
            tmpNode->date.hour = today->tm_hour;
            tmpNode->date.minute = today->tm_min;
        } else MakeDir(dirTree, fName, 'f');
        tmpNode = IsExistDir(dirTree, fName, 'f');
        tmpNode->SIZE = tmpSIZE;
    }
    return 0;
}

int ft_cat(DirectoryTree* dirTree, char* command)
{
    DirectoryNode* currentNode = NULL;
    DirectoryNode* tmpNode = NULL;
    DirectoryNode* tmpNode2 = NULL;
    char* str;
    char tmp[MAX_DIR];
    char tmp2[MAX_DIR];
    char tmp3[MAX_DIR];
    int value;

    if (!command) {
        long			size;
	    unsigned char	c;
        size = read(0, &c, 1);
	    while (size)
	    {
		write(1, &c, size);
		size = read(0, &c, 1);
	    }
    }
    currentNode = dirTree->current;

    if (strcmp(command, ">") == 0) {
        str = strtok(NULL, " ");
        if(str == NULL){
            printf("cat: Invalid option\n");
            printf("Try 'cat --help' for more information.\n");
            return -1;
        }
        strncpy(tmp, str, MAX_DIR);
        if(!strstr(str, "/")) {
            if (checkPermission(dirTree->current, 'w')) {
                printf("cat: Can not create file '%s': No permission.\n", dirTree->current->name);
                return -1;
            }
            tmpNode = IsExistDir(dirTree, str, 'd');
            if (tmpNode) {
                printf("cat: '%s': Is a directory\n", str);
                return -1;
            }
            else Concatenate(dirTree, str, 0);
        } else {
            strncpy(tmp2, getDir(str), MAX_DIR);
            value = MovePath(dirTree, tmp2);
            if (value) {
                printf("cat: '%s': No such file or directory\n", tmp2);
                return -1;
            }
            str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            if (checkPermission(dirTree->current, 'w')) {
                printf("cat: Can not create file '%s': No permission.\n", dirTree->current->name);
                dirTree->current = currentNode;
                return -1;
            }
            tmpNode = IsExistDir(dirTree, tmp3, 'd');
            if (tmpNode) {
                printf("cat: '%s': Is a directory\n", tmp3);
                dirTree->current = currentNode;
                return -1;
            } else Concatenate(dirTree, tmp3, 0);
            dirTree->current = currentNode;
        }
        return 0;
    } else if(command[0] == '-'){
        if (!strcmp(command, "-n")) {
            str = strtok(NULL, " ");
            strncpy(tmp, str, MAX_DIR);
            if (!strstr(str, "/")) {
                if (checkPermission(dirTree->current, 'w')) {
                    printf("cat: Can not create file '%s': No permission.\n", dirTree->current->name);
                    return FALSE;
                }
                tmpNode = IsExistDir(dirTree, str, 'd');
                tmpNode2 = IsExistDir(dirTree, str, 'f');
    
                if (!tmpNode && !tmpNode2) {
                    printf("cat: '%s': No such file or directory.\n", str);
                    return FALSE;
                } else if (tmpNode && !tmpNode2) {
                    printf("cat: '%s': Is a directory.\n", str);
                    return FALSE;
                } else if (tmpNode2 && checkPermission(tmpNode2, 'r')){
                    printf("cat: Can not open file '%s': Permission denied\n", tmpNode2->name);
                    return FALSE;
                } else Concatenate(dirTree, str, 2);
            } else {
                strncpy(tmp2, getDir(str), MAX_DIR);
                value = MovePath(dirTree, tmp2);
                if (value) {
                    printf("cat: '%s': No such file or directory.\n", tmp2);
                    return -1;
                }
                str = strtok(tmp, "/");
                while (str) {
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'd');
                tmpNode2 = IsExistDir(dirTree, tmp3, 'f');
    
                if (!tmpNode && !tmpNode2) {
                    printf("cat: '%s': No such file or directory.\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                else if (tmpNode && !tmpNode2) {
                    printf("cat: '%s': Is a directory.\n", tmp3);
                    dirTree->current = currentNode;
                    return -1;
                }
                else if (tmpNode2 && checkPermission(tmpNode2, 'r')) {
                    printf("cat: Can not open file '%s': Permission denied\n", tmpNode2->name);
                    dirTree->current = currentNode;
                    return -1;
                } else Concatenate(dirTree, tmp3, 2);
                dirTree->current = currentNode;
            }
        } else if(!strcmp(command, "-b")) {
            str = strtok(NULL, " ");
            strncpy(tmp, str, MAX_DIR);
            if (!strstr(str, "/")) {
                if (checkPermission(dirTree->current, 'w')) {
                    printf("cat: Can not create file '%s': Permission denied\n", dirTree->current->name);
                    return FALSE;
                }
                tmpNode = IsExistDir(dirTree, str, 'd');
                tmpNode2 = IsExistDir(dirTree, str, 'f');
                if(!tmpNode && !tmpNode2) {
                    printf("cat: '%s': No such file or directory.\n", str);
                    return FALSE;
                }
                else if (tmpNode && !tmpNode2) {
                    printf("cat: '%s': Is a directory.\n", str);
                    return FALSE;
                }
                else if (tmpNode2 && checkPermission(tmpNode2, 'r')) {
                    printf("cat: Can not open file '%s': Permission denied\n", tmpNode2->name);
                    return FALSE;
                }
                else Concatenate(dirTree, str, 3);
                
            } else {
                strncpy(tmp2, getDir(str), MAX_DIR);
                value = MovePath(dirTree, tmp2);
                if (value) {
                    printf("cat: '%s': No such file or directory.\n", tmp2);
                    return -1;
                }
                str = strtok(tmp, "/");
                while (str) {
                    strncpy(tmp3, str, MAX_NAME);
                    str = strtok(NULL, "/");
                }
                tmpNode = IsExistDir(dirTree, tmp3, 'd');
                tmpNode2 = IsExistDir(dirTree, tmp3, 'f');
                if (!tmpNode && !tmpNode2) {
                    printf("cat: '%s': No such file or directory.\n", tmp3);
                    dirTree->current = currentNode;
                    return FALSE;
                }
                else if (tmpNode && !tmpNode2){
                    printf("cat: '%s': Is a direcotry\n", tmp3);
                    dirTree->current = currentNode;
                    return FALSE;
                }
                else if (tmpNode2 && checkPermission(tmpNode2, 'r')) {
                    printf("cat: Can not open file '%s': Permission denied\n", tmpNode2->name);
                    dirTree->current = currentNode;
                    return FALSE;
                }
                else Concatenate(dirTree, tmp3, 3);
                dirTree->current = currentNode;
            }
        } else if(strcmp(command, "--help") == 0) {
            printf("Usage: cat [OPTION]... [FILE]...\n");
            printf("Concatenate FILE(s) to standard output.\n\n");
            printf("With no FILE, or when FILE is -, read standard input.\n\n");
            printf("  -n, --number             number all output lines\n");
            printf("  -b, --number-nonblank    number nonempty output lines, overrides -n\n");
            printf("      --help     display this help and exit\n");
            return FALSE;
        } else {
            str = strtok(command, "-");
            if(!str) {
                printf("cat: Invalid option\n");
                printf("Try 'cat --help' for more information.\n");
                return FALSE;
            } else {
                printf("cat: Unrecognized option -- '%s'\n", str);
                printf("Try 'cat --help' for more information.\n");
                return FALSE;
            }
        }
    } else {
        if (!strcmp(command, "/etc/passwd")) {
            Concatenate(dirTree, command, 4);
            return SUCCESS;
        }
        strncpy(tmp, command, MAX_DIR);
        if (!strstr(command, "/")) {
            if (checkPermission(dirTree->current, 'w')) {
                printf("cat: Can not create file '%s': Permission denied\n", dirTree->current->name);
                return FALSE;
            }
            tmpNode = IsExistDir(dirTree, command, 'd');
            tmpNode2 = IsExistDir(dirTree, command, 'f');
            if (!tmpNode && !tmpNode2) {
                printf("cat: '%s': No such file or directory.\n", command);
                return FALSE;
            } else if (tmpNode && !tmpNode2) {
                printf("cat: '%s': Is a directory\n", command);
                return FALSE;
            } else if (tmpNode2 && checkPermission(tmpNode2, 'r')) {
                printf("cat: Can not open file '%s': Permission denied\n", tmpNode2->name);
                return FALSE;
            } else Concatenate(dirTree, command, 1);
        } else {
            strncpy(tmp2, getDir(command), MAX_DIR);
            value = MovePath(dirTree, tmp2);
            if (value) {
                printf("cat: '%s': No such file or directory.\n", tmp2);
                return FALSE;
            }
            str = strtok(tmp, "/");
            while (str) {
                strncpy(tmp3, str, MAX_NAME);
                str = strtok(NULL, "/");
            }
            tmpNode = IsExistDir(dirTree, tmp3, 'd');
            tmpNode2 = IsExistDir(dirTree, tmp3, 'f');
            if(!tmpNode && !tmpNode2) {
                printf("cat: '%s': No such file or directory.\n", tmp3);
                dirTree->current = currentNode;
                return FALSE;
            } else if (tmpNode && !tmpNode2) {
                printf("cat: '%s': Is a directory\n", tmp3);
                dirTree->current = currentNode;
                return FALSE;
            } else if (tmpNode2 && checkPermission(tmpNode2, 'r')) {
                printf("cat:  Can not open file '%s': Permission denied\n", tmpNode2->name);
                dirTree->current = currentNode;
                return FALSE;
            } else Concatenate(dirTree, tmp3, 1);
            dirTree->current = currentNode;
        }
    }
    return 1;
}