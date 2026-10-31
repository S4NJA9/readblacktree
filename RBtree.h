/*
 * rbtree.h
 * Red Black Tree - header file
 *
 * Holds the node structure, return codes and the function
 * prototypes used by main.c
 */

#ifndef RBTREE_H
#define RBTREE_H

#define SUCCESS  0
#define FAILURE -1

/* the type of data stored in the tree (change here if you want floats etc.) */
typedef int data_t;

/* every node is either red or black */
typedef enum
{
    RED,
    BLACK
} color_t;

typedef struct node
{
    data_t data;
    color_t color;
    struct node *left;
    struct node *right;
    struct node *parent;
} tree_t;

/* ---- main operations ---- */
int insert(tree_t **root, data_t item);
int delete(tree_t **root, data_t item);
int search(tree_t **root, data_t item);
int find_minimum(tree_t **root, data_t *min);
int find_maximum(tree_t **root, data_t *max);
int delete_minimum(tree_t **root);
int delete_maximum(tree_t **root);

/* ---- helper functions ---- */
void display_inorder(tree_t *root);
void display_tree(tree_t *root, int level);
void free_tree(tree_t **root);

#endif