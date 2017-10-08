CFLAGS = -std=gnu11 -Wall -fcilkplus -fopenmp
CLIBS = -lm -lpng
BIN = ./bin
OBJ = ./obj
objects = pngUtil.o mandelbrot.o
OBJS = $(objects:%.o=$(OBJ)/%.o)
binaries = serial cilk openmp nonrecursive
BINS = $(binaries:%=$(BIN)/mandelbrot_%)

ifeq (1, $(CHECK_ARR))
CFLAGS := $(CFLAGS) -D CHECK_ARR
endif

ifeq (1, $(DEBUG))
CFLAGS := $(CFLAGS) -g
endif

.PHONY: all
all: $(BINS)

$(BIN)/mandelbrot_%: $(OBJ)/%.o $(OBJS)
	@echo Linking $@
	@mkdir -p $(BIN)
	@gcc -o $@ $< $(OBJS) $(CFLAGS) $(CLIBS)

.PRECIOUS: $(OBJ)/%.o
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
