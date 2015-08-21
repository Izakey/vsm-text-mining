#This is the Makefile for our Vector Space Model project

vsm: stemmer.c create_inverted_file_implementations.c main.c
	gcc -o vsm create_inverted_file_implementations.c stemmer.c main.c

#This is the end of our Makefile
