

FILE *fa, *fb,*fc,*fd;
int ca,cb,sa,sb;
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
		                 
		if(sa=='/')
		{
			sb=getc(fc);
			if(sb=='/')
			{
				while(sa !='\n'){
					sa=getc(fc);
					putc(' ',fd);        int b=10;
				}
				if(sa=='\n')
					putc('\n',fd);
			}
			else if(sb=='*')
			{
				putc(' ',fd);
				              
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
	            
	fclose(fc);
	fclose(fd);

	                  
	return 0;

}
