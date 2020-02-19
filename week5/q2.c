
#include "lex.h"
#include <string.h>
FILE *fa;
int l=1,c=0;
tokenptr t;
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
	//printf("statement \n");
	return assign_stat() && semi_colon();
}
int statement_list()
{
	printf("statement list \n");
	t=getNextToken(fa,&l,&c);
	if(t->ln[0]=='}')
	{
		ungetToken(fa,&l,&c);
		return 1;
	}
	else
	{
		ungetToken(fa,&l,&c);
		//printf("statement list\n");
		if (statement()){
			//printf("here : \n");
			return statement_list();
		}
	}
	
}
int comma()
{
	printf(" comma\n");
	return getNextToken(fa, &l, &c)->ln[0] == ',' ? 1 : 0;
}
int left_s_bracket()
{
	printf("left_s_bracket \n");
	return getNextToken(fa, &l, &c)->ln[0] == '[' ? 1 : 0;
}
int right_s_bracket()
{
	printf("right_s_bracket \n");
	return getNextToken(fa, &l, &c)->ln[0] == ']' ? 1 : 0;
}
int number()
{
	printf(" number\n");
	return getNextToken(fa, &l, &c)->type==6;
}
int left_c_bracket()
{
	printf(" left_c_bracket\n");
	return getNextToken(fa, &l, &c)->ln[0] == '{' ? 1 : 0;
}
int right_c_bracket()
{
	printf("right_c_bracket \n");
	return getNextToken(fa, &l, &c)->ln[0] == '}' ? 1 : 0;
}
int left_round_bracket()
{
	printf("left_round_bracket \n");
	return getNextToken(fa, &l, &c)->ln[0] == '(' ? 1 : 0;
}
int right_round_bracket()
{
	printf(" right_round_bracket\n");
	return getNextToken(fa, &l, &c)->ln[0] == ')' ? 1 : 0;
}
int identifier_list()
{
	printf("identifier_list \n");
	t=getNextToken(fa,&l,&c);
	if(t->type!=8)
	{
		return 0;
	}
	t=getNextToken(fa,&l,&c);
	if(comma())
	{
		return identifier_list();
	}
	else if(left_s_bracket())
	{
		t=getNextToken(fa,&l,&c);
		if(number() && right_s_bracket())
		{
			if(comma())
			{
				return identifier_list();
			}
			else
			{
				ungetToken(fa,&l,&c);
				return 1;
			}
		}
		else
			return 0;
	}
	else{
		ungetToken(fa,&l,&c);
		return 1;
	}

}
int data_type(){
	printf("data_type \n");
	//t=getNextToken(fa, &l, &c);
	printf("%s\n",getNextToken(fa, &l, &c)->ln);
	char dtypes[][20]= {"int", "char", "double", "float"};
	for(int i=0; i<8; i++){
		if(strcmp(t->ln, dtypes[i])==0)
		{
			return 1;
		}
	}
	return 0;
}
int declarations()
{
	printf("declarations \n");
	t=getNextToken(fa,&l,&c);
	if(t->type==8)
	{
		return 1;
	}
	else
	{
		ungetToken(fa,&l,&c);
		return data_type() && identifier_list () && semi_colon() && declarations();
	}
}
int check_main()
{
	printf("check_main \n");
	//printf("%s\n",getNextToken(fa, &l, &c)->ln);
	return strcmp(getNextToken(fa, &l, &c)->ln,"main")==0;
}
int program()
{
	printf("program \n");
	return check_main() && left_round_bracket() && right_round_bracket()
	&& left_c_bracket() && declarations() && statement_list() && right_c_bracket();
}
int main(){
	
	
	int l=1, c=0;
	
	fa = fopen("inp.txt", "r");

	if (fa == NULL){
		//printf("Cannot open file \n");
		exit(0); 
	}
	printf("%d\n", program());
	// for(int i=0; i<19;i++)
	// {
	// 	tokenptr t;
	// 	t=getNextToken(fa,&l,&c);
	// 	printf("%s",t->ln);
	// }


}
