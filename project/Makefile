CFLAGS = -std=c11 -Wall
CLIBS = -lm -lpng
OBJS = ./obj/pngUtil.o ./obj/serial.o


./bin/mandelbrot_serial: ./bin ./obj $(OBJS)
	gcc -o $@ $(OBJS) $(CFLAGS) $(CLIBS)

./%:
	mkdir $@

./obj/%.o: ./src/%.c $(wildcard, ./src/%.h)
	gcc -o $@ -c $^ $(CFLAGS)

.PHONY:clean
clean:
	rm -f *.png
	rm -f -r ./obj
	rm -f -r ./bin
