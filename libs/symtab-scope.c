#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./symtab-scope.h"

extern SymbEntry *currentFunction;

bool searchScope(ScopeStack **currentScope, char *name)
{
    SymbEntry *symb_walker;
    symb_walker = (*currentScope)->table_ptr;
    while (symb_walker)
    {
        if (strcmp(name, symb_walker->name) == 0)
        {
            fprintf(stderr, "previous declaration of ‘%s’ was found in this scope!\n", name);
            return true;
        }
        symb_walker = symb_walker->nextEntry;
    }

    // searched all tables in all scopes, did not found, returns NULL
    return false;
}

/**
 * This function will only look for the right name
 * if it find a function with a name
 * it's up to the reciving function to validate integrity
 * 
 * return the symbol itself if found by name
 * return NULL if no result found
 * 
 */
SymbEntry *find(ScopeStack **currentScope, char *name)
{
    SymbEntry *symb_walker;
    ScopeStack *scope_walker = *currentScope;
    while (scope_walker)
    {
        symb_walker = scope_walker->table_ptr;
        while (symb_walker)
        {
            if (strcmp(name, symb_walker->name) == 0)
                return symb_walker;
            symb_walker = symb_walker->nextEntry;
        }
        scope_walker = scope_walker->next_scope;
    }
    // searched all tables in all scopes, did not found, returns NULL
    return NULL;
}

void printTable(char *SCOPE_NAME, SymbEntry *head)
{
    if (!head)
        return;

    SymbEntry *walker = head;
    printf("\n---------\nSYMBOL TABLE OF SCOPE: %s\n", SCOPE_NAME);
    printf("%-15s%-15s%-15s%-15s%-15s%-15s%-15s\n", "NAME", "|", "TYPE", "|", "DATA", "|", "POINTS TO");
    do
    {
        if (walker->nextEntry)
            printf("%-15s%-15s%-15s%-15s%-15p%-15s%-15s\n", walker->name, "|", walker->var_type, "|", walker->data, "|", walker->nextEntry->name);
        else
        {
            printf("%-15s%-15s%-15s%-15s%-15p%-15s%-15s\n", walker->name, "|", walker->var_type, "|", walker->data, "|", "NULL");
        }
        walker = walker->nextEntry;
    } while (walker);
    printf("---------\n\n");
}

/**
 * inserts a new symbol to the current top scope in the stack
 */
void insert(ScopeStack *currentScope, SymbEntry *newEntry)
{
    // printf("INSERT: '%s:%s'  into symbol table of '%s'\n", newEntry->var_type, newEntry->name, currentScope->name);
    SymbEntry *walker = currentScope->table_ptr;
    // if symbol table is empty
    if (!walker)
        currentScope->table_ptr = newEntry;
    // printf("\n####  %s  ####\n", newEntry->name);

    else
    {
        while (walker->nextEntry)
            walker = walker->nextEntry;
        walker->nextEntry = newEntry;
    }
    // printTable(currentScope->name, currentScope->table_ptr);
}

void printStack(ScopeStack **currentScope)
{
    ScopeStack *walker = *currentScope;
    printf("---------\n\nCurrent Scopes Stack\n");
    printf("%-15s%-15s%-15s%-15s%-15s\n", "NAME", "|", "ADDRESS", "|", "POINTS TO\n");

    if (!(*currentScope))
        return;

    do
    {
        if (walker->next_scope)
            printf("%-15s%-15s%-15p%-15s%-15s\n", walker->name, "|", walker, "|", walker->next_scope->name);
        else
            printf("%-15s%-15s%-15p%-15s%-15s\n", walker->name, "|", walker, "|", "NULL");
        walker = walker->next_scope;
    } while (walker);
    printf("---------\n\n");
}

/** 
 * inserts a new scope to the stack
 */
void push(ScopeStack **currentScope, ScopeStack *newScope)
{
    //printf("PUSH: '%s'\n", newScope->name);
    // first, make the new scope point to the current top scope
    newScope->next_scope = *currentScope;
    // now move the currentScope top the new scope
    *currentScope = newScope;
    // printStack(currentScope);
}

void pop(ScopeStack **currentScope)
{
    // printf("POP: '%s'\n", (*currentScope)->name);

    if (strcmp("FUNCTION INFO", (*currentScope)->name) == 0)
    {

        // reset currentFunction ptr
        currentFunction = NULL;
        // swtich to the current scope function
        ScopeStack *scopesRunner = (*currentScope);
        while (scopesRunner != NULL)
        {
            // while we are not in global scope
            // search for a function wrapper
            SymbEntry *tableRunner = scopesRunner->table_ptr;
            // get the last entry
            while (tableRunner && tableRunner->nextEntry)
                tableRunner = tableRunner->nextEntry;

            if (tableRunner && tableRunner->data)
                // found a function, assing
                currentFunction = tableRunner;

            scopesRunner = scopesRunner->next_scope;
        }

        if (strcmp("GLOBAL", (*currentScope)->next_scope->name) == 0)
        {
            currentFunction = NULL;
            // printf("SWITCHINGT TO NEW FUNCTION PTR: %s\n", currentFunction->name);
        }
        else
        {
            // printf("SWITCHINGT TO GLOBAL\n");
        }
    }
    SymbEntry *runner = (*currentScope)->table_ptr;
    ScopeStack *ptr = *currentScope;
    *currentScope = (*currentScope)->next_scope;
    //printStack(currentScope);
}