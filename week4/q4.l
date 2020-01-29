%{

%}

%%
"scanf"	{ fprintf(yyout,"read");}
"printf"	{ fprintf(yyout,"write"); }
.	{ fprintf(yyout,"%s",yytext); }
%%
int yywrap()
{
	return 1;
}
int main(int argc, char** argv)
{
	yyin=fopen("a.txt","r");
	yyout=fopen("b.txt","w");
	yylex();
	return 0;
}