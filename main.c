/*
 * File        : rbtree.c
 * Project     : Red Black Tree (Data Structures)
 * Author      : SANJAY HIBARE
 * Date        : 31-10-2026
 * Description : Implementation of Red Black Tree operations (insert, delete, search, min/max)
 */

/*
 * main.c
 * Red Black Tree - menu driven test program
 */

#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

/* read an integer safely, keep asking until user gives a valid number */
static int read_number(const char *msg)
{
    int num;

    printf("%s", msg);
    while (scanf("%d", &num) != 1)
    {
        int c;
        while ((c = getchar()) != '\n' && c != EOF)
            ;                       /* throw away the bad input */
        if (c == EOF)
        {
            exit(0);
        }
        printf("Invalid input, try again: ");
    }

    return num;
}

static void print_menu(void)
{
    printf("\n------ RED BLACK TREE ------\n");
    printf("1. Insert\n");
    printf("2. Delete\n");
    printf("3. Search\n");
    printf("4. Find minimum\n");
    printf("5. Delete minimum\n");
    printf("6. Find maximum\n");
    printf("7. Delete maximum\n");
    printf("8. Display (inorder)\n");
    printf("9. Display (tree shape)\n");
    printf("0. Exit\n");
}

int main(void)
{
    tree_t *root = NULL;
    int choice;
    data_t item;

    while (1)
    {
        print_menu();
        choice = read_number("Enter your choice: ");

        switch (choice)
        {
            case 1:
                item = read_number("Enter data to insert: ");
                if (insert(&root, item) == SUCCESS)
                    printf("%d inserted\n", item);
                else
                    printf("Insertion failed (duplicate or no memory)\n");
                break;

            case 2:
                item = read_number("Enter data to delete: ");
                if (delete(&root, item) == SUCCESS)
                    printf("%d deleted\n", item);
                else
                    printf("Deletion failed, %d not found\n", item);
                break;

            case 3:
                item = read_number("Enter data to search: ");
                if (search(&root, item) == SUCCESS)
                    printf("%d is present in the tree\n", item);
                else
                    printf("%d is not present in the tree\n", item);
                break;

            case 4:
                if (find_minimum(&root, &item) == SUCCESS)
                    printf("Minimum is %d\n", item);
                else
                    printf("Tree is empty\n");
                break;

            case 5:
                if (delete_minimum(&root) == SUCCESS)
                    printf("Minimum node deleted\n");
                else
                    printf("Tree is empty\n");
                break;

            case 6:
                if (find_maximum(&root, &item) == SUCCESS)
                    printf("Maximum is %d\n", item);
                else
                    printf("Tree is empty\n");
                break;

            case 7:
                if (delete_maximum(&root) == SUCCESS)
                    printf("Maximum node deleted\n");
                else
                    printf("Tree is empty\n");
                break;

            case 8:
                if (root == NULL)
                {
                    printf("Tree is empty\n");
                }
                else
                {
                    printf("Inorder: ");
                    display_inorder(root);
                    printf("\n");
                }
                break;

            case 9:
                if (root == NULL)
                {
                    printf("Tree is empty\n");
                }
                else
                {
                    printf("(tilt your head left, root is on the left side)\n\n");
                    display_tree(root, 0);
                }
                break;

            case 0:
                free_tree(&root);
                printf("Bye!\n");
                return 0;

            default:
                printf("Wrong choice, please select from the menu\n");
        }
    }

    return 0;
}