#include "../include/linked_tree.h"

TreeNode *createNode(int data) {
    TreeNode *newNode = (TreeNode *)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->firstChild = NULL;
    newNode->nextSibling = NULL;
    return newNode;
}

void addChild(TreeNode *parent, TreeNode *child) {
    if (parent->firstChild == NULL) {
        parent->firstChild = child;
    } else {
        TreeNode *sibling = parent->firstChild;
        while (sibling->nextSibling != NULL) {
            sibling = sibling->nextSibling;
        }
        sibling->nextSibling = child;
    }
}

TreeNode *findParent(TreeNode *root, TreeNode *nodeToFind) {
    if (root == NULL || root->firstChild == NULL) {
        return NULL;
    }

    TreeNode *child = root->firstChild;
    while (child != NULL) {
        if (child == nodeToFind) {
            return root;
        }
        TreeNode *foundNode = findParent(child, nodeToFind);
        if (foundNode != NULL) {
            return foundNode;
        }
        child = child->nextSibling;
    }
    return NULL;
}

void deleteChild(TreeNode *parent, TreeNode *childToDelete) {
    if (parent == NULL || parent->firstChild == NULL) {
        return;
    }

    if (parent->firstChild == childToDelete) {
        parent->firstChild = childToDelete->nextSibling;
        childToDelete->nextSibling = NULL;
        free(childToDelete);
        return;
    }

    TreeNode *sibling = parent->firstChild;
    while (sibling->nextSibling != NULL) {
        if (sibling->nextSibling == childToDelete) {
            sibling->nextSibling = childToDelete->nextSibling;
            childToDelete->nextSibling = NULL;
            free(childToDelete);
            return ;
        }
        sibling = sibling->nextSibling;
    }
}

void printTree(TreeNode *node, int level) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    printf("%d\n", node->data);

    TreeNode *child = node->firstChild;
    while (child != NULL) {
        printTree(child, level + 1);
        child = child->nextSibling;
    }
}