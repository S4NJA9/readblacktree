/*
 * rbtree.c
 * Red Black Tree - function definitions
 *
 * Rules we have to keep:
 *  1. every node is red or black
 *  2. root is black
 *  3. NULL (leaf) is treated as black
 *  4. a red node never has a red child
 *  5. every path from a node down to a NULL has the same number of black nodes
 */

#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

/* ------------------------------------------------------------------ */
/*                      small helper functions                        */
/* ------------------------------------------------------------------ */

/* NULL is black, so we need to check for NULL before reading color */
static int is_red(tree_t *node)
{
    return (node != NULL && node->color == RED);
}

static int is_black(tree_t *node)
{
    return (node == NULL || node->color == BLACK);
}

static tree_t *create_node(data_t item)
{
    tree_t *new = malloc(sizeof(tree_t));

    if (new == NULL)
    {
        return NULL;
    }

    new->data = item;
    new->color = RED;      /* new nodes always start as red */
    new->left = NULL;
    new->right = NULL;
    new->parent = NULL;

    return new;
}

/* go to the left most node of a subtree */
static tree_t *minimum_node(tree_t *node)
{
    while (node->left != NULL)
    {
        node = node->left;
    }
    return node;
}

/* go to the right most node of a subtree */
static tree_t *maximum_node(tree_t *node)
{
    while (node->right != NULL)
    {
        node = node->right;
    }
    return node;
}

static tree_t *find_node(tree_t *root, data_t item)
{
    while (root != NULL)
    {
        if (item == root->data)
        {
            return root;
        }
        else if (item < root->data)
        {
            root = root->left;
        }
        else
        {
            root = root->right;
        }
    }
    return NULL;
}

/* ------------------------------------------------------------------ */
/*                           rotations                                */
/* ------------------------------------------------------------------ */

/*
 *      x                 y
 *       \      --->     /
 *        y             x
 */
static void rotate_left(tree_t **root, tree_t *x)
{
    tree_t *y = x->right;

    x->right = y->left;
    if (y->left != NULL)
    {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NULL)
    {
        *root = y;
    }
    else if (x == x->parent->left)
    {
        x->parent->left = y;
    }
    else
    {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

/*
 *        x             y
 *       /     --->      \
 *      y                 x
 */
static void rotate_right(tree_t **root, tree_t *x)
{
    tree_t *y = x->left;

    x->left = y->right;
    if (y->right != NULL)
    {
        y->right->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == NULL)
    {
        *root = y;
    }
    else if (x == x->parent->right)
    {
        x->parent->right = y;
    }
    else
    {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
}

/* ------------------------------------------------------------------ */
/*                            insertion                               */
/* ------------------------------------------------------------------ */

/* fix the tree after a new red node is added */
static void insert_fixup(tree_t **root, tree_t *z)
{
    tree_t *uncle;

    while (z != *root && z->parent->color == RED)
    {
        /* parent is red so it can't be the root, grandparent exists */
        if (z->parent == z->parent->parent->left)
        {
            uncle = z->parent->parent->right;

            if (is_red(uncle))
            {
                /* case 1: uncle is red -> just recolor and move up */
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                /* case 2: z is a right child -> make it a left-left case */
                if (z == z->parent->right)
                {
                    z = z->parent;
                    rotate_left(root, z);
                }

                /* case 3: left-left case */
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_right(root, z->parent->parent);
            }
        }
        else
        {
            /* same thing but left and right swapped */
            uncle = z->parent->parent->left;

            if (is_red(uncle))
            {
                z->parent->color = BLACK;
                uncle->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            }
            else
            {
                if (z == z->parent->left)
                {
                    z = z->parent;
                    rotate_right(root, z);
                }

                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rotate_left(root, z->parent->parent);
            }
        }
    }

    (*root)->color = BLACK;
}

int insert(tree_t **root, data_t item)
{
    tree_t *parent = NULL;
    tree_t *temp = *root;
    tree_t *new;

    /* find the place where the new node should go (like a normal BST) */
    while (temp != NULL)
    {
        parent = temp;

        if (item == temp->data)
        {
            return FAILURE;     /* duplicates are not allowed */
        }
        else if (item < temp->data)
        {
            temp = temp->left;
        }
        else
        {
            temp = temp->right;
        }
    }

    new = create_node(item);
    if (new == NULL)
    {
        return FAILURE;
    }

    new->parent = parent;

    if (parent == NULL)
    {
        *root = new;            /* tree was empty */
    }
    else if (item < parent->data)
    {
        parent->left = new;
    }
    else
    {
        parent->right = new;
    }

    insert_fixup(root, new);

    return SUCCESS;
}

/* ------------------------------------------------------------------ */
/*                            deletion                                */
/* ------------------------------------------------------------------ */

/* put node v in the place of node u (only fixes u's parent link) */
static void transplant(tree_t **root, tree_t *u, tree_t *v)
{
    if (u->parent == NULL)
    {
        *root = v;
    }
    else if (u == u->parent->left)
    {
        u->parent->left = v;
    }
    else
    {
        u->parent->right = v;
    }

    if (v != NULL)
    {
        v->parent = u->parent;
    }
}

/*
 * x is the node that has an "extra black".
 * x can be NULL, that is why we also carry x_parent separately.
 */
static void delete_fixup(tree_t **root, tree_t *x, tree_t *x_parent)
{
    tree_t *w;      /* sibling of x */

    while (x != *root && is_black(x))
    {
        if (x == x_parent->left)
        {
            w = x_parent->right;

            if (is_red(w))
            {
                /* case 1: sibling is red */
                w->color = BLACK;
                x_parent->color = RED;
                rotate_left(root, x_parent);
                w = x_parent->right;
            }

            if (is_black(w->left) && is_black(w->right))
            {
                /* case 2: sibling and both its children are black */
                w->color = RED;
                x = x_parent;
                x_parent = x->parent;
            }
            else
            {
                if (is_black(w->right))
                {
                    /* case 3: sibling's far child is black */
                    w->left->color = BLACK;
                    w->color = RED;
                    rotate_right(root, w);
                    w = x_parent->right;
                }

                /* case 4: sibling's far child is red */
                w->color = x_parent->color;
                x_parent->color = BLACK;
                w->right->color = BLACK;
                rotate_left(root, x_parent);
                x = *root;      /* done, this will end the loop */
            }
        }
        else
        {
            /* mirror image of the above */
            w = x_parent->left;

            if (is_red(w))
            {
                w->color = BLACK;
                x_parent->color = RED;
                rotate_right(root, x_parent);
                w = x_parent->left;
            }

            if (is_black(w->left) && is_black(w->right))
            {
                w->color = RED;
                x = x_parent;
                x_parent = x->parent;
            }
            else
            {
                if (is_black(w->left))
                {
                    w->right->color = BLACK;
                    w->color = RED;
                    rotate_left(root, w);
                    w = x_parent->left;
                }

                w->color = x_parent->color;
                x_parent->color = BLACK;
                w->left->color = BLACK;
                rotate_right(root, x_parent);
                x = *root;
            }
        }
    }

    if (x != NULL)
    {
        x->color = BLACK;
    }
}

/* removes the given node from the tree and frees it */
static void delete_node(tree_t **root, tree_t *z)
{
    tree_t *y = z;          /* node which is actually removed / moved */
    tree_t *x;              /* node which takes y's old place */
    tree_t *x_parent;
    color_t y_original_color = y->color;

    if (z->left == NULL)
    {
        x = z->right;
        x_parent = z->parent;
        transplant(root, z, z->right);
    }
    else if (z->right == NULL)
    {
        x = z->left;
        x_parent = z->parent;
        transplant(root, z, z->left);
    }
    else
    {
        /* two children: use the inorder successor */
        y = minimum_node(z->right);
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z)
        {
            x_parent = y;
        }
        else
        {
            x_parent = y->parent;
            transplant(root, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        transplant(root, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    free(z);

    /* removing a black node breaks rule 5, so we have to fix it */
    if (y_original_color == BLACK)
    {
        delete_fixup(root, x, x_parent);
    }
}

int delete(tree_t **root, data_t item)
{
    tree_t *node;

    if (root == NULL || *root == NULL)
    {
        return FAILURE;
    }

    node = find_node(*root, item);
    if (node == NULL)
    {
        return FAILURE;         /* item not present */
    }

    delete_node(root, node);

    return SUCCESS;
}

/* ------------------------------------------------------------------ */
/*                       search / min / max                           */
/* ------------------------------------------------------------------ */

int search(tree_t **root, data_t item)
{
    if (root == NULL || *root == NULL)
    {
        return FAILURE;
    }

    if (find_node(*root, item) != NULL)
    {
        return SUCCESS;
    }

    return FAILURE;
}

int find_minimum(tree_t **root, data_t *min)
{
    if (root == NULL || *root == NULL || min == NULL)
    {
        return FAILURE;
    }

    *min = minimum_node(*root)->data;

    return SUCCESS;
}

int find_maximum(tree_t **root, data_t *max)
{
    if (root == NULL || *root == NULL || max == NULL)
    {
        return FAILURE;
    }

    *max = maximum_node(*root)->data;

    return SUCCESS;
}

int delete_minimum(tree_t **root)
{
    if (root == NULL || *root == NULL)
    {
        return FAILURE;
    }

    delete_node(root, minimum_node(*root));

    return SUCCESS;
}

int delete_maximum(tree_t **root)
{
    if (root == NULL || *root == NULL)
    {
        return FAILURE;
    }

    delete_node(root, maximum_node(*root));

    return SUCCESS;
}

/* ------------------------------------------------------------------ */
/*                             display                                */
/* ------------------------------------------------------------------ */

/* prints the data in sorted order along with the color */
void display_inorder(tree_t *root)
{
    if (root == NULL)
    {
        return;
    }

    display_inorder(root->left);
    printf("%d(%c) ", root->data, root->color == RED ? 'R' : 'B');
    display_inorder(root->right);
}

/*
 * prints the tree sideways: root on the left, right subtree on top,
 * left subtree at the bottom. Tilt your head to the left to see the tree.
 */
void display_tree(tree_t *root, int level)
{
    int i;

    if (root == NULL)
    {
        return;
    }

    display_tree(root->right, level + 1);

    for (i = 0; i < level; i++)
    {
        printf("      ");
    }
    printf("%d(%c)\n", root->data, root->color == RED ? 'R' : 'B');

    display_tree(root->left, level + 1);
}

/* free every node (post order) */
void free_tree(tree_t **root)
{
    if (*root == NULL)
    {
        return;
    }

    free_tree(&(*root)->left);
    free_tree(&(*root)->right);
    free(*root);
    *root = NULL;
}