CC=gcc
CFLAGS=-Wall -g
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
EXEC=dungeon-adventure-game.exe

# Varsayılan hedef: derleme ve çalıştırma
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC)

# .c dosyasından .o dosyasına derleme
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Temizlik (geçici dosyaları silmek için)
clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
