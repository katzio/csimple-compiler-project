#include "./ast.h"
#include "./typecheck.h"
extern int yylineno;

Node *makeBaseLeaf(char *token)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = strdup(token); // so we get a new pointer and not the original
    new_node->line = yylineno;
    new_node->type = -1;
    return new_node;
}
Node *makeParentNode(char *token, Node *left)
{

    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = strdup(token); // so we get a new pointer and not the original
    new_node->line = yylineno;
    new_node->left = left;
    new_node->type = -1;
    return new_node;
}
Node *makePairNode(char *token, Node *left, Node *right)
{

    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = strdup(token); // so we get a new pointer and not the original
    new_node->line = yylineno;
    new_node->left = left;
    new_node->right = right;
    new_node->type = -1;
    return new_node;
}

Node *makeTripNode(char *token, Node *left, Node *middle, Node *right)
{

    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->data = strdup(token); // so we get a new pointer and not the original
    new_node->line = yylineno;
    new_node->left = left;
    new_node->middle = middle;
    new_node->right = right;
    new_node->type = -1;
    return new_node;
}

void printInOrder(Node *tree, int indent)
{
    if (tree)
    {
        // printf("line: %d |   ", tree->line);

        for (int i = 0; i < indent; i++)
            printf(" | ");
        if (tree->data)
        {
            printf("-> %s", tree->data);
            if (tree->type != -1)
                printf(" (type: %s)\n", typeToChar(tree->type));
            else
                printf("\n");
        }
        if (tree->left)
        {
            printInOrder(tree->left, indent + 1);
        }
        if (tree->middle)
        {
            printInOrder(tree->middle, indent + 1);
        }
        if (tree->right)
        {
            printInOrder(tree->right, indent + 1);
        }
    }
    else
        printf("Null Tree\n");
}