default: rdv.o rdv clean

rdv.o : rdv.c
	gcc -c rdv.c

rdv : rdv.c obj/calculC.o
	gcc obj/calculC.o rdv.o -o rdv

clean:
	rm -f rdv.o
	chmod +x rdv
