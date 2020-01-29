%{
	int vowels=0;
	int cons=0;
	int tot=0;
%}

%%
[aeiouAEIOU]	{ vowels++; }
[a-zA-Z]	{tot++;}
%%
int yywrap()
{
	return 1;
}
int main(int argc, char **argv)
{
	yylex();
	printf("Vowels: %d Consonants: %d",vowels, tot);
}