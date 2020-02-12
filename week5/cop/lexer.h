#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>

#define TKL 80

typedef struct token {
    int row, col;
    int type;
    char lexeme[80], name[80];
} Token;

enum tokenTypes {IDENT, INVALID, AUTO, BREAK, CASE, CHAR, CONST, CONTINUE, DEFAULT, DO, DOUBLE, ELSE, ENUM, EXTERN, FLOAT, FOR, GOTO, IF, INT, LONG, REGISTER, RETURN, SHORT, SIGNED, SIZEOF, STATIC, STRUCT, SWITCH, TYPEDEF, UNION, UNSIGNED, VOID, VOLATILE, WHILE, PLUS, SUB, MUL, DIV, EQ, LP, RP, LCB, RCB, SEMI_COLON, LSQB, RSQB, LT, LTE, GT, GTE, EQEQ, PEQ, SEQ, MEQ, DEQ, MOD, MDEQ, QSTN, COLON, STRINGC, CHARC, INTC, FLOATC, DOUBLEC, SHORTC, LONGC, LS, RS, URS, INC, DEC, COMMA, SCA, SCO, NOT, NEQ, LA, LO, INV, ARROW, DOT};

char *tokenNames[] =  {"IDENT", "INVALID", "AUTO", "BREAK", "CASE", "CHAR", "CONST", "CONTINUE", "DEFAULT", "DO", "DOUBLE", "ELSE", "ENUM", "EXTERN", "FLOAT", "FOR", "GOTO", "IF", "INT", "LONG", "REGISTER", "RETURN", "SHORT", "SIGNED", "SIZEOF", "STATIC", "STRUCT", "SWITCH", "TYPEDEF", "UNION", "UNSIGNED", "VOID", "VOLATILE", "WHILE", "PLUS", "SUB", "MUL", "DIV", "EQ", "LP", "RP", "LCB", "RCB", "SEMI_COLON", "LSQB", "RSQB", "LT", "LTE", "GT", "GTE", "EQEQ", "PEQ", "SEQ", "MEQ", "DEQ", "MOD", "MDEQ", "QSTN", "COLON", "STRINGC", "CHARC", "INTC", "FLOATC", "DOUBLEC", "SHORTC", "LONGC", "LS", "RS", "URS", "INC", "DEC", "COMMA", "SCA", "SCO", "NOT", "NEQ", "LA", "LO", "INV", "ARROW", "DOT"};

char *tokenTypesStr[] = {"ident", "invalid", "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while", "+", "-", "*", "/", "=", "(", ")", "{", "}", ";", "[", "]", "<", "<=", ">", ">=", "==", "+=", "-=", "*=", "/=", "%", "%=", "?", ":", "stringc", "charc", "intc", "floatc", "doublec", "shortc", ">>", "<<", ">>>", "++", "--", ",", "&", "|", "!", "!=", "&&", "||", "~", "->", "."};

extern void insert_symbol(Token *);

int row, col;
FILE *fp;
Token *token;

void init_lexer(FILE *f)
{
	row = col = 1;
	fp = f;
}

int in(char *s, char *arr[])
{
    int i;
    for (i = 0; i < TKL; i++)
		if (strncmp(s, arr[i], 80) == 0)
			return i;
    return -1;
}

void ungetToken()
{
	fseek(fp, token->col - col - 1, SEEK_CUR);
	row = token->row;
	col = token->col;
}

Token* getToken()
{
    token = (Token *) malloc(sizeof(Token));
    char c, n;
    int lexeme_idx = 0;

    int type_idx;
    
    while ((c = fgetc(fp)) != EOF) {
		// comment skip
		if (c == '/') {
			// multi line 
			if ((n = fgetc(fp)) == '*') {
				while (1) {
					n = fgetc(fp);

					if (n == '\n')
						row++;

					else if (n == '*' && (n = fgetc(fp)) == '/' || c == EOF)
						break;
				}
			}
			// single line
			else if (n == '/') {
				while ((n = fgetc(fp)) != '\n' && c != EOF);
				ungetc(n, fp);
			}
			else {
			// DIV
				ungetc(n, fp);
			}
		}

		// skip whitespace automatically

		// pre-processor skip
		if (c == '#') {
			while ((c = fgetc(fp)) != '\n' && c != EOF);

			ungetc(c, fp);
		}
	
		// get strings
		if (c == '"') {
			token->row = row;
			token->col = col;
			token->type = STRINGC;

			token->lexeme[lexeme_idx++] = c;

			col++;
			while ((c = fgetc(fp)) != '"' && c != EOF) {
				token->lexeme[lexeme_idx++] = c;
				col++;
			}
			col++;
			token->lexeme[lexeme_idx++] = c;
	    
			break;
		}

		if (c == '\'') {
			token->row = row;
			token->col = col;
			token->type = CHARC;
			token->lexeme[lexeme_idx++] = c;
			col++;
			c = fgetc(fp);
			token->lexeme[lexeme_idx++] = c;
			if (c == '\\') {
				col++;
				c = fgetc(fp);
				token->lexeme[lexeme_idx++] = c;
			}
			col++;
			c = fgetc(fp);
			token->lexeme[lexeme_idx++] = c;

			break;
		}

		// ident or numerical constants or invalid tokens
		if (isalnum(c) || c == '_') {
			token->row = row;
			token->col = col;
			token->lexeme[lexeme_idx++] = c;

			// numbers
			if (isdigit(c)) {
				token->type = INTC;

				col++;
				while ((c = fgetc(fp)) && isdigit(c)) {
					token->lexeme[lexeme_idx++] = c;
					col++;
				}

				// decimals
				if (c == '.' || c == 'e') {
					token->lexeme[lexeme_idx++] = c;
					token->type = DOUBLEC;
					col++;
					while ((c = fgetc(fp)) && isdigit(c)) {
						token->lexeme[lexeme_idx++] = c;
						col++;
					}
				}

				// invalid
				else if (isalpha(c)) {
					token->type = INVALID;
				}

				ungetc(c, fp);
				col--;
			}
			// idents and kewords
			else {
				col++;
				while ((c = fgetc(fp)) && isalnum(c) || c == '_') {
					token->lexeme[lexeme_idx++] = c;
					col++;
				}
				ungetc(c, fp);
				col--;

				type_idx = in(token->lexeme, tokenTypesStr);
				token->type = type_idx == -1 ? IDENT : type_idx;
			}
	    
			break;
		}

		if (!isspace(c)) {
		// symbol tokens
			token->lexeme[lexeme_idx++] = c;
			token->row = row;
			token->col = col;

			if ((type_idx = in(token->lexeme, tokenTypesStr)) != -1) {
			// valid symbol
				// check if arithmatic symbol
				if (type_idx == PLUS || type_idx == SUB || type_idx == MUL || type_idx == DIV || type_idx == MOD) {

					col++;
					n = fgetc(fp);

					// check if combined operator
					if (n == '=' || type_idx == PLUS && n == '+' || type_idx == SUB && n == '-' || type_idx == SUB && n == '>') {
						token->lexeme[lexeme_idx++] = n;
					}
					else {
						ungetc(n, fp);
						col--;
					}
				}
				// check if relational symbol
				else if (type_idx == LT || type_idx == GT) {
					col++;
					n = fgetc(fp);

					if (type_idx == LT && n == '<' || n == '='){
					// check if <= or <<
						token->lexeme[lexeme_idx++] = n;
					}
					else if (type_idx == GT && n == '>'){
					// check if >= or >>
						token->lexeme[lexeme_idx++] = n;
						col++;
						char p = fgetc(fp);
						if (p == '>') {
							// check if >>>
							token->lexeme[lexeme_idx++] = n;
						}
						else {
							ungetc(p, fp);
							col--;
						}
					}
					else {
						ungetc(n, fp);
						col--;
					}
				}
				else if (type_idx == SCA || type_idx == SCO || type_idx == NOT) {
				// check if & or | or ! symbol
					col++;
					n = fgetc(fp);

					// check if && or || or !=
					if (type_idx == SCA && n == '&' || type_idx == SCO && n == '|' || type_idx == NOT && n == '=') {
						token->lexeme[lexeme_idx++] = n;
					}
					else {
						ungetc(n, fp);
						col--;
					}
				}
			} // valid symbol

		    else {
				ungetc(n, fp);
				col--;
		    }

			type_idx = in(token->lexeme, tokenTypesStr);
			token->type = type_idx == -1? INVALID : type_idx;

			break;
		}

		if (c == '\n') {
			col = 0;
			row++;
		}

		col++;
	} // while

	if (c == EOF)
		return NULL;

	// insert into symbol table
	insert_symbol(token);

    return token;
}

