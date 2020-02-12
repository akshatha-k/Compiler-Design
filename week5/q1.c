
/* CODE ONLY WORKS FOR STATEMENTS */

#include "lex.h"
#include <string.h>
FILE *fa;
int l=1,c=0;

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
	tokenptr tp= getNextToken(fa, &l, &c);
	if(tp->type==1)
	{
		if(tp->ln[0]=='/'||tp->ln[0]=='*'||tp->ln[0]=='%')
			return 1;
	}
	return 0;
}

int addop()
{
	tokenptr tp= getNextToken(fa, &l, &c);
	//printf("addop %s\n", types[tp->type]);
	if(tp->type==1)
	{
		if(tp->ln[0]=='+'||tp->ln[0]=='-')
			return 1;
	}
	//printf("not\n");
	return 0;
}

int relop()
{
	tokenptr tp= getNextToken(fa, &l, &c);
	//printf("relop %s\n",  types[tp->type]);
	if(tp->type==2)
	{
		return 1;
	}
	//printf("not\n");
	return 0;
}

int factor()
{

	tokenptr tp= getNextToken(fa, &l, &c);
	//printf("factor %s\n",  types[tp->type]);
	if(tp->type==8 || tp->type==6)
		return 1;
	//printf("not\n");
	return 0;
}

int tprime()
{
	//printf("tprime\n");
	if (mulop())
		return factor() && tprime();
	else {
		ungetToken(fa, &l, &c);
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
		ungetToken(fa, &l, &c);
		//printf("unget done \n");
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
		ungetToken(fa, &l, &c);
		return 1;
	}
}

int expn()
{
	//printf("expn \n");
	return simple_expn() && eprime();
}

int assign_stat()
{
	//printf("assign_Stat \n");
	return getNextToken(fa, &l, &c)->type==8 && (getNextToken(fa, &l, &c)->ln[0] == '=') && expn();
}

int semi_colon()
{
	return getNextToken(fa, &l, &c)->ln[0] == ';' ? 1 : 0;
}

int statement()
{
	return assign_stat() && semi_colon();
}
int main(){
	
	
	int l=1, c=0;
	
	fa = fopen("inp.txt", "r");

	if (fa == NULL){
		//printf("Cannot open file \n");
		exit(0); 
	}
	printf("%d\n", statement());

}
