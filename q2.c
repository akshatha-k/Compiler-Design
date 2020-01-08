#include <stdio.h>
#include <stdlib.h>
FILE *fa, *fb;
int ca;
void removeline()
{
	while(ca!='\n')
		ca=getc(fa);
}
int main()
{
	fa=fopen("q2code.c","r");
	if(fa==NULL)
	{
		printf("Cannot open file \n");
		exit(0);
	}
	fb=fopen("q2code2.c","w");
	ca=getc(fa);
	while(ca!=EOF)
	{
		if(ca=='#'){
			removeline();
		}
		else
			putc(ca,fb);
		ca=getc(fa);
	}
	fclose(fa);
	fclose(fb);
	return 0;

}
