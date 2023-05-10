#include <stdio.h>
#include <stdlib.h>

void get_token();
void expression();
void term();
void factor();

enum
{
	NNULL,
	PLUS,
	STAR,
	NUMBER,
	LPAREN,
	RPAREN,
	END
} token;

char ch = ' ';

int main(void)
{
	get_token();
	expression();
	if (token != END)
	{
		error();
	}
	return 0;
}

void error()
{
	// error handling
	printf("Syntax error!\n");
	exit(1);
}

void get_token()
{
	while (ch == ' ')
	{
		// skip white spaces
		ch = getchar();
	}

	if (ch == '+')
	{
		token = PLUS;
		ch = getchar();
	}
	else if (ch == '*')
	{
		token = STAR;
		ch = getchar();
	}
	// else if(ch >= '0' && ch <= '9') {
	else if (isdigit(ch))
	{
		int num = 0;
		while (isdigit(ch))
		{
			num = num * 10 + (ch - '0');
			ch = getchar();
		}
		token = NUMBER;
	}
	else if (ch == '(')
	{
		token = LPAREN;
		ch = getchar();
	}
	else if (ch == ')')
	{
		token = RPAREN;
		ch = getchar();
	}
	else if (ch == '\n' || ch == EOF)
	{ // end of input
		token = END;
	}
	else
	{
		error();
	}
}

void expression()
{
	term();
	while (token == PLUS)
	{
		get_token();
		term();
	}
}

void term()
{
	factor();
	while (token == STAR)
	{
		get_token();
		factor();
	}
}

void factor()
{
	if (token == NUMBER)
	{
		get_token();
	}
	else if (token == LPAREN)
	{
		get_token();
		expression();

		if (token == RPAREN)
		{
			get_token();
		}
		else
			error();
	}
	else
		error();
}
