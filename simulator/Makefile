# Makefile for project 1

CC = cc
CFLAGS = -c -Wall
LDFLAGS = 
SOURCES = main.c reader.c HM.c data.c instrc.c register.c execute.c operator.c
OBJECTS = $(SOURCES:.c = .o)
EXECUTABLE = CMP

# Include .h file here
INCLUDES = data.h HM.h instrc.h register.h reader.h excute.h operator.h
all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
%.o: %.c ${INCLUDES}
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf *.o $(EXECUTABLE) *.bin *.rpt
