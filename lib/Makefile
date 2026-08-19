# ==== Configuración ====
CC       = gcc
CFLAGS   = -std=c17 -Wall -Wextra -fPIC -I$(INCLUDE_DIR)
LDFLAGS  = -shared

INCLUDE_DIR = include
SRC_DIR     = src
BUILD_DIR   = build
LIB_DIR     = lib

TARGET = $(LIB_DIR)/libmaze.so

# Todos los .c en src/, y su correspondiente .o en build/
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))

# ==== Reglas ====
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS) | $(LIB_DIR)
	$(CC) $(LDFLAGS) -o $@ $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)/libmaze.so