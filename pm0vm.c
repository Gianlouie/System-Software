// Gianlouie Molinary gi713278
// COP 3402 Spring 2018
// Homework #1 (P-Machine) 
// This work is mine, and mine alone
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

typedef struct instruction {
	int op;
	int r;
	int l;
	int m;
}instruction;

int stack[MAX_STACK_HEIGHT];
instruction instructions[MAX_CODE_LENGTH];
int register_file[15];

const char* ISA[23] = {"", "LIT", "RTN", "LOD", "STO", "CAL", "INC", "JMP", "JPC", "SIO", "NEQ", "ADD", 
						"SUB", "MUL", "DIV", "ODD", "MOD", "EQL", "NEQ", "LSS", "LEQ", "GTR", "GEQ"};
int sp = 0;
int bp = 1;
int pc = 0;
int lines = 0;
int halt_flag = 0;
int lex = 0;

instruction ir;

int base(int l, int base) // from the assignment pdf
{
	int b1 = base;
	
	while (l > 0)
	{
		b1 = stack[b1 + 1];
		l--;
	}
	
	return b1;
}

void fetchCycle() // fetches the next instruction to be executed
{
	ir = instructions[pc];
	pc++;
}

void executeCycle()
{
	switch (ir.op)
	{
		case 1: // LIT
			register_file[ir.r] = ir.m;
			break;
		case 2: // RTN
			sp = bp - 1;
			bp = stack[sp + 3];
			pc = stack[sp + 4];
			lex--;
			break;
		case 3: // LOD 
			register_file[ir.r] = stack[(base(ir.l, bp) + ir.m)-1]; // same here as below with STO 
			break;
		case 4: // STO 
			stack[(base(ir.l, bp) + ir.m)-1] = register_file[ir.r];
			// sto was a problem, I think because of the pseudocode being incorrect in the pdf
			break;
		case 5: // CAL 
			stack[sp + 1] = 0;
			stack[sp + 2] = base(ir.l, bp);
			stack[sp + 3] = bp;
			stack[sp + 4] = pc;
			bp = sp + 1;
			sp = sp + 4;
			pc = ir.m;
			lex++;
			break;
		case 6:  // INC
			sp = sp + ir.m;
			break;
		case 7: // JMP
			pc = ir.m;
			break;
		case 8: // JPC
			if (register_file[ir.r] == 0)
				pc = ir.m;
			break;
		case 9: // SIO
			if (ir.m == 1)
				printf("%d\n", register_file[ir.r]);
			
			else if (ir.m == 2)
				scanf("%d", &register_file[ir.r]);
			
			else 
				halt_flag = 1;
			
			break;	
		case 10: // NEG
			register_file[ir.r] = -register_file[ir.l];
			break;
		case 11: // ADD
			register_file[ir.r] = register_file[ir.l] + register_file[ir.m];
			break;
		case 12: // SUB
			register_file[ir.r] = register_file[ir.l] - register_file[ir.m];
			break;
		case 13: // MUL
			register_file[ir.r] = register_file[ir.l] * register_file[ir.m];
			break;
		case 14: // DIV
			register_file[ir.r] = register_file[ir.l] / register_file[ir.m];
			break;
		case 15: // ODD
			register_file[ir.r] = register_file[ir.r] % 2;
			break;
		case 16: // MOD
			register_file[ir.r] = register_file[ir.l] % register_file[ir.m];
			break;
		case 17: // EQL
			register_file[ir.r] = register_file[ir.l] == register_file[ir.m];
			break;
		case 18: // NEQ
			register_file[ir.r] = register_file[ir.l] != register_file[ir.m];
			break;
		case 19: // LSS
			register_file[ir.r] = register_file[ir.l] < register_file[ir.m];
			break;
		case 20: // LEQ
			register_file[ir.r] = register_file[ir.l] <= register_file[ir.m];
			break;
		case 21: // GTR
			register_file[ir.r] = register_file[ir.l] > register_file[ir.m];
			break;
		case 22: // GEQ
			register_file[ir.r] = register_file[ir.l] >= register_file[ir.m];
			break;
	}
}

void printStackRecursive(int sp, int bp, int* stack, int lex) // from the assignment pdf
{
	int i;
	
	if (bp == 1)
	{
		if (lex > 0)
			printf("|");
	}
	
	else 
	{
		printStackRecursive(bp-1, stack[bp+2], stack, lex-1);
		printf("|");
	}
	
	for (i = bp; i <= sp; i++)
		printf("%3d ", stack[i]);
}

void printStack()
{
	
	printf("%-4s%3d%3d%3d[%3d%3d%3d] ", ISA[ir.op], ir.r, ir.l, ir.m, pc, bp, sp);
	
	int i;
	
	printStackRecursive(sp, bp, stack, lex);
	
	printf("\n");
	
	printf("\tRegisters:[%3d%3d%3d%3d%3d%3d%3d%3d]\n", register_file[0], register_file[1], // register contents
	        register_file[2], register_file[3], register_file[4], register_file[5],
			register_file[6], register_file[7]);
			
	if (halt_flag)
		exit(0);
}

int main(int argc, char *argv[])
{
	int i;
	for (i = 0; i < 16; i++)
		register_file[i] = 0;
	
	for (i = 0; i < MAX_STACK_HEIGHT; i++)
		stack[i] = 0;
	
	FILE *input = fopen(argv[1], "r");
	if (input == NULL)
	{
		printf("Unable to open file\n");
		return 0;
	}
	
	printf("\n OP   Rg Lx Vl[ PC BP SP]\n");
	
	while(!feof(input))
	{
		fscanf(input, "%d%d%d%d", &instructions[lines].op, &instructions[lines].r, &instructions[lines].l, &instructions[lines].m);
		lines++;
	}
	
	for (i = 0; i < lines; i++)
	{
		fetchCycle();
		executeCycle();
		printStack();
	}
	
	return 0;
}