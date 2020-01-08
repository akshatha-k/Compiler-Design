#include <stdio.h>
#include <stdlib.h>
int main()
{
	FILE *fa, *fb;
	int ca,cb;
	fa=fopen("code1.c","r");
	if(fa==NULL)
	{
		printf("Cannot open file \n");
		exit(0);
	}
	fb=fopen("code2.c","w");
	ca=getc(fa);
	while(ca!=EOF){
		if(ca=='\t')
		{
			cb=getc(fa);
			if(cb==' ' || cb=='\t')
			{
				//ca=getc(fa);
				ca=cb;
				continue;
			}
			else{
				putc(' ',fb);
				putc(cb,fb);
			}
		}
		else if(ca==' '){
			cb=getc(fa);
			if(cb==' ')
			{
				ca=getc(fa);
				continue;
			}
			else{
				putc(' ',fb);
				putc(cb,fb);
			}
		}
		else
			putc(ca,fb);
		ca=getc(fa);
	}
	fclose(fa);
	fclose(fb);
	return 0;

}