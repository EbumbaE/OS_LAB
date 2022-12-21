#pragma once
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>

struct _tnode {
    int id;
    int height;
    struct Node* left;
    struct Node* right;
};
typedef struct _tnode Node;

int max(int a, int b);
Node* makeNode(int id);

int height(Node* node);
int balanceFactor(Node* node);
void fixHeight(Node* node);

Node* rotateRight(Node* p);
Node* rotateLeft(Node* q);

Node* balance(Node* p);

Node* insertNode(Node* root, int id);

int searchMin(Node* root);
Node* deleteNode(Node* root, int id);

void deleteTree(Node* root);

void printValue(int value, int indent);
void indexCount(Node* tree, int indent);
void printTree(Node* tree);

int countSize(Node* root);