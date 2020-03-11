#include "lex.h"
#include <string.h>
FILE *fa;
int l=1,c=0;
tokenptr t;
void mulop();
void addop();
void relop();
void looping_stat();
void dprime();
void decision_stat();
void factor();
void tprime();
void term();
void seprime();
void simple_expn();
void eprime();
void expn();
void assign_stat();
void statement();
void statement_list();
void identifier_list();
void idListPrime();
void idListPrime2();
void data_type();
void declarations();
void program();

void error()
{
    fprintf(stderr,"ERROR - token :%s at line :%d , column :%d \n",t->ln, t->row,t->col);
    //exit(1);
}
void mulop()
{
	t=getNextToken(fa, &l, &c);
    printf("mulop %s \n",t->ln);
	if(t->ln[0]=='/'||t->ln[0]=='*'||t->ln[0]=='%')
			return;
	else{
		error();
		return;
	}
}

void addop()
{
	t= getNextToken(fa, &l, &c);
    printf("addop %s\n",t->ln);
	if(t->ln[0]=='+'||t->ln[0]=='-')
			return;
	else{
		error();
		return;
	}
}
void looping_state()
{
    t= getNextToken(fa, &l, &c);
    printf("looping_state with %s\n", t->ln);
    if(!strcmp(t->ln,"while") == 0 || !strcmp(t->ln,"for") == 0 )
    {
    	error();
    }
	if(strcmp(t->ln,"while") == 0) {
		
	}
	t= getNextToken(fa, &l, &c);
	if(strcmp(t->ln,"(") == 0) {
		
	}
	expn();
	t= getNextToken(fa, &l, &c);
	if(strcmp(t->ln,")") == 0) {
		
	}
	t= getNextToken(fa, &l, &c);
	if(strcmp(t->ln,"{") == 0) {
		
	}
	statement_list();
	t= getNextToken(fa, &l, &c);
	if(strcmp(t->ln,"}") == 0) {
		error();
	return;


	else if(strcmp(t->ln,"for") == 0) {
		t= getNextToken(fa, &l, &c);
		if(strcmp(t->ln,"(") == 0) {
			assign_stat();
			t= getNextToken(fa, &l, &c);
			if(strcmp(t->ln,";") == 0) {
				expn();
				t= getNextToken(fa, &l, &c);
				if(strcmp(t->ln,";") == 0) {
					assign_stat();
					t= getNextToken(fa, &l, &c);
					if(strcmp(t->ln,")") == 0) {
						t= getNextToken(fa, &l, &c);
						if(strcmp(t->ln,"{") == 0) {
							statement_list();
							t= getNextToken(fa, &l, &c);
							if(strcmp(t->ln,"}") == 0) {
								return;
							}
							else{
								error();
								return;
							}
						}
						else{
							error();
							return;
						}
					}
					else{
						error();
						return;
					}
				}
				else{
					error();
					return;
				}
			}
			else{
				error();
				return;
			}
		}
		else{
			error();
			return;
		}
	}
	else{
		error();
		return;
	}
}

void relop()
{
	t= getNextToken(fa, &l, &c);
    printf("relop %s\n",t->ln);
	if(t->type==2)
	{
		error();
	}
	return;
}
void dprime()
{
    t= getNextToken(fa, &l, &c);
    printf("dprime %s\n",t->ln);
	if(strcmp(t->ln,"else") == 0) {
		t= getNextToken(fa, &l, &c);
		if(!t->ln[0]=='{') {
			error();
		}
		statement_list();
		t= getNextToken(fa, &l, &c);
		if(!t->ln[0]=='}') {
			error();
		}
		return;
	}
	else
		ungetToken(fa,&l,&c);
}
void decision_stat()
{
    t=getNextToken(fa, &l, &c);
    printf("decision_state with %s \n",t->ln);
    if(!strcmp(t->ln,"if")==0)
    {
        error();
    }
    t=getNextToken(fa, &l, &c);
    if(!t->ln[0]=='(')
    {
        
        error();
    }
    expn();
    t=getNextToken(fa, &l, &c);
    if(!strcmp(t->ln,")")==0)
    {
        error();
        
    }
    t=getNextToken(fa, &l, &c);
    if(!t->ln[0]=='{')
    {
        
        error();
    }
    statement_list();
    t=getNextToken(fa, &l, &c);
    if(!t->ln[0]=='}')
    {
        error();
    }

    dprime();
}
void factor()
{
	t= getNextToken(fa, &l, &c);
    printf("factor %s\n",t->ln);
	//printf("factor %s\n",  types[tp->type]);
	if(!t->type==8 || !t->type==6)
		error();
	return;

}

void tprime()
{
	t=getNextToken(fa,&l,&c);
    printf("tprime %s\n",t->ln);
    if(t->ln[0]=='*'|| t->ln[0]=='/'||t->ln[0]=='%')
    {
        ungetToken(fa,&l,&c);
        mulop();
        factor();
        tprime();
    }
    else
    {
        ungetToken(fa,&l,&c);
    }
    
}

void term()
{
	printf("term\n");
	factor(); 
    tprime();
}

void seprime()
{
    t=getNextToken(fa,&l,&c);
    printf("seprime %s\n",t->ln);
    if(t->ln[0]=='+'|| t->ln[0]=='-')
    {
        ungetToken(fa,&l,&c);
        addop();
        term();
        seprime();
    }
    else
    {
        ungetToken(fa,&l,&c);
    }
}

void simple_expn()
{
	printf("simp_expn\n");
	term(); 
    seprime();
}

void eprime()
{
    t=getNextToken(fa, &l, &c);
    printf("eprime %s\n",t->ln);
	if(t->ln[0]=='<'|| strcmp(t->ln,"<=")==0||t->ln[0]=='>'|| strcmp(t->ln,">=")==0||strcmp(t->ln,"==")==0||strcmp(t->ln,"!=")==0)
    {
        ungetToken(fa,&l,&c);
        relop();
        simple_expn();
    }
    else
    {
        ungetToken(fa,&l,&c);
    }
}

void expn()
{
	printf("expn \n");
	simple_expn(); 
    eprime();
}

void assign_stat()
{
    t=getNextToken(fa, &l, &c);
    printf("assign_Stat with %s \n",t->ln);
    if(!t->type==8)
    {
        error(); 
    }
    t=getNextToken(fa,&l,&c);
    if(!t->ln[0]=='=')
    {
        expn();
    }
    return;
    
}

void statement()
{
	t = getNextToken(fa,&l,&c);
    printf("statement with %s \n",t->ln);
    if(!t->type==8 || !strcmp(t->ln,"if") == 0 || !strcmp(t->ln,"while") == 0 || !strcmp(t->ln,"for") == 0)
    {
    	error();
    }
	if(t->type==8) {
		ungetToken(fa,&l,&c);
		assign_stat();
		t = getNextToken(fa,&l,&c);
		if(!t->ln[0]==';') {
			error();
		}
		else{
			
			return;
		}
	}
	else if(strcmp(t->ln,"if") == 0) {
		ungetToken(fa,&l,&c);
		decision_stat();
	}
	else if(strcmp(t->ln,"while") == 0 || strcmp(t->ln,"for") == 0) {
		ungetToken(fa,&l,&c);
		looping_state();
	}

}
void statement_list()
{
	t = getNextToken(fa,&l,&c);
    printf("statement list with %s\n",t->ln);
	if(t->type==8 || strcmp(t->ln,"if") == 0 || strcmp(t->ln,"while") == 0 || strcmp(t->ln,"for") == 0) {
		ungetToken(fa,&l,&c);
		statement();
		statement_list();
	}
	else
		ungetToken(fa,&l,&c);
}

// identifier-list -> id identifier-list'
void identifier_list() {
	t = getNextToken(fa,&l,&c);
	printf("IdentifierList with %s\n",t->ln);
	if(!t->type==8) {
		error();
	}
	idListPrime();
}

// identifier-list' -> , identifier-list | [number] identifier-list'' | (epsilon)
void idListPrime() {
	t = getNextToken(fa,&l,&c);
	printf("idListPrime with %s\n",t->ln);
	if(t->ln[0]==',') {
		identifier_list();
	}
	else if (t->ln[0]=='[') {
		t = getNextToken(fa,&l,&c);
		//printf("idListPrime with %s\n",t->ln);
		if (!t->type == 6) {
			error();
		}
		t = getNextToken(fa,&l,&c);
		//printf("idListPrime with %s\n",t->ln);
		if (!t->ln[0]==']') {
			error();
		}
		idListPrime2();
	}
	else
		ungetToken(fa,&l,&c);
}

// identifier-list'' -> (epsilon) | , identifier-list
void idListPrime2() {
	t = getNextToken(fa,&l,&c);
	printf("idListPrime2 with %s\n",t->ln);
	if(t->ln[0]==',') {
		identifier_list();
	}
	else
		ungetToken(fa,&l,&c);
}

void data_type(){
	t=getNextToken(fa, &l, &c);
    printf("data_type %s\n",t->ln);
	//printf("%s\n",getNextToken(fa, &l, &c)->ln);
	char dtypes[][20]= {"int", "char", "double", "float"};
	for(int i=0; i<4; i++){
		if(!strcmp(t->ln, dtypes[i])==0)
		{
			error();
		}
	}

}
void declarations()
{
	t=getNextToken(fa,&l,&c);
    printf("declarations %s\n",t->ln);
	char dtypes[][20]= {"int", "char", "double", "float"};
	for(int i=0; i<4; i++){
		if(strcmp(t->ln, dtypes[i])==0)
		{
			ungetToken(fa,&l,&c);
            data_type();
            identifier_list();
            t=getNextToken(fa,&l,&c);
            if(!t->ln[0]==';')
            {
               error();
            }
            declarations();
            
		}
        else
        {
            ungetToken(fa,&l,&c);
        }
        
	}
}
void program()
{
    t=getNextToken(fa,&l,&c);
    printf("program %s \n",t->ln);
	if(strcmp(t->ln,"main")==0)
    {
    	error();
    }
     t=getNextToken(fa,&l,&c);
     if(!t->ln[0]=='(')
     {
     	error();
     }
    t=getNextToken(fa,&l,&c);
    if(!t->ln[0]==')')
    {
    	 error();
    }
    t=getNextToken(fa,&l,&c);
    if(!t->ln[0]=='{')
    {
            
        error();    
    }
    declarations();
    statement_list();
    t=getNextToken(fa,&l,&c);
    if(!t->ln[0]=='}'){
        error();
    } 

}
int main(){
	
	
	int l=1, c=0;
	
	fa = fopen("prog.c", "r");

	if (fa == NULL){
		//printf("Cannot open file \n");
		exit(0); 
	}
	program();
	// for(int i=0; i<19;i++)
	// {
	// 	tokenptr t;
	// 	t=getNextToken(fa,&l,&c);
	// 	printf("%s",t->ln);
	// }


}