CC = gcc
CFLAGS = -std=c11 -Wall
TARGET = juego
OBJS = main.o tablero.o piezas.o armas.o

# Regla principal
all: $(TARGET)

# Linkeo final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilación separada de cada módulo
main.o: main.c main.h tablero.h armas.h piezas.h
	$(CC) $(CFLAGS) -c main.c

tablero.o: tablero.c tablero.h piezas.h main.h
	$(CC) $(CFLAGS) -c tablero.c

piezas.o: piezas.c piezas.h main.h
	$(CC) $(CFLAGS) -c piezas.c

armas.o: armas.c armas.h main.h
	$(CC) $(CFLAGS) -c armas.c

# Comando para limpiar la carpeta
clean:
	rm -f *.o $(TARGET)