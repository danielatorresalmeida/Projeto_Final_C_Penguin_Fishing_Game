# Makefile para o Penguin Fishing Game
# Usar no terminal MSYS2 UCRT64 dentro da pasta do projeto.

CC = gcc
TARGET = jogo.exe

SRC = main.c game.c menu.c logic.c render.c
OBJ = $(SRC:.c=.o)

CFLAGS = -Wall -Wextra -std=c11 -I/ucrt64/include
LDFLAGS = -L/ucrt64/lib -lncursesw

.PHONY: all run clean rebuild format kill debug

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -f $(OBJ) jogo jogo.exe jogo2 jogo2.exe

rebuild: clean all

format:
	clang-format -i main.c game.c game.h menu.c menu.h logic.c logic.h render.c render.h

kill:
	cmd.exe /c "taskkill /IM jogo.exe /F >NUL 2>NUL" || true
	rm -f jogo jogo.exe

debug: CFLAGS += -g -O0
debug: clean all
