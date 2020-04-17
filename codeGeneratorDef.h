#ifndef _CODEGENERATORDEFH
#define _CODEGENERATORDEFH
#include "symbolTable.h"
#include "symbolTableDef.h"
#include "typeChecker.h"
#include "ASTNodeDef.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
enum Instruction{PRINT,INPUT,MOV,CMP,ADD,SUB,MUL,DIV,LABEL};
enum valueType{MEM,REG,IMM_NUM,IMM_RNUM};
enum Register{AX,BX,CX,DX,ESP,EBP};

union untaggedValue{
	int num;
	float rnum;
	enum Register reg;
	struct Memory{
		enum Register reg;
		struct Offset{
			int tag;// 0 for no register and only offset , 1 for register along with offset
			enum Register;
			int offset;
		}index;
	}mem;
};

typedef struct Value{
	enum valueType tag;
	union untaggedValue val;
}Value;

typedef struct Line{
	enum Instruction instr;
	char label[25];
	
	Value res; 	// Result
	Value op1;	//Operand 1
	Value op2;	//Operand 2
	Line *nextLine;
}

typedef struct Code{
	Line *firstLine;
	Line *lastLine;
}Code;
#endif
