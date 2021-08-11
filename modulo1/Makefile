NOMBRE_EJECUTABLE=modulo1
CC = gcc
CFLAGS = -g -Wall
LIBS = -lcommons -lpthread -lreadline

BIN = bin
OBJ = obj
SRC = src

TARGET = $(BIN)/$(NOMBRE_EJECUTABLE)
SRCS = $(wildcard $(SRC)/*.c)
HEADERS = $(wildcard $(SRC)/*.h)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))


SHARED_SRC = ../shared/src
SHARED_OBJ = ../shared/obj

SHARED_SRCS = $(wildcard $(SHARED_SRC)/*.c)
SHARED_HEADERS = $(wildcard $(SHARED_SRC)/*.h)
SHARED_OBJS = $(patsubst $(SHARED_SRC)/%.c,$(SHARED_OBJ)/%.o,$(SHARED_SRCS))

.PHONY: default all clean

all: $(TARGET)

release: CFLAGS = -Wall -O2 -DNDEBUG
release: clean
release: $(TARGET)

wclang: CC = clang
wclang: TARGET = $(BIN)/i-mongo-wclang
wclang: clean
wclang: $(TARGET)


$(TARGET): $(OBJS) $(SHARED_OBJS)
	$(CC) $^ $(CFLAGS) -I$(SHARED_SRC) $(LIBS) -o $@

$(OBJ)/%.o: $(SRC)/%.c $(HEADERS) $(SHARED_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

$(SHARED_ODIR)/%.o: $(SHARED_SRC)/%.c $(SHARED_HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(BIN)/* $(OBJ)/*