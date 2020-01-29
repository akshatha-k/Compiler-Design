%{
%}

%%


[a-zA-Z ]+(" and "|" but "|" or ")[a-zA-Z]+	{printf("Compund\n");}
[a-zA-Z ]+	{printf("Simple\n");}
. ;
%%

int yywrap() {
	return 1;
}

int main(int argc, char *argv[]) {
	yylex();
	return 0;
}
