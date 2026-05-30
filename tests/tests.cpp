#include <gtest/gtest.h>

extern "C" {
#include "BTree.h"
}

TEST(BTreeTest, InsertSingleAndSearch) {
    BTreeNode* root = NULL;
    insert(&root, "abc", 123.45);
    double val;
    ASSERT_TRUE(value_by_key(root, "abc", &val));
    EXPECT_DOUBLE_EQ(val, 123.45);
    free_tree(&root);
}

TEST(BTreeTest, InsertAndSearch) {
    BTreeNode* root = NULL;
    insert(&root, "beta", 20.3);
    insert(&root, "alpha", 10.5);
    insert(&root, "gamma", 30.7);
    double val;
    EXPECT_TRUE(value_by_key(root, "alpha", &val));
    EXPECT_DOUBLE_EQ(val, 10.5);
    EXPECT_TRUE(value_by_key(root, "beta", &val));
    EXPECT_DOUBLE_EQ(val, 20.3);
    EXPECT_TRUE(value_by_key(root, "gamma", &val));
    EXPECT_DOUBLE_EQ(val, 30.7);
    free_tree(&root);
}

TEST(BTreeTest, SearchMissingKey) {
    BTreeNode* root = NULL;
    insert(&root, "exist", 99.9);
    double val;
    EXPECT_FALSE(value_by_key(root, "missing", &val));
    free_tree(&root);
}

TEST(BTreeTest, DeleteLeafKey) {
    BTreeNode* root = NULL;
    insert(&root, "a", 1.0);
    insert(&root, "b", 2.0);
    insert(&root, "c", 3.0);
    double val;
    EXPECT_TRUE(delete_key(&root, "b"));
    EXPECT_FALSE(value_by_key(root, "b", &val));
    EXPECT_TRUE(value_by_key(root, "a", &val));
    EXPECT_TRUE(value_by_key(root, "c", &val));
    free_tree(&root);
}

TEST(BTreeTest, DeleteKey) {
    BTreeNode* root = NULL;
    insert(&root, "d", 40.0);
    insert(&root, "b", 20.0);
    insert(&root, "f", 60.0);
    insert(&root, "a", 10.0);
    insert(&root, "c", 30.0);
    insert(&root, "e", 50.0);
    insert(&root, "g", 70.0);
    double val;
    EXPECT_TRUE(delete_key(&root, "d"));
    EXPECT_FALSE(value_by_key(root, "d", &val));
    EXPECT_TRUE(value_by_key(root, "a", &val));
    EXPECT_TRUE(value_by_key(root, "b", &val));
    EXPECT_TRUE(value_by_key(root, "c", &val));
    EXPECT_TRUE(value_by_key(root, "e", &val));
    EXPECT_TRUE(value_by_key(root, "f", &val));
    EXPECT_TRUE(value_by_key(root, "g", &val));
    free_tree(&root);
}