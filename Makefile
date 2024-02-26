OBJDIR = .
SRCDIR = .
INCLDIR = .
OUTDIR = .

INCLUDES = -I/usr/include/libftdi1/

## General Flags
PROGRAM = $(shell basename `pwd`)
CC = gcc
LD = gcc
CFLAGS = -Wall -Werror $(INCLUDES)
LDFLAGS = -lftdi1

## Objects that must be built in order to link
OBJECTS = $(OBJDIR)/ft_gpio.o \
          $(OBJDIR)/ft_spi.o \
          $(OBJDIR)/si446x.o \
          $(OBJDIR)/main.o


## Build both compiler and program
all: binary

## Compile source files
$(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c -o $(OBJDIR)/$*.o $< 

binary: $(OBJECTS)
	$(LD) -o $(OBJDIR)/$(PROGRAM) $(OBJECTS) $(LDFLAGS)

clean:
	-rm -rf $(OBJECTS) $(OBJDIR)/$(PROGRAM) 

