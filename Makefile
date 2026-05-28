
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

test:
	gcc -Wall -Wextra -pthread -Iinclude src/queue.c tests/pruebas_unitarias.c -o tests/ejec_test
	./tests/ejec_test