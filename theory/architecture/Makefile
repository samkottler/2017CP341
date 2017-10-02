traffic: traffic.o linkedList.o
	gcc -o traffic traffic.o linkedList.o

traffic.o: traffic.c traffic.h linkedList.h
	gcc -c traffic.c

linkedList.o: linkedList.c linkedList.h
	gcc -c linkedList.c

.PHONY: clean
clean:
	rm -f *.o
	rm -f traffic
	rm -f *~
