CFLAGS = -std=gnu11 -Wall
CLIBS = -lm -lpng -lrt
BIN = ./bin
OBJ = ./obj
objects = pngUtil.o serial.o
OBJS = $(objects:%.o=$(OBJ)/%.o)

$(BIN)/mandelbrot_serial: $(OBJS)
	@echo Linking $@
	@mkdir -p $(BIN)
	@gcc -o $@ $(OBJS) $(CFLAGS) $(CLIBS)

$(OBJ)/%.o: ./src/%.c
	@echo Compiling $<
	@mkdir -p $(OBJ)
	@gcc -MMD -c -o $@ $< $(CFLAGS)

.PHONY:clean
clean:
	@rm -f *.png
	@rm -f -r $(OBJ)
	@rm -f -r $(BIN)

-include ./obj/*.d
