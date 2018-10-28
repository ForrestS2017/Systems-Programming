all: scannerCSVsorter

scannerCSVsorter: scannerCSVsorter.c scannerCSVsorter.h mergesort.c utilities.c sorter.c
	gcc -fsanitize=address -g -o scannerCSVsorter scannerCSVsorter.c mergesort.c utilities.c sorter.c

clean: 
	rm -rf scannerCSVsorter
