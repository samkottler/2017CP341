CFLAGS = -std=c11 -Wall
CLIBS = -lm -lpng
OBJS = ./obj/pngUtil.o ./obj/serial.o


mandelbrot_serial: ./obj $(OBJS)
	gcc -o $@ $(OBJS) $(CFLAGS) $(CLIBS)

./obj:
	mkdir ./obj

./obj/%.o: ./src/%.c $(wildcard, ./src/%.h)
	gcc -o $@ -c $^ $(CFLAGS)

.PHONY:clean
clean:
	rm -f *.png
	rm -f -r ./obj
	rm -f mandelbrot_*
