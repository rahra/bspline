CC = gcc
#CFLAGS = -std=gnu99 -Wall $(CAIRO_CFLAGS)
CFLAGS = -g -std=gnu99 -Wall -DCONSTRUCTION $(CAIRO_CFLAGS)
LDLIBS = -lm $(CAIRO_LIBS)
CAIRO_CFLAGS = $(shell pkg-config --cflags cairo)
CAIRO_LIBS = $(shell pkg-config --libs cairo)

all: bspline

bspline.o: bspline.c bspline.h

bspline_constr.o: bspline_constr.c bspline.h

bspline_ctrl.o: bspline_ctrl.c bspline.h

bspline: bspline.o bspline_ctrl.o bspline_constr.o

clean:
	rm -f *.o bspline

.PHONY: clean

