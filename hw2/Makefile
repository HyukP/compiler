hw2.exe:	hw2.l
	flex hw2.l
	gcc -o parser_201620908.out lex.yy.c -lfl

.PHONY: clean
clean :
	rm -rf hw2.exe lex.yy.c