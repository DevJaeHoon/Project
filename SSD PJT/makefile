ssd : ssd.o shell.o shell
		gcc ssd.o -o ssd

ssd.o : 
	gcc -c ssd.c

shell.o :
	gcc -c shell.c

shell :
	gcc shell.o -o shell

clean1 : ssd.o shell.o ssd shell
	rm -r $^

clean2 : nand.txt result.txt
	rm -r $^
