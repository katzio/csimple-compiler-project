%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	#include "./libs/ast.h"

	extern int yylex();
	extern int yylineno;
	extern char *yytext;
	int yyerror(const char *msg);
%}
%union {
  char* string; // node head
  struct Node* Node; // leafs
}

/* TOKENS */
/* TERMINALS */
%token <string> CHAR_LITERAL STRING_LITERAL INTEGER NULL_TYPE
%token <string> IF ELSE FOR WHILE DOWHILE BOOL_TRUE BOOL_FALSE
%token <string> PLUS MINUS MUL DIV
%token <string> AND OR EQUAL GT GTE LT LTE NOT NOTEQUAL
%token <string> ADDRESS CONTENT TYPE RETURN IDENTIFIER
%token <string> SEMICOLON COLON COMMA O_CURL C_CURL O_PAREN C_PAREN VERT_LINE C_BRACK O_BRACK ASS
/* SPECS */
%left PLUS MINUS
%left MUL DIV
%left AND OR GT GTE LT LTE EQUAL NOTEQUAL NEG
%right ADDRESS CONTENT NOT

/* NODES */
/* basic leaf */
%type <Node> iden_name ident_type
%type <Node> bool_type
/* complex leafs */
%type <Node> var_dec vars_list parameters single_param
%type <Node> arr_index
%type <Node> expr func_call return_stmt
%type <Node> cond block stmt
%type <Node> assignment
%type <Node> code function f_parans

%nonassoc IFX
%nonassoc ELSE
/* definitions */
%%
start: code {printInOrder($1, 0);};

code: code stmt { $$ = makePairNode("MULTI-LINE", $1 ,$2); }
	| stmt {$$ = $1;}
		;
function: 
		ident_type iden_name O_PAREN parameters C_PAREN block { 
				Node * input = makePairNode("INPUT",$2, $4);
				Node * output = makePairNode("OUTPUT", $1, $6);
				$$ = makePairNode("FUNCTION", input, output); 
			}
		| 
		ident_type iden_name O_PAREN C_PAREN block {
				Node * input = makeParentNode("INPUT",$2);
				Node * output = makePairNode("OUTPUT", $1, $5);
				$$ = makePairNode("FUNCTION NO PARAMS", input, output);  
			 }
		;


f_parans: f_parans COMMA expr { $$ = makePairNode("FUNC INPUT PARAMS", $1, $3);}
		| expr {$$ = $1;}
		;

func_call: iden_name O_PAREN f_parans C_PAREN {$$ = makePairNode("FUNCTION CALL", $1, $3);}
		| iden_name O_PAREN C_PAREN {$$ = makeParentNode("FUNCTION CALL NO PARAMS", $1);}
		;

block: O_CURL code C_CURL {$$ = makeParentNode("BLOCK",$2);}
      | O_CURL C_CURL {$$ = makeBaseLeaf("EMPTY BLOCK");}
      ;

stmt: var_dec { $$ = $1; }
	| cond { $$ = $1; }
	| WHILE O_PAREN expr C_PAREN stmt {$$ = makePairNode("WHILE LOOP",$3,$5); }
	| DOWHILE stmt WHILE O_PAREN expr C_PAREN SEMICOLON {$$ = makePairNode("DO WHILE", $2, $5);}
	| FOR O_PAREN assignment SEMICOLON expr SEMICOLON assignment C_PAREN stmt {
			$$ = makePairNode("FOR LOOP", makeTripNode("FOR INPUT",$3, $5,$7), $9);  
		}
	| iden_name ASS expr SEMICOLON{ $$ = makePairNode("=", $1, $3); }
	| CONTENT iden_name ASS expr SEMICOLON{ $$ = makePairNode("PTR CONTENT", $2, $4); }
	| function  { $$ = $1; }
	| func_call SEMICOLON { $$ = $1; }
	| block { $$ =  $1; }
	| return_stmt { $$ =  $1; }
	;


assignment: iden_name ASS expr{ $$ = makePairNode("=", $1, $3); }
		|	CONTENT	iden_name ASS expr{ $$ = makePairNode("PTR CONTENT", $2, $4); }
		;

/* int x, char y */
parameters:parameters COMMA single_param { $$ = makePairNode("PARAMETERS", $1, $3); }
		| single_param {$$ = $1;}
		;
/* int x */
single_param: ident_type iden_name { $$ = makePairNode("PARAM", $1, $2); }
			;

/* int x, y, z; */
var_dec: ident_type vars_list SEMICOLON { $$ = makePairNode("VARIABLES DECLERATION",$1 ,$2); }
	;


/* x,y,z */
vars_list: vars_list COMMA iden_name { $$ = makePairNode("MULTIPLE IDENTIFIERS", $1, $3); }
		| iden_name { $$ = $1; }
		;

cond: IF expr stmt %prec IFX {$$ = makePairNode("IF",$2,$3); }
	| IF expr stmt ELSE stmt {$$ = makeTripNode("IF/ELSE",$2, $3, $5); }
	;

expr: IDENTIFIER {$$ = makeBaseLeaf($1);}
	| INTEGER  {$$ = makeBaseLeaf($1);}
	| CHAR_LITERAL {$$ = makeBaseLeaf($1);}
	| STRING_LITERAL {$$ = makeBaseLeaf($1);}
	| func_call { $$ = $1; }
	| O_PAREN expr C_PAREN { $$ = $2; }
	| IDENTIFIER arr_index {$$ = makeParentNode("ACCESS INDEX",$2);}
	| NULL_TYPE { $$ = makeBaseLeaf("NULL"); }
	| NOT expr { $$ = makeParentNode("!", $2); }
	| MINUS expr %prec NEG { $$ = makeParentNode("-", $2); }
    | expr PLUS expr { $$ = makePairNode("+",$1,$3); }
    | expr MUL expr { $$ = makePairNode("*",$1,$3); }
    | expr MINUS expr { $$ = makePairNode("-",$1,$3); }
    | expr DIV expr { $$ = makePairNode("/",$1,$3); }
	| CONTENT expr { $$ = makeParentNode("^",$2); }
	| ADDRESS expr { $$ = makeParentNode("&",$2); }
	| VERT_LINE iden_name VERT_LINE { $$ = makeParentNode("|",$2); }
	| expr LT expr { $$ = makePairNode("<",$1,$3); }
    | expr GT expr { $$ = makePairNode(">",$1,$3); }
    | expr LTE expr { $$ = makePairNode("<=",$1,$3); }
    | expr GTE expr { $$ = makePairNode(">=",$1,$3); }
    | expr EQUAL expr { $$ = makePairNode("==",$1,$3); }
    | expr NOTEQUAL expr { $$ = makePairNode("!=",$1,$3); }
    | expr AND expr { $$ = makePairNode("&&",$1,$3); }
    | expr OR expr { $$ = makePairNode("||",$1,$3); }
	| bool_type { $$ = makeParentNode("boolean", $1); }
	;

ident_type:	TYPE {$$ = makeBaseLeaf($1);};
bool_type:	BOOL_TRUE { $$ = makeBaseLeaf("bool_true"); }
          | BOOL_FALSE { $$ = makeBaseLeaf("bool_false"); }
		  ;
iden_name:	IDENTIFIER {$$ = makeBaseLeaf($1);}
		| IDENTIFIER arr_index {$$ = makePairNode("ARRAY ACCESS",makeBaseLeaf($1), $2);}
		;
arr_index:  O_BRACK expr C_BRACK { $$ = $2 ;}
		| O_BRACK assignment C_BRACK { $$ = $2 ;}
;
return_stmt: RETURN expr SEMICOLON { $$ = makeParentNode("RETURN",$2);}
| RETURN SEMICOLON { $$ = makeBaseLeaf("RETURN VOID");}
;
%%
/* subroutines */
int yyerror(const char *msg)
{
	fflush(stdout);
	fprintf(stderr, "Error: %s at line %d\n", msg, yylineno);
	fprintf(stderr, "Parser does not expect '%s'\n",yytext);
}
int main() {
  yyparse();
  return 0;
}