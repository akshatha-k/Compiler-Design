%{
	int posi = 0;
	int negi = 0;
	int posf = 0;
	int negf = 0;
%}

DIGIT	[0-9]
POS_INT	[+]?{DIGIT}+
NEG_INT	[-]{DIGIT}+
POS_FLOAT	[+]?{DIGIT}+(\.{DIGIT}+)?([Ee][+-]?{DIGIT}+)?
NEG_FLOAT	[-]{DIGIT}+(\.{DIGIT}+)?([Ee][+-]?{DIGIT}+)?

%%
{POS_INT} {posi++;}
{NEG_INT} {negi++;}
{POS_FLOAT} {posf++;}
{NEG_FLOAT} {negf++;}
%%
int yywrap()
{
	return 1;
}
int main(int argc, char** argv)
{
	yylex();
	printf("PI: %8d NI: %8d PF: %8d NF: %8d\n",posi,negi,posf,negf);
}