BINARY = dinobash_shell
TARGET = $(BINARY)
CC = gcc
CFLAGS = -Wall -g

SRC = dinobash.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

install: $(TARGET)
	sudo cp $(TARGET) /bin/

uninstall:
	sudo rm -f /bin/$(TARGET)

clean:
	rm -f $(TARGET)