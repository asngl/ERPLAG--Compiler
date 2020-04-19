compiler:	driver2.c	grammar_Init.c	lexer.c	parser.c	stack.c	astGenerator.c	semanticAnalyzer.c	typeChecker.c	symbolTable.c 	codeGenerator.c
		gcc	-g -o	compiler	driver2.c	grammar_Init.c	lexer.c	parser.c	stack.c	astGenerator.c	semanticAnalyzer.c	symbolTable.c	typeChecker.c 	codeGenerator.c
stage1exe:	driver.c	grammar_Init.c	lexer.c	parser.c	stack.c	symbolTable.c
		gcc	-g -o	stage1exe	driver.c	grammar_Init.c	lexer.c	parser.c	stack.c	astGenerator.c	symbolTable.c	typeChecker.c