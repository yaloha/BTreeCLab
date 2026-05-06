#include "BTree.h"

BTreeNode* init_node(int is_leaf){
    BTreeNode* node = (BTreeNode*) malloc(sizeof(BTreeNode));
    if (node == NULL){
        printf("couldn't allocate memory for the node\n");
        return NULL;
    }
    node->key_count = 0;

    node->is_leaf = is_leaf;
    for (int i = 0; i < M; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void insert_key_value(KeyValuePair* kvp, const char* key, double value){
    strncpy(kvp->key, key, 6);
    kvp->key[6] = '\0';
    kvp->value = value;
}

void split_child(BTreeNode* parent, int idx){
    BTreeNode* child = parent->children[idx];
    BTreeNode* new_node = init_node(child->is_leaf);

    new_node->key_count = M / 2 - 1;

    for (int i = 0; i < M / 2 - 1; i++){
        insert_key_value(&new_node->keys[i], child->keys[i + M / 2].key, child->keys[i + M / 2].value);
    }

    if (!child->is_leaf) {
        for (int i = 0; i < M / 2; i++) {
            new_node->children[i] = child->children[i + M / 2];
        }
    }

    child->key_count = M/2 - 1;
    for (int i = parent->key_count; i >= idx + 1; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[idx + 1] = new_node;

    for (int i = parent->key_count - 1; i >= idx; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }

    insert_key_value(&parent->keys[idx], child->keys[M / 2 - 1].key, child->keys[M / 2 - 1].value);
    parent->key_count++;
}

void insert_node(BTreeNode* node, int value, const char* key){
    int i = node->key_count - 1;
    if (node->is_leaf) {
        while ((strcmp(key, node->keys[i].key) < 0) && (i >= 0)) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        insert_key_value(&node->keys[i + 1], key, value);
        node->key_count++;
    }else{
        while ((strcmp(key, node->keys[i].key) < 0) && (i >= 0)) {
            i--;
        }
        i++;
        if (node->children[i]->key_count == M - 1) {
            split_child(node, i);

            if (strcmp(key, node->keys[i].key) > 0) {
                i++;
            }
        }
        insert_node(node->children[i], value, key);
    }
}

void insert(BTreeNode** root, const char* key, int value){
    struct BTreeNode *node = *root;
    if (node == NULL){
        *root = init_node(1);
        insert_key_value(&(*root)->keys[0], key, value);
        (*root)->key_count = 1;
    }else{
        if ((*root)->key_count == M - 1) {
            struct BTreeNode *new_root = init_node( 0);
            new_root->children[0] = *root;
            split_child(new_root, 0);
            *root = new_root;
        }
        insert_node(*root, value, key);
    }
}

void printB(BTreeNode* root){
    if (root != NULL){
        int i;
        for (i = 0; i < root->key_count; i++){
            printB(root->children[i]);
            printf("[k]: %s [v]: %lf \n", root->keys[i].key, root->keys[i].value);
        }
        printB(root->children[i]);
    }
}

int value_by_key(BTreeNode* node, char* key, double* result){
    if (node == NULL)
        return 0;
    int i = 0;
    while ((strcmp(key, node->keys[i].key) > 0) && (i <= node->key_count - 1)) {
        i++;
    }
    if (i < node->key_count && strcmp(key, node->keys[i].key) == 0) {
        *result = node->keys[i].value;
        return 1;
    }

    if (node->is_leaf)
        return 0;

    return value_by_key(node->children[i], key, result);
}

void fill(BTreeNode* node, int idx){
    BTreeNode* lchild = node->children[idx - 1];
    BTreeNode* child = node->children[idx];
    BTreeNode* rchild = node->children[idx + 1];
    if (idx != 0 && lchild->key_count > M / 2 - 1){
        for (int i = child->key_count - 1; i >= 0; i--) child->keys[i + 1] = child->keys[i];
        if (child->is_leaf != 1) for (int i = child->key_count - 1; i >= 0; i--) child->children[i + 1] = child->children[i];

        child->keys[0] = node->keys[idx - 1];

        if (child->is_leaf != 1) child->children[0] = lchild->children[lchild->key_count];
        node->keys[idx - 1] = lchild->keys[lchild->key_count - 1];

        child->key_count++;
        lchild->key_count--;
    }else if (idx != 0 && rchild->key_count > M / 2 - 1) {
        child->keys[child->key_count] = node->keys[idx];

        if (child->is_leaf != 1) child->children[child->key_count] = rchild->children[0];

        for (int i = 0; i < child->key_count - 1; i++) child->keys[i] = child->keys[i + 1];
        if (child->is_leaf != 1) for (int i = 0; i <= child->key_count - 1; i++) rchild->children[i] = rchild->children[i + 1];

        child->key_count++;
        rchild->key_count--;
    }else{
        if (idx == node->key_count) idx--;
        child->keys[M / 2 - 1] = node->keys[idx];

        for (int i = 0; i < rchild->key_count; i++) {
            child->keys[i + M / 2] = rchild->keys[i];
        }

        if (!child->is_leaf) {
            for (int i = 0; i <= rchild->key_count; i++) {
                child->children[i + M / 2] = rchild->children[i];
            }
        }

        for (int i = idx + 1; i < node->key_count; i++) {
            node->keys[i - 1] = node->keys[i];
        }

        for (int i = idx + 2; i <= node->key_count; i++) {
            node->children[i - 1] = node->children[i];
        }

        child->key_count += rchild->key_count + 1;
        node->key_count--;

        free(rchild);
    }
}

int delete_by_key(BTreeNode* node, char* key){
    if (node == NULL)
        return 0;
    int idx = 0;
    while ((strcmp(key, node->keys[idx].key) > 0) && (idx <= node->key_count - 1)) {
        idx++;
    }
    if (idx < node->key_count && strcmp(key, node->keys[idx].key) == 0) {
        if (node->is_leaf) {
            for (int i = idx + 1; i < node->key_count; i++) {
                node->keys[i - 1] = node->keys[i];
            }
            node->key_count--;
            return 1;
        } else {
            BTreeNode* left_child = node->children[idx];
            BTreeNode* right_child = node->children[idx + 1];
            if (left_child->key_count >= M / 2) {
                BTreeNode* cur = left_child;
                while (!cur->is_leaf) cur = cur->children[cur->key_count];

                node->keys[idx] = cur->keys[cur->key_count - 1];
                return delete_by_key(left_child, node->keys[idx].key);
            } else if (right_child->key_count >= M / 2) {
                BTreeNode* cur = right_child;
                while (!cur->is_leaf) cur = cur->children[0];

                node->keys[idx] = cur->keys[0];
                return delete_by_key(right_child, node->keys[idx].key);
            } else {
                fill(node, idx);
                return delete_by_key(left_child, key);
            }
        }
    }
    if (node->is_leaf) return 0;

    if (node->children[idx]->key_count < M / 2) {
        fill(node, idx);
        if (idx > node->key_count) idx--;
    }

    return delete_by_key(node->children[idx], key);
}

void delete_key(BTreeNode** root, char* key) {
    if (*root == NULL) return;
    int err = delete_by_key(*root, key);
    if (err != 1) {
        printf("Deletion error");
        return;
    }
    if ((*root)->key_count == 0) {
        BTreeNode* old_root = *root;
        if (!(*root)->is_leaf) *root = (*root)->children[0];
        else *root = NULL;
        free(old_root);
    }
}

void free_tree(BTreeNode** tree){
    if (tree == NULL || *tree == NULL) return;
    BTreeNode* node = *tree;
    if (node->is_leaf != 1)
        for(int i = 0;i <= node->key_count; i++)
            free_tree(&node->children[i]);
    free(node);
    *tree = NULL;
}
