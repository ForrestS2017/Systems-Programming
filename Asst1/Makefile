all: scannerCSVsorter

scannerCSVsorter: scannerCSVsorter.c scannerCSVsorter.h mergesort.c utilities.c
	gcc -fsanitize=address -g -o scannerCSVsorter scannerCSVsorter.c mergesort.c utilities.c

clean: 
	rm -rf scannerCSVsorter
