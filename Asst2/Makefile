all: multiThreadSorter

multiThreadSorter: multiThreadSorter.c multiThreadSorter.h mergesort.c utilities.c sorter.c
	gcc -g -lpthread -fsanitize=address -g -o multiThreadSorter multiThreadSorter.c mergesort.c utilities.c sorter.c

clean: 
	rm -rf multiThreadSorter
