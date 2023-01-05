#include "../include/tree.h"

int nodeExist(Node *root, int id) {
    if (root == NULL) {
        return 0;
    }
    if (root->id == id) {
        return 1;
    }
    return nodeExist(root->left, id) || nodeExist(root->right, id);
}

int Max(int a, int b) {
    if (a > b) 
        return a;
    return b;
}

Node* makeNode(int id) {
    Node *node = (Node*)malloc(sizeof(Node));
    node->id = id;
    node->height = 1;
    node->left = node->right = NULL;
    return node;
}

int height(Node* node) {
    if (node != NULL) 
        return node->height;
    return 0;
}

int balanceFactor(Node* node) {
    return height(node->right) - height(node->left);
}

void fixHeight(Node* node) {
    int h1 = height(node->left);
    int h2 = height(node->right);
    node->height = Max(h1, h2) + 1;
}

Node* rotateRight(Node* p) {
    Node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixHeight(p);
    fixHeight(q);
    return q;
}

Node* rotateLeft(Node* q) {
    Node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixHeight(q);
    fixHeight(p);
    return p;
}

Node* balance(Node* p) {
    fixHeight(p);
    if (balanceFactor(p) == 2) {
        if (balanceFactor(p->right) < 0) {
            p->right = rotateRight(p->right);
        }
        return rotateLeft(p);
    }
    if (balanceFactor(p) == -2) {
        if (balanceFactor(p->left) > 0) {
            p->left = rotateLeft(p->left);
        }
        return rotateRight(p);
    }
    return p;
}

Node* insertNode(Node* root, int id) {
    if (!root) {
        return makeNode(id);
    }
    if (id < root->id) {
        root->left = insertNode(root->left, id);
    } else {
        root->right = insertNode(root->right, id);
    }
    return balance(root);
}

int searchMin(Node* root) {
    if (root->left == NULL)
        return root->id;
    return searchMin(root->left);
}

Node* deleteNode(Node* root, int id) {
    if (root == NULL)
        return root;
    if (id < root->id) {
        root->left = deleteNode(root->left, id);
        return root;
    }
    if (id > root->id) {
        root->right = deleteNode(root->right, id);
        return root;
    }
    if (root->left != NULL && root->right != NULL) {
        root->id = searchMin(root->right);
        root->right = deleteNode(root->right, root->id);
        return root;
    } 
    if (root->left != NULL) {
        Node* c = root;
        root = root->left;
        free(c);
        c = NULL;
        return root;
    }
    if (root->right != NULL) {
        Node* c = root;
        root = root->right;
        free(c);
        c = NULL;
        return root;
    }
    free(root);
    root = NULL;
    
    return root;
}

void deleteTree(Node* tree) {
    if (tree != NULL) {
        deleteTree(tree->left);
        deleteTree(tree->right);
        free(tree);
    }
}

void printValue(int value, int indent) {
    while (indent--) {
        printf(" ");
    }
    printf("%d\n", value);
}

void indexCount(Node* tree, int indent) {
    if (tree == NULL) {
        return;
    }
    indexCount(tree->right, indent + 3);
    printValue(tree->id, indent);
    indexCount(tree->left, indent + 3);
}

void printTree(Node* tree) {
    indexCount(tree, 0);
}

int countSize(Node* tree) {
    if (tree == NULL) {
        return 0;
    } else {
        return countSize(tree->left) + countSize(tree->right) + 1;
    }
}