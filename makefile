CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = rbtree
OBJS = main.o rbtree.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c rbtree.h
	$(CC) $(CFLAGS) -c main.c

rbtree.o: rbtree.c rbtree.h
	$(CC) $(CFLAGS) -c rbtree.c

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean