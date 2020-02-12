#include "lexer.h"

enum scopes {L = 'L', G = 'G'};

enum symbolDataTypes {PTRt, INTt, SHORTt, LONGt, FLOATt, DOUBLEt, CHARt, VOIDt, FUNCt};
char *symbolDataTypesStr[] = {"PTRt", "INTt", "SHORTt", "LONGt", "FLOATt", "DOUBLEt", "CHARt", "VOIDt", "FUNCt"};
int symbolSizes[] = {sizeof(int*), sizeof(int), sizeof(short), sizeof(long), sizeof(float), sizeof(double), sizeof(char)};

typedef struct symbol{
    char name[80], scope;
    int type, size, no_args, ret_type;
    struct symbol *args[10];
	struct symbol *next;
} symbol;

symbol table[100];
int table_idx = -1;

Token *data_type = NULL, *ptr = NULL, *var = NULL, *prev = NULL, *size = NULL;
int func_idx = -1;

enum scopes scope = G;

void insert_symbol(Token *tk)
{
    if (tk->type == INT || tk->type == SHORT || tk->type == LONG || tk->type == FLOAT || tk->type == DOUBLE || tk->type == STRUCT || tk->type == ENUM || tk->type == CHAR) {
		data_type = tk;
    }

    else if (tk->type == MUL && data_type) {
		// ptr type
		ptr = tk;
    }

    else if (tk->type == IDENT && data_type) {
		// assume its a var
		var = tk;

		// set type
		if (ptr) {
		    // ptr var
			table[++table_idx].type = PTRt;
		}
		else {
			// non ptr var
			switch (data_type->type) {
			    case INT:
					table[++table_idx].type = INTt;
					break;
			    case SHORT:
					table[++table_idx].type = SHORTt;
					break;
			    case LONG:
					table[++table_idx].type = LONGt;
					break;
			    case FLOAT:
					table[++table_idx].type = FLOATt;
					break;
			    case DOUBLE:
					table[++table_idx].type = DOUBLEt;
					break;
			    case CHAR:
					table[++table_idx].type = CHARt;
					break;
			}
		} // non ptr var

		table[table_idx].size = symbolSizes[table[table_idx].type];
		table[table_idx].no_args = 0;
		table[table_idx].ret_type = -1;
		table[table_idx].scope = scope;
		strncpy(table[table_idx].name, var->lexeme, 80);

		if (func_idx != -1) {
			// function argument
			table[func_idx].args[table[func_idx].no_args++] = &table[table_idx];
		}
	} // var

	else if (tk->type == RSQB && data_type) {
		// array
		table[table_idx].size = symbolSizes[table[table_idx].type] * strtol(prev->lexeme, NULL, 10);
	}

	else if (tk->type == LP && data_type) {
		// its a function
		func_idx = table_idx;	
		
		// set return type
		table[func_idx].ret_type = table[func_idx].type; // since type was already determined
		table[func_idx].type = FUNCt;
		table[func_idx].size = -1;

		table[func_idx].scope = 'G';
		scope = 'L';
	} // function

	else if (tk->type == COMMA) {
		if (data_type)
			ptr = NULL;
	}

	else if (tk->type == RP || tk->type == SEMI_COLON) {
		// function arguments finished
		func_idx = -1;
		// reset context
		data_type = ptr = NULL;
	}

	prev = tk;
}

