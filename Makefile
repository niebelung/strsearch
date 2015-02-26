CC=gcc
CFLAGS=-c -g -Wall -std=c99
LDFLAGS=
SOURCES=main.c strsearch.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=strsearch

all: $(EXECUTABLE)

rebuild: clean $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@
clean:
	rm -fr *.o ${EXECUTABLE}