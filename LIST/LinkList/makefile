CC = gcc
CFLAGS = -o0 -g -std=gnu99 -Wall

SRC = $(wildcard *.c)
OBJS = Test
$(OBJS): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY:clean

clean:
	rm -rf $(OBJS) *.o

