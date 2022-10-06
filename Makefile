hw1.exe: hw1.c
	gcc -o hw1.exe hw1.c

.PHONY: clean
clean :
	rm -rf hw1.exe