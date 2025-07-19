CC = gcc
TARGET = mysh
SRC = main.c mysh.c
OBJ = $(SRC:.c=.o)
CFLAGS = -Wall -g

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) -lreadline
 
%.o: %.c mysh.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

