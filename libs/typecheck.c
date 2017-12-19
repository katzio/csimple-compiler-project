
#include <stdlib.h>
#include <string.h>
#include "./typecheck.h"
void typecheck(ScopeStack **SCOPES_STACK_TOP_PTR, Node *ast, int depth)
{
    if (!ast->left)
        printf("Leaf(Depth: %d): %s\n", depth, ast->data);
    else
        printf("Type Checking of HEAD Node(Depth: %d): %s\n", depth, ast->data);

    if (strcmp("FUNCTION", ast->data) == 0)
    {
        char *scopeName = ast->left->left->data;
        printf("##SCOPE OF: %s\n", scopeName);
        // create a new scope
        ScopeStack *newScope = malloc(sizeof(ScopeStack));
        newScope->name = scopeName;
        push(SCOPES_STACK_TOP_PTR, newScope);
    }

    if (ast->left)
    {
        printf("Type Checking LEFT Node of %s(%d): %s\n", ast->data, depth, ast->left->data);
        typecheck(SCOPES_STACK_TOP_PTR, ast->left, depth + 1);
    }
    if (ast->middle)
    {
        printf("Type Checking MIDDLE Node of %s(%d): %s\n", ast->data, depth, ast->middle->data);
        typecheck(SCOPES_STACK_TOP_PTR, ast->middle, depth + 1);
    }
    if (ast->right)
    {
        printf("Type Checking RIGHT Node of %s(%d): %s\n", ast->data, depth, ast->right->data);
        typecheck(SCOPES_STACK_TOP_PTR, ast->right, depth + 1);
    }

    // AFTER WE FINISHED PROC THE SUB NODES

    if (strcmp("FUNCTION", ast->data) == 0 || strcmp("FUNCTION NO PARAMS", ast->data) == 0)
    {
        pop(SCOPES_STACK_TOP_PTR);
    }
}