all: dirinfo.o
	gcc dirinfo.o

clean: 
	rm -f *~ *.gch* a.out *.o *#*

run: 
	./a.out

dirinfo.o: dirinfo.c
	gcc -c dirinfo.c
