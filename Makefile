MODULE_NAME = php_shm

LIBPHPHI_DIR := libphphi

# Include the common build rules
-include $(LIBPHPHI_DIR)/build/common.mk

.PHONY: all fetch_lib  build

all: fetch_lib build_solo


PHP_CONFIG ?= php-config

PHP_INCLUDE_DIRS := $(shell $(PHP_CONFIG) --includes)


# Your local build steps
build:
	$(MAKE) -C $(LIBPHPHI_DIR) build_lib
	$(MAKE)




# all: build
# 	@git submodule init
# 	@git submodule update --remote
# 	@echo "Submodules updated."


# include $(LIBPHPHI_DIR)/build/common.mk

## $(shell git rev-parse --show-toplevel)/


## all: build




# # Module-specific names
# MODULE_NAME = mqueue  # Change this to shm_table for other modules

# # Directories
# EXT_DIR = ext
# SRC_DIR = src
# LIBPHPI_DIR = libphphi
# LIB_DIR = lib

# # Source and object files for the module
# SRCS = $(wildcard $(EXT_DIR)/*.c)
# OBJS = $(patsubst $(EXT_DIR)/%.c,$(LIB_DIR)/%.o,$(SRCS))

# # Source and object files for libphphi (the submodule)
# LIBPHPI_SRCS = $(wildcard $(LIBPHPI_DIR)/ext/*.c)
# LIBPHPI_OBJS = $(patsubst $(LIBPHPI_DIR)/ext/%.c,$(LIB_DIR)/%.o,$(LIBPHPI_SRCS))

# # Compiler and flags
# CC = gcc
# AR = ar
# CFLAGS = -Wall -Wextra -fPIC
# LDFLAGS = -shared -ldl -lc

# # Default target
# all: $(MODULE_NAME)

# # Compile the module and statically link with libphphi
# $(MODULE_NAME): $(OBJS) $(LIBPHPI_OBJS)
# 	$(CC) $(LDFLAGS) -o $(LIB_DIR)/$(MODULE_NAME).so $(OBJS) $(LIBPHPI_OBJS)

# # Rule to build object files for the module
# $(LIB_DIR)/%.o: $(EXT_DIR)/%.c
# 	@mkdir -p $(LIB_DIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Rule to build object files for libphphi (static linking)
# $(LIB_DIR)/%.o: $(LIBPHPI_DIR)/ext/%.c
# 	@mkdir -p $(LIB_DIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# # Clean target
# clean:
# 	rm -rf $(LIB_DIR)/*.o $(LIB_DIR)/*.so




# CC = gcc
# CFLAGS = -Wall -Wextra -O2 -fPIC
# LDFLAGS = -shared
# TARGET = shm_table.so
# EXTDIR = ext
# LIBDIR = $(EXTDIR)/lib
# SOURCES = $(wildcard $(EXTDIR)/*.c)
# OBJECTS = $(LIBDIR)/$(notdir $(SOURCES:.c=.o))
# HEADERS = $(wildcard $(EXTDIR)/*.h)

# .PHONY: all clean install

# all: $(LIBDIR)/$(TARGET)

# $(LIBDIR)/$(TARGET): $(OBJECTS)
# 	@mkdir -p $(LIBDIR)
# 	$(CC) $(LDFLAGS) -o $@ $^

# $(LIBDIR)/%.o: $(EXTDIR)/%.c $(HEADERS)
# 	@mkdir -p $(LIBDIR)
# 	$(CC) $(CFLAGS) -c $< -o $@

# clean:
# 	rm -f $(OBJECTS) $(LIBDIR)/$(TARGET)

# install: all
