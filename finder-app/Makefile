ifeq ($(CROSS_COMPILE), aarch64-none-elf-)
	CC = aarch64-none-elf-gcc
else
	CC = gcc
endif

CFLAGS = -Wall -g
EXECUTABLE = writer

# Rule to link object file(s) and create the executable
$(EXECUTABLE) : writer.o
	$(CC) $(CFLAGS) -o $(EXECUTABLE) writer.o

# Rule to compile source file(s) into object file(s)
writer.o : writer.c
	$(CC) $(CFLAGS) -c writer.c -o writer.o

# Clean up build files
.PHONY : clean
clean :
	rm -f writer.o writer