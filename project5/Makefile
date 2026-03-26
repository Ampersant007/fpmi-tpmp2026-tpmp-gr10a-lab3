CC = gcc
CFLAGS = -Iinclude
LIBS = -lsqlite3
TARGET = bin/company_manager

$(shell mkdir -p obj bin data)

SRC = src/main.c src/database.c
OBJ = obj/main.o obj/database.o

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

obj/main.o: src/main.c include/database.h
	$(CC) $(CFLAGS) -c src/main.c -o obj/main.o

obj/database.o: src/database.c include/database.h
	$(CC) $(CFLAGS) -c src/database.c -o obj/database.o

clean:
	rm -f obj/*.o bin/*

run: $(TARGET)
	./$(TARGET)
