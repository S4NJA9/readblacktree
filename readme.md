# Red Black Tree

**Author:** Your Name
**Date:** 31-10-2026
**Language:** C
**Module:** Data Structures

## Description

This project is an implementation of a **Red Black Tree** in C. A Red Black
Tree is a self-balancing binary search tree. Every node has a color (red or
black), and a set of rules on these colors keeps the tree balanced after every
insertion and deletion. Because of this, search, insert and delete always take
**O(log n)** time, even for sorted input where a normal BST would become a
linked list.

The program is menu driven, so every operation can be tried from the terminal.

## Red Black Tree Rules

1. Every node is either red or black.
2. The root is always black.
3. All leaves (NULL) are black.
4. A red node cannot have a red child.
5. Every path from a node to its descendant NULL nodes has the same number of
   black nodes.

## Features

- Insert a node
- Delete a node
- Search a node
- Find minimum
- Delete minimum
- Find maximum
- Delete maximum
- Display in sorted order (with colors)
- Display the tree shape

## Project Structure

```
rbtree_project/
├── rbtree.h    -> structure, return codes, function prototypes
├── rbtree.c    -> all tree operations, rotations and fix-up functions
├── main.c      -> menu driven test program
├── Makefile    -> build script
└── README.md   -> this file
```

## Function Prototypes

| Function         | Prototype                                      |
|------------------|------------------------------------------------|
| Insert           | `int insert(tree_t **root, data_t item);`      |
| Delete           | `int delete(tree_t **root, data_t item);`      |
| Search           | `int search(tree_t **root, data_t item);`      |
| Find Minimum     | `int find_minimum(tree_t **root, data_t *min);`|
| Find Maximum     | `int find_maximum(tree_t **root, data_t *max);`|
| Delete Minimum   | `int delete_minimum(tree_t **root);`           |
| Delete Maximum   | `int delete_maximum(tree_t **root);`           |

All functions return `SUCCESS` (0) or `FAILURE` (-1).

## How to Build and Run

Requirements: `gcc` and `make`.

```
make            # compile the project
./rbtree        # run the program
make clean      # remove object files and the executable
```

## Sample Output

```
------ RED BLACK TREE ------
1. Insert
2. Delete
3. Search
4. Find minimum
5. Delete minimum
6. Find maximum
7. Delete maximum
8. Display (inorder)
9. Display (tree shape)
0. Exit
Enter your choice: 1
Enter data to insert: 10
10 inserted
```

Option 9 prints the tree sideways (root on the left). Tilt your head to the
left to see it:

```
      30(B)
20(B)
      15(B)
            5(R)
```

`(R)` means a red node and `(B)` means a black node.

## Time Complexity

| Operation                         | Time     |
|-----------------------------------|----------|
| Insert / Delete / Search          | O(log n) |
| Find / Delete minimum or maximum  | O(log n) |
| Display (inorder)                 | O(n)     |

## Notes

- Duplicate values are not allowed.
- NULL pointers are treated as black leaves.
- `data_t` is `int` by default. It can be changed in `rbtree.h`.

## Reference

- https://en.wikipedia.org/wiki/Red%E2%80%93black_tree