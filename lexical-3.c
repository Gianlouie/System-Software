// Gianlouie Molinary gi713278
// COP 3402 Spring 2018
// This work is mine, and mine alone

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef enum token
{
	nulsym = 1, identsym = 2, numbersym = 3, plussym = 4, minussym = 5,
	multsym = 6, slashsym = 7, oddsym = 8, eqlsym = 9, neqsym = 10,
	lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14, lparentsym = 15,
	rparentsym = 16, commasym = 17, semicolonsym = 18, periodsym = 19,
	becomessym = 20, beginsym = 21, endsym = 22, ifsym = 23, thensym = 24,
	whilesym = 25, dosym = 26, callsym = 27, constsym = 28, varsym = 29,
	procsym = 30, writesym = 31, readsym = 32, elsesym = 33
}token_type;

char fileInput[1000];
int charNum = 0;
int position = 0;
int lexVal;
int id = 0;
int length;
int idholder[100];
char singleholder[12][12];

int identifier(char c)
{
	switch(c)
	{
		case 'a'...'z':
		case '0'...'9':
		case '+':
		case '-':
		case '*':
		case '/':
		case '<':
		case '=':
		case '>':
		case ':':
		case '(':
		case ')':
		case '.':
		case ';':
		case ',':
			return 1;
		default:
			return 0;
	}

	return 0;
}

int tokenSym(char* token)
{
	int ASCIIval = 0;
	int max = 99999;
	int num = 0;
	int i = 0;

	for(i = 0; token[i]; i++)
		token[i] = tolower(token[i]);

	i = 0;
	switch(*token)
	{
		case '+':
			return plussym;
		case '-':
			return minussym;
		case '*':
			return multsym;
		case '/':
			return slashsym;
		case '<':
			if (token[i+1] == '=')
				return leqsym;
			else if (token[i+1] == '>')
				return neqsym;
			else
				return lessym;
		case '=':
			return eqlsym;
		case '>':
			if (token[i+1] == '=')
				return geqsym;
			else
				return gtrsym;
		case ':':
			if (token[i+1] == '=')
				return becomessym;
			else
				return 0;
		case '(':
			return lparentsym;
		case ')':
			return rparentsym;
		case ',':
			return commasym;
		case '.':
			return periodsym;
		case ';':
			return semicolonsym;
		case 'a'...'z':
			if(strcmp(token, "odd") == 0)
				return oddsym;
			if(strcmp(token, "begin") == 0)
				return beginsym;
			if(strcmp(token, "end") == 0)
				return endsym;
			if(strcmp(token, "if") == 0)
				return ifsym;
			if(strcmp(token, "then") == 0)
				return thensym;
			if(strcmp(token, "while") == 0)
				return whilesym;
			if(strcmp(token, "do") == 0)
				return dosym;
			if(strcmp(token, "call") == 0)
				return callsym;
			if(strcmp(token, "const") == 0)
				return constsym;
			if(strcmp(token, "var") == 0)
				return varsym;
			if(strcmp(token, "procedure") == 0)
				return procsym;
			if(strcmp(token, "write") == 0)
				return writesym;
			if(strcmp(token, "read") == 0)
				return readsym;
			if(strcmp(token, "else") == 0)
				return elsesym;
			return identsym;
		case '1'...'9':
			/*if(isalpha(token[1]))
			{
				printf("Error: Invalid identifier.");
				exit(0);
			}*/

			if(strlen(token) > 12)
				printf("Error: Identifier too long.");

			num = atoi(token);

			if(abs(num) > max)
			{
				printf("Error: Number too large.");
				exit(0);
			}

			else
				return numbersym;
		default:
			break;
	}

	return 0;
}

void readInput(FILE *input)
{
	while (!feof(input))
		fscanf(input, "%c", &fileInput[charNum++]);

	fclose(input);
}

void printSource(char *argv[])
{
	fprintf(stdout, "Source Program:%s\n",argv[1]);
	fprintf(stdout, "%s\n", fileInput);
}

char *tokenPiece(char fileInput[])
{
    id = 0;
    char *single;
    single = (char*)malloc(charNum);
    int i = 0;
    length = 0;
	
    do
	{
        switch (fileInput[position]) 
		{
            case '(':
            case ')':
            case ',':
            case '.':
            case ';':
            case '+':
            case '-':
            case '*':
            case '/':
                if (i != 0)
                    return (char*)single;
				
				else
				{
					length = 1;
                    single[i] = fileInput[position];
                    position++;
                    return (char*)single;
                }

            case '<':
                if (i != 0)
                    return (char*)single;
				
				else
				{
                    if (fileInput[position+1] != '=' || fileInput[position+1] != '>') 
					{
                        single[i] = fileInput[position];
                        position++;
						length = 1;
                        return (char*)single;
                    }
					
					else
					{
						length = 2;
                        single[i] = fileInput[position];
                        single[i+1] = fileInput[position+1];
                        position = position+2;
                        return (char*)single;
                    }
                }

            case ':':
                if (i != 0)
                    return (char*)single;
				
				else
				{
                    if (fileInput[position+1] != '=') 
					{
                        single[i] = fileInput[position];
                        position++;
                        length = 1;
                        return (char*)single;
                    }
					
					else
					{
                        single[i] = fileInput[position];
                        single[i+1] = fileInput[position+1];
                        position = position+2;
                        length = 2;
                        return (char*)single;
                    }
                }
				
            case '>':
                if (i != 0) 
                    return (char*)single;
				
				else
				{
                    if (fileInput[position+1] != '=') 
					{
                        length = 1;
                        single[i] = fileInput[position];
                        position++;
                        return (char*)single;
                    }
					
					else
					{
						length = 2;
                        single[i] = fileInput[position];
                        single[i+1] = fileInput[position+1];
                        position = position+2;
                        return (char*)single;
                    }
                }
				
            case '\t':
                if(strcmp(single, "\t") == 0)
				{
                    id = 1;
                    position++;
                    return (char*)single;
                }
                position++;
                return (char*)single;
				
            case '\r':
                if(strcmp(single, "\r") == 0)
				{
                    id = 1;
                    position++;
                    return (char*)single;
                }
                position++;
                return (char*)single;
				
            case '\n':
                if(strcmp(single, "\n") == 0)
				{
                    id = 1;
                    position++;
                    return (char*)single;
                }
				
                position++;
                return (char*)single;
				
            case 'a'...'z':
            case 'A'...'Z':
            case '1'...'9':
                single[i] = fileInput[position];
                position++;
                i++;
                length++;
                break;
            default:
                id = 1;
                position++;
                return (char*)single;

        }
    }
	
	while(fileInput[position] != ' ');

    position++;
    return (char*)single;
}

int main(int argc, char*argv[])
{
	int j = 0, i;
	FILE *input = fopen(argv[1], "r");

	if(input == NULL)
	{
		printf("Error in opening file");
		exit(0);
	}

	readInput(input);
	printSource(argv);

	char *single = (char*)malloc(charNum);
	char *token = (char*)malloc(charNum);

	fprintf(stdout, "\nLexeme Table:\n");
	fprintf(stdout, "lexeme\t\ttoken type\n");

	while(position < charNum)
	{
		length = 0;
		single = tokenPiece(fileInput);
		*token = '\0';
		
		strcpy(singleholder[j], single);
		
		if(strlen(single) > length)
		{
			for(i = 0; i < length; i++)
				token[i] = single[i];
			single = token;
		}
		
		if(strlen(single) > 12)
		{
			printf("Error: identifier too long");
			exit(0);
		}

		int i;
			if(id == 0)
			{
				lexVal = tokenSym(single);
				if(lexVal != 0)
				{
					for(i = 0; i < 12; i++)
						if(i < length)
							if(identifier(single[i]))
							{
								printf("%c", single[i]);
							}
							else
								printf(" ");
						else
							printf(" ");
						printf("%\t\t%d\n", lexVal);
						idholder[j] = lexVal;

					j++;
				}
			}
	}

	fprintf(stdout, "\nLexeme List:\n");
	for(i = 0; i < j; i++)
	{
		fprintf(stdout, "%d ", idholder[i]);

		if(idholder[i] == 2)
			fprintf(stdout, "%s ", singleholder[i]);
	}
}
