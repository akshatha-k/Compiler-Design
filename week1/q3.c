#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int main()
{
	FILE *fa, *fb;
	int lno=1,wcolno,colno=1,j=0;
	int ca,cb,low,high,mid;
	const char *keywords[] = {
    "auto", 
    "break", 
    "case", 
    "char", 
    "continue", 
    "do", 
    "default", 
    "const", 
    "double", 
    "else", 
    "enum", 
    "extern", 
    "for", 
    "if", 
    "goto", 
    "float", 
    "int", 
    "long", 
    "register", 
    "return", 
    "signed", 
    "static", 
    "sizeof", 
    "short", 
    "struct", 
    "switch", 
    "typedef", 
    "union", 
    "void", 
    "while", 
    "volatile", 
    "unsigned"
};
	char str[100];
	fa=fopen("q3.c","r");
	if(fa==NULL)
	{
		printf("Cannot open file \n");
		exit(0);
	}
	fb=fopen("b.c","w");
	ca=getc(fa);
	while(ca!=EOF)
	{
		if(!isalpha(ca)){
			ca=getc(fa);
			colno++;
			continue;
		}
		str[j++]=ca;
		wcolno=colno;
		cb=getc(fa);
		//ungetc(cb,fa);
		//printf("here \n");
		while(cb!=' '&& cb!='\t'&&cb!='(' && cb!='\n' && cb!=EOF)
		{
			str[j++]=cb;
			cb=getc(fa);
			//getchar();
		}
		if(cb==' '|| cb=='\t'||cb=='(' || cb=='\n' || cb==EOF)
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
					fprintf(fb,"%s %d %d\n",str,lno,wcolno);
					break;
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
			 j=0;
			 memset(str,0,sizeof(str));
			 ca=getc(fa);
		}
		if(cb=='\n'){
			lno++;
			colno=0;
		}
	}
	fclose(fa);
	fclose(fb);
	return 0;

}
