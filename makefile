stage2exe:	driver.c	grammar_Init.c	lexer.c	parser.c	stack.c	symbolTable.c
		gcc	-g -o	stage2exe	driver2.c	grammar_Init.c	lexer.c	parser.c	stack.c	astGenerator.c	symbolTable.c	typeChecker.c
stage1exe:	driver.c	grammar_Init.c	lexer.c	parser.c	stack.c	symbolTable.c
		gcc	-g -o	stage1exe	driver.c	grammar_Init.c	lexer.c	parser.c	stack.c	astGenerator.c	symbolTable.c	typeChecker.c