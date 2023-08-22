CFLAGS=-std=c17 -Wall -Wpedantic -Wextra -fsanitize=address
SOURCES=$(wildcard *.c)
OBJECTS=$(SOURCES:.c=.o)
TARGET=test

%.o:%.c
	$(CC) $(CFLAGS) -c $^

$(TARGET):$(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY : clean all

all:$(TARGET)
	@echo $(SOURCES)
	@echo $(OBJECTS)
	@echo $(TARGET)

clean:
	rm $(OBJECTS) $(TARGET)
