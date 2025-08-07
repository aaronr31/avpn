CC = gcc

CFLAGS = -Wall -g

EXECUTABLE = avpn
OUTPUT_DIR = bin

all: $(EXECUTABLE)

# Final binary executable
$(EXECUTABLE):
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(CFLAGS) src/main.c -o $(OUTPUT_DIR)/$(EXECUTABLE)

# Delete built files from project directory
clean:
	rm -rf bin/

# Auto-formatter
format:
	clang-format -i src/* --style=Microsoft
