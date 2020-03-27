stage1exe:	driver.c	grammar_Init.c	lexer.c	parser.c	stack.c
		gcc	-o	stage1exe	driver.c	grammar_Init.c	lexer.c	parser.c	stack.c	astGenerator.c