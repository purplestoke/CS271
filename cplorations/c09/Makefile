CC = gcc
CFLAGS = -Wall -I.
LDFLAGS =
OBJFILES = main.o parser.o symtable.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

main.o: main.c parser.h symtable.h
	$(CC) $(CFLAGS) -c main.c

parser.o: parser.c parser.h symtable.h
	$(CC) $(CFLAGS) -c parser.c

symtable.o: symtable.c symtable.h
	$(CC) $(CFLAGS) -c symtable.c

clean:
	rm -f $(OBJFILES) $(TARGET) *~
