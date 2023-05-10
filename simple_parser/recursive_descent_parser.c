#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum
{
	INT,
	FLT
} kind;

struct num
{
	kind t;
	union
	{
		int i;
		float f;
	} val;
} num_value;

void get_token();
struct num expression();
struct num term();
struct num factor();
void error(int);

enum
{
	NNULL,
	PLUS,
	MINUS,
	STAR,
	SLASH,
	NUMBER,
	LPAREN,
	RPAREN,
	END
} token;

char ch = ' ';

int main(void)
{
	get_token();
	struct num result = expression();
	if (token != END)
	{
		error(3);
	}
	if (result.t == INT)
	{
		printf("%d\n", result.val.i);
	}
	else if (result.t == FLT)
	{
		printf("%f\n", result.val.f);
	}
	return 0;
}

void error(int i)
{
	switch (i)
	{
	case 1:
		printf("error: number of '(' expected. \n");
		break;
	case 2:
		printf("error: ')' expected. \n");
		break;
	case 3:
		printf("error: EOF expected. \n");
		break;
	}
	exit(1);
}

void get_token()
{
	while (ch == ' ')
	{ // skip white spaces
		ch = getchar();
	}

	if (ch == '+')
	{
		token = PLUS;
		ch = getchar();
	}
	else if (ch == '-')
	{
		token = MINUS;
		ch = getchar();
	}
	else if (ch == '*')
	{
		token = STAR;
		ch = getchar();
	}
	else if (ch == '/')
	{
		token = SLASH;
		ch = getchar();
	}
	else if (isdigit(ch))
	{
		int num = 0;
		while (isdigit(ch))
		{
			num = num * 10 + (ch - '0');
			ch = getchar();
		}

		if (ch == '.')
		{
			// float
			ch = getchar();
			float fnum = num;
			float d = 0.1;
			while (isdigit(ch))
			{
				fnum += (ch - '0') * d;
				d *= 0.1;
				ch = getchar();
			}
			num_value.t = FLT;
			num_value.val.f = fnum;
		}
		else
		{ // integer
			num_value.t = INT;
			num_value.val.i = num;
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
	{
		// end of input
		token = END;
	}
	else
	{
		error(3);
	}
}

struct num expression()
{
	struct num result = term();
	while (token == PLUS || token == MINUS)
	{
		if (token == PLUS)
		{
			get_token();
			struct num termval = term();

			if (result.t == INT && termval.t == INT)
			{
				result.val.i += termval.val.i;
			}
			else if (result.t == INT && termval.t == FLT)
			{
				result.t = FLT;
				result.val.f = result.val.i + termval.val.f;
			}
			else if (result.t == FLT && termval.t == INT)
			{
				result.val.f += termval.val.i;
			}
			else
			{
				result.val.f += termval.val.f;
			}
		}
		else if (token == MINUS)
		{
			get_token();
			struct num termval = term();

			if (result.t == INT && termval.t == INT)
			{
				result.val.i -= termval.val.i;
			}
			else if (result.t == INT && termval.t == FLT)
			{
				result.t = FLT;
				result.val.f = result.val.i - termval.val.f;
			}
			else if (result.t == FLT && termval.t == INT)
			{
				result.val.f -= termval.val.i;
			}
			else
			{
				result.val.f -= termval.val.f;
			}
		}
	}
	return result;
}

struct num term()
{
	struct num result = factor();
	while (token == STAR || token == SLASH)
	{
		if (token == STAR)
		{
			get_token();
			struct num factorval = factor();

			if (result.t == INT && factorval.t == INT)
			{
				result.val.i *= factorval.val.i;
			}
			else if (result.t == INT && factorval.t == FLT)
			{
				result.t = FLT;
				result.val.f = result.val.i * factorval.val.f;
			}
			else if (result.t == FLT && factorval.t == INT)
			{
				result.val.f *= factorval.val.i;
			}
			else
			{
				result.val.f *= factorval.val.f;
			}
		}
		else if (token == SLASH)
		{
			get_token();
			struct num factorval = factor();

			if (result.t == INT && factorval.t == INT)
			{
				result.val.i /= factorval.val.i;
			}
			else if (result.t == INT && factorval.t == FLT)
			{
				result.t = FLT;
				result.val.f = result.val.i / factorval.val.f;
			}
			else if (result.t == FLT && factorval.t == INT)
			{
				result.val.f /= factorval.val.i;
			}
			else
			{
				result.val.f /= factorval.val.f;
			}
		}
	}
	return result;
}

struct num factor()
{
	struct num result;
	if (token == NUMBER)
	{
		result = num_value;
		get_token();
	}
	else if (token == LPAREN)
	{
		get_token();
		result = expression();

		if (token == RPAREN)
		{
			get_token();
		}
		else
			error(2);
	}
	else
		error(1);

	return result;
}
