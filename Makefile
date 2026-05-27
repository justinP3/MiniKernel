
CC = gcc
CFLAGS = -Wall -Wextra -pthread -Iinclude

SRCS = src/main.c src/queue.c src/cpu.c src/metrics.c
OBJS = $(SRCS:.c=.o)

TARGET = minikernel

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(TARGET)