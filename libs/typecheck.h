#ifndef TYPECHECK_H_
#define TYPECHECK_H_
#include "./ast.h"
#include "./symtab-scope.h"
void typecheck(ScopeStack **, Node *, int);
void scopeHandler(ScopeStack **, Node *);
#endif