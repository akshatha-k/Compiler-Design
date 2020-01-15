#include <stdio.h>
#include <stdlib.h>
#include <string.h>
FILE *fa, *fb,*fc,*fd;
int ca,cb,sa,sb;
#define clrst memset(str,0,sizeof(str));
#define rrn t->rno=rno;
#define rcn t->cno=cno;
#define ret return t;
const char *keywords[] = {"auto","break","case","char","continue","do", "default", "const", "double", "else", "enum", "extern", "for", "if", "goto", "float", "int", "long", "register", "return", "signed", "static", "sizeof", "short", "struct", "switch", "typedef", "union", "void", "while", "volatile", "unsigned"};
enum TokenType{KEYWORD, IDENTIFIER, SS, AR_OP, REL_OP, L_OP, STRING,NUM};
typedef struct token{
	char name[20];
	unsigned int rno,cno;
	enum TokenType type;
}token; 

void removeline()
{
	while(ca!='\n')
		ca=getc(fa);
	putc('\n',fb);
}
int main()
{
	
	fa=fopen("q.c","r");
	if(fa==NULL)
	{
		printf("Cannot open file \n");
		exit(0);
	}
	fb=fopen("lexout.c","w");
	ca=getc(fa);
	while(ca!=EOF)
	{
		if(ca=='#'){
			removeline();
		}
		else
			break;
		ca=getc(fa);
	}
	while(ca!=EOF)
	{
		putc(ca,fb);
		ca=getc(fa);
	}
	fclose(fb); 
	fc=fopen("lexout.c","r");
	if(fc==NULL)
	{
		printf("Cannot open file \n");
		exit(0);
	}
	fd=fopen("lex_rem_comm.c","w");
	sa=getc(fc);
	while(sa!=EOF)
	{
		//printf("%c",sa);
		if(sa=='/')
		{
			sb=getc(fc);
			if(sb=='/')
			{
				while(sa !='\n'){
					sa=getc(fc);
					putc(' ',fd); /*ad*/ int b=10;
				}
				if(sa=='\n')
					putc('\n',fd);
			}
			else if(sb=='*')
			{
				putc(' ',fd);
				//putc(' ',fd);
				do
				{
					while(sa!='*')
					{
						sa=getc(fc);
						putc(' ',fd);
					}
					sa=getc(fc);
				}while(sa!='/');
				putc(' ',fd);
				putc(' ',fd);
			}
			else
			{
				putc(sa,fd);
				putc(sb,fd);
			}
		}
		else putc(sa,fd);
		sa=getc(fc);
	}
	fclose(fa);
	//fclose(fb);
	fclose(fc);
	fclose(fd);
	return 0;

}

token getNextToken()
{
	token t;
	int lno=1,cno=1,wcolno,j=0;
	char ma,mb,low,high,mid;
	char str[100];
	fd=fopen("lex_rem_comm.c","r");
	if(fd==NULL)
	{
		printf("Cannot open file \n");
		exit(0);
	}
	ma=getc(fd);
	while(ma!=EOF)
	{
		if(isalpha(ma))
		{
			if(ma=='"')
			{
				t->rno=wcolno;
				mb=getc(fd);
				cno++;
				while(mb!='"')
				{
					str[j++]=mb;
					mb=getc(fd);
				}
				if(mb=='"')
				{
					t->name=strcpy(t->name,str);
					rc;
					cc;
					t->type=STRING;
					ret;
				}
			}
			//printf("here \n");
			while(mb!=' '&& mb!='\t'&&mb!='(' && mb!='\n' && mb!=EOF)
			{
				str[j++]=mb;
				mb=getc(fa);
				//getchar();
			}
			if(mb==' '|| mb=='\t'||mb=='(' || mb=='\n' || mb==EOF)
			{
				str[j++]='\0';
				//printf("%s\n",str);
				low=0;
				high=32-1;
				while(low<=high)
				{
					mid=(low+high)/2;
					if (strcmp(str,keywords[mid])==0)
					{
						t->type=KEYWORD;
						rc;
						cc;
						rt;
					}
					else if(strcmp(str,keywords[mid])>0)
					{
						 high=high;
						 low=mid+1;
					}
					else
					{
						 low=low;
						 high=mid-1;
					}
				}
				if(low>high)
				{
					t->type=IDENTIFIER;
				}
				 j=0;
				 clrst;
				 ma=getc(fa);
			}
		}
		
		else if(!isalpha(ma))
		{
			while(cb!=' '&& cb!='\t'&&cb!='(' && cb!='\n' && cb!=EOF)
			{
				str[j++]=cb;
				cb=getc(fa);
			}
		}
		else if(ma=='\n')
		{
			lno++;
			cno=0;
		}
		ma=getc(fd);
		fclose(fd);
	}
}
	
