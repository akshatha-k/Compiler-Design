#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define LOCAL 0
#define GLOBAL 1
#define LENGTH 10

struct Token{
	char ln[100];
	int row, col, type;
};
typedef struct Token* tokenptr;

char data_types[][15]={"int", "int*","char", "char*", "float","float*", "double", "double*","long", "long*", "short", "short*"};
int data_sizes[]={4,8,1,2,sizeof(float),sizeof(float*),sizeof(double),sizeof(double*),sizeof(long),sizeof(long*),sizeof(short),sizeof(short*)};
typedef struct tableElement{
	tokenptr tok;
	int id;
	char name[20];
	char type[10];
	int size;
	char rettype[10];
	int scope;
	int argument;
}tableElement;


int st_c=0;

int current_scope=GLOBAL;
char last_keyword[10];

void Display(tableElement table[20])
{
	printf("SYMBOL TABLE  : \n");

	for(int i=0; i<st_c;i++)
	{
		printf("id: %d  n:%s  t:%s  si:%d  ret:%s sc:%d arg:%d \n",table[i].id,table[i].name,table[i].type,table[i].size, table[i].rettype,table[i].scope,table[i].argument);
	}

}

int search(tableElement table[20], char str[])
{
	for(int i=0; i<st_c;i++)
	{
		if(strcmp(table[i].name,str)==0)
			return table[i].id;
	}
	return -1;
}

void insert(tableElement table[20],tableElement entry )
{
	if(search(table, entry.name)!=-1)
		return;
	int i=st_c;
	table[i].id=i;
	strcpy(table[i].name,entry.name);
	strcpy(table[i].type,entry.type);
	table[i].size=entry.size;
	strcpy(table[i].rettype,entry.rettype);
	table[i].scope=entry.scope;
	table[i].argument=entry.argument;
	st_c++;

}

tokenptr getNextToken(FILE *fa, int *l, int *c){
	char a[100], ch, temp, i=0, keys[][32]= {"auto","break","case","char","continue","do", "default", "const", "double", "else", "enum", "extern", "for", "if", "goto", "float", "int", "long", "register", "return", "signed", "static", "sizeof", "short", "struct", "switch", "typedef", "union", "void", "while", "volatile", "unsigned"};
	tokenptr newToken=(tokenptr)malloc(sizeof(struct Token));
	ch = getc(fa);
	if(ch==EOF){
		newToken = NULL;
		return newToken;
	}
	a[i]=ch;
	i++;
	newToken->row = *l;
	newToken->col = *c;

	if(ch == '\n'){
		*c=0;
		(*l)++;
		newToken->type = 0;
		return newToken;
	}

	if(ch == '#'){
		ch = getc(fa);
		while(ch!='\n')
			ch = getc(fa);
		(*l)++;
		newToken->type = 0;
		return newToken;
	}

	temp=ch;
	if(temp=='/'){
		temp = getc(fa);
		if(temp == '*'){
			*c+=2;
			while(1){
				temp = getc(fa);
				(*c)++;
				if(temp == '\n'){
					*c = 0;
					(*l)++;
				}
				if(temp == '*'){
					char t = getc(fa);
					if(t == '/'){
						(*c)++;
						newToken->type = 0;
						return newToken;
					}
					fseek(fa, -1, SEEK_CUR);
				}
			}
		}
		else if(temp == '/'){
			*c+=2;
			while(1){
				temp = getc(fa);
				(*c)++;
				if(temp == '\n'){
					*c = 0;
					(*l)++;
					newToken->type = 0;
					return newToken;
				}
			}
		}
	}

	if(ch == '"'){
		(*c)++;
		ch = getc(fa);
		while(ch!='"'){
			(*c)++;
			a[i]=ch;
			i++;
			ch = getc(fa);
		}
		a[i]=ch;
		a[i+1]='\0';
		strcpy(newToken->ln, a);
		newToken->type = 7;
		return newToken;
	}

	if(ch == ' ' || ch =='\t'){
		(*c)++;
		newToken->type = 0;
		return newToken;
	}

	if(ch=='+'||ch=='-'||ch=='/'||ch=='*'||ch=='%'){
		(*c)++;
		ch=getc(fa);
		if(ch=='='){
			a[i] = ch;
			i++;
			(*c)++;
			a[i] = '\0';
			newToken->type = 9;
			strcpy(newToken->ln, a);
			return newToken;
		}
		else{
			a[i]='\0';
			newToken->type = 1;
			strcpy(newToken->ln, a);
			fseek(fa, -1, SEEK_CUR);
			return newToken;
		}
	}

	if(ch == '<' || ch == '>' || ch == '!' || ch == '='){
		(*c)++;
		ch = getc(fa);
		if(ch=='='){
			a[i] = ch;
			i++;
			(*c)++;
			a[i] = '\0';
			newToken->type = 2;
			strcpy(newToken->ln, a);
			return newToken;
		}
		else{
			a[i]='\0';
			if(a[0]=='!')
				newToken->type = 3;
			else if(a[0]=='=')
				newToken->type = 10;
			else
				newToken->type = 2;
			strcpy(newToken->ln, a);
			fseek(fa, -1, SEEK_CUR);
			return newToken;
		}
	}

	if(isalpha(ch)||ch=='_'){
		(*c)++;
		ch = getc(fa);
		while(isalpha(ch)||ch=='_'||(ch>='0'&&ch<='9')){
			a[i]=ch;
			i++;
			(*c)++;
			ch = getc(fa);
		}
		a[i]='\0';
		fseek(fa, -1, SEEK_CUR);
		strcpy(newToken->ln, a);
		for(int i=0; i<32; i++)
			if(strcmp(a, keys[i])==0){
				newToken->type = 5;
				return newToken;
			}
		newToken->type = 8;
		return newToken;
	}

	if(isdigit(ch)){
		(*c)++;
		ch = getc(fa);
		while(isdigit(ch)||ch=='.'){
			a[i]=ch;
			i++;
			(*c)++;
			ch = getc(fa);
		}
		a[i]='\0';
		fseek(fa, -1, SEEK_CUR);
		strcpy(newToken->ln, a);
		newToken->type = 6;
		return newToken;
	}

	a[i]='\0';
	strcpy(newToken->ln, a);
	newToken->type = 4;
	return newToken;
}



int main(){
	tableElement table,symb[50];
	FILE *fa;
	char types[][20]={"No_Token", "Arithmetic_Operator", "Relational_Operator", "Logical_Operator", "Special_Symbol", "Keyword", "Number_Constant", "String_Literal", "Identifier", "Shorthand_Operator", "Assignment_Operator"};
	int l=1, c=0,i=0,bracket=0;

	char inname[100],last_identifier[100], last_identifier_type[100];
	//printf("Enter input filename: ");
	//scanf(" %s", inname);

	//fa = fopen(inname, "r");
	fa = fopen("program.c", "r");

	if (fa == NULL){
		printf("Cannot open file \n");
		exit(0);
	}
	tokenptr temp, next_temp;
	do{
		temp = getNextToken(fa, &l, &c);
		if(temp==NULL)
			break;
		if(bracket==1)
			current_scope=LOCAL;
		else if(bracket==0)
			current_scope=GLOBAL;
		if(strcmp(types[temp->type],"Identifier")==0)
		{
			strcpy(table.name,temp->ln);
			next_temp= getNextToken(fa, &l,&c);
			if(strcmp(next_temp->ln,"(")==0)
			{
				strcpy(table.type,"FUNC");
				if(strcmp(last_identifier_type,"STRUCT")==0)
				{
					strcpy(table.type,last_identifier);
				}
				else
					strcpy(table.rettype,last_keyword);
				next_temp=getNextToken(fa,&l,&c);
				if(strcmp(next_temp->ln,")")==0)
				{
					table.argument=-1;
				}
				else
					table.argument=st_c+1;
			}
			if(strcmp(next_temp->ln,",")==0 || strcmp(next_temp->ln,";")==0 || strcmp(next_temp->ln,"[")==0)
			{
				if(strcmp(last_identifier_type,"STRUCT")==0)
				{
					strcpy(table.type,last_identifier);
				}
				else{
					strcpy(table.type,last_keyword);
					for(int i=0;i<12;i++)
					{
						if(strcmp(table.type,data_types[i])==0)
						{
							table.size=data_sizes[i];
						}
					}
				}
			}
			if(strcmp(next_temp->ln,"{")==0)
			{
				strcpy(table.type,"STRUCT");

			}
			table.scope= current_scope;
			strcpy(last_identifier, table.name);
			strcpy(last_identifier_type,table.type);
			insert(symb,table);
		}

		else if(strcmp(temp->ln,"{")==0 || strcmp(temp->ln,"(")==0)
		{
			bracket++;
		}
		else if(strcmp(temp->ln,"}")==0 || strcmp(temp->ln,")")==0)
			bracket--;
		else if(strcmp(types[temp->type],"Keyword")==0)
		{
			strcpy(last_keyword, temp->ln);
		}


	}while(temp!=NULL);
	Display(symb);
}
