#include "symbol-table.h"

int mulop();
int addop();
int relop();
int factor();
int tprime();
int term();
int seprime();
int simple_expn();
int eprime();
int expn();
int assign_stat();
int semi_colon();
int statement();

int mulop()
{
	Token *tk = getToken();
	switch (tk->type) {
		case MUL :
		case DIV :
		case MOD :
			return 1;
	}

	return 0;
}

int addop()
{
	Token *tk = getToken();
	switch (tk->type) {
		case PLUS:
		case SUB:
			return 1;
	}
	return 0;
}

int relop()
{
	Token *tk = getToken();
	//printf("relop %s\n", tokenTypesStr[tk->type]);

	switch (tk->type) {
		case EQEQ:
		case NEQ:
		case LT:
		case LTE:
		case GT:
		case GTE:
			return 1;
	}

	//printf("not\n");
	return 0;
}

int factor()
{
	Token *tk = getToken();
	//printf("fact %s\n", tokenTypesStr[tk->type]);

	switch (tk->type) {
		case IDENT:
		case INTC:
		case SHORTC:
		case LONGC:
		case FLOATC:
		case DOUBLEC:
		case CHARC:
			return 1;
	}

	//printf("not\n");
	return 0;
}

int tprime()
{
	//printf("tprime\n");
	if (mulop())
		return factor() && tprime();
	else {
		ungetToken();
		return 1;
	}
}

int term()
{
	//printf("term\n");
	return factor() && tprime();
}

int seprime()
{
	//printf("seprime\n");
	if (addop())
		return term() && seprime() ;
	else {
		ungetToken();
		return 1;
	}
}

int simple_expn()
{
	//printf("simp_expn\n");
	return term() && seprime();
}

int eprime()
{
	//printf("eprime\n");
	if (relop())
		return simple_expn();
	else {
		ungetToken();
		return 1;
	}
}

int expn()
{
	return simple_expn() && eprime();
}

int assign_stat()
{
	return getToken()->type == IDENT && getToken()->type == EQ && expn();
}

int semi_colon()
{
	return getToken()->type == SEMI_COLON ? 1 : 0;
}

int statement()
{
	return assign_stat() && semi_colon();
}

int main(void)
{
    char *filename = (char *) malloc(sizeof(char) * 80);
    fgets(filename, 79, stdin);

    int l = strlen(filename);
    filename[l-1] = '\0';

    FILE *fdi = fopen(filename, "r");

	init_lexer(fdi);

	printf("%d\n", statement());

    return 0;
}
