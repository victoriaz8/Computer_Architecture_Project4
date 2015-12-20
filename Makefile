
objects = main ldata.o llist.o main.o hashtable.o

proj4: ldata.o llist.o main.o hashtable.o
	cc -std=c99 -g -o $(objects) -lm


main.o: main.c
	cc -c -g main.c
	
ldata.o: ldata.c ldata.h
	cc -c -g ldata.c
llist.o: llist.c llist.h
	cc -c -g llist.c
	
hashtable.o: hashtable.c hashtable.h
	cc -c -g hashtable.c
	
clean: 
	rm -rf $(objects)