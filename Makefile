CC = gcc
CFLAGS = -Wall -Wextra -O2 -fPIC
LDFLAGS = -shared
TARGET = shm_table.so
EXTDIR = ext
LIBDIR = $(EXTDIR)/lib
SOURCES = $(wildcard $(EXTDIR)/*.c)
OBJECTS = $(LIBDIR)/$(notdir $(SOURCES:.c=.o))
HEADERS = $(wildcard $(EXTDIR)/*.h)

.PHONY: all clean install

all: $(LIBDIR)/$(TARGET)

$(LIBDIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(LIBDIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(LIBDIR)/%.o: $(EXTDIR)/%.c $(HEADERS)
	@mkdir -p $(LIBDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(LIBDIR)/$(TARGET)

install: all
