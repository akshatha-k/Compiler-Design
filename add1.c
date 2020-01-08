#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
int main()
{
	FILE *fa, *fb;
	int lno=1;
	int ca,cb,low,high,mid;
	fa=fopen("add1.c","r");
	if(fa==NULL)
	{
		printf("Cannot open file \n");
		exit(0);
	}
	fb=fopen("b.c","w");
	ca=getc(fa);
	fprintf(fb,"1");
	while(ca!=EOF)
	{
		fprintf(fb,"%c",ca);
		if(ca=='\n'){
			lno++;
			fprintf(fb,"%d",lno);
	}
	ca=getc(fa);
	}
	fclose(fa);
	fclose(fb);
	return 0;

}
