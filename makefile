stage1exe:	driver.c	grammar_Init.c	lexer.c	parser.c	stack.c	symbolTable.c
		gcc	-o	stage1exe	driver.c	grammar_Init.c	lexer.c	parser.c	stack.c	astGenerator.c	symbolTable.c