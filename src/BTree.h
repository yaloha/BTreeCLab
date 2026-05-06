#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef BTREECLAB_BTREE_H
#define BTREECLAB_BTREE_H

#define M 4

typedef struct {
    char key[7];    
    double value;  
} KeyValuePair;

typedef struct BTreeNode{
    int is_leaf;
    int key_count;
    KeyValuePair keys[M - 1];
    struct BTreeNode* children[M];
} BTreeNode;

BTreeNode* init_node(int is_leaf);
void split_child(BTreeNode* parent, int idx);
void insert_node(BTreeNode* node, int value, const char* key);
void insert(BTreeNode** root, const char* key, int value);
void printB(BTreeNode* root);
int value_by_key(BTreeNode* node, char* key, double* result);
void fill(BTreeNode* node, int idx);
int delete_by_key(BTreeNode* node, char* key);
void delete_key(BTreeNode** root, char* key);
void free_tree(BTreeNode** tree);

#endif
