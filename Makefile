
MODULE_NAME = php_mqueue

LIBPHPHI_DIR = libphphi

PHP_CONFIG ?= php-config
PHP_INCLUDE_DIRS := $(shell $(PHP_CONFIG) --includes)

.PHONY: all build pre-update check-submodule update-submodules

ifeq ($(MAKECMDGOALS),)
    MAKECMDGOALS := all
endif

all: build

pre-update:
	@echo "Updating submodules..."
	@git submodule update --remote --recursive
	@echo "Submodules updated."

check-submodule:
	@if [ ! -d "$(LIBPHPHI_DIR)" ]; then \
		echo "Submodule directory $(LIBPHPHI_DIR) not found. Initializing..."; \
		git submodule init; \
		git submodule update --init --recursive; \
	fi

update-submodules: check-submodule pre-update

build: update-submodules
	@echo "Building libphphi..."
	@$(MAKE) -C $(LIBPHPHI_DIR) build_lib || exit 1
	@echo "Building $(MODULE_NAME)..."
	@$(MAKE) -f Makefile.builder

# Include common rules after all targets are defined
include $(LIBPHPHI_DIR)/build/common.mk

# Ensure all goals go through pre-update
$(filter-out pre-update update-submodules check-submodule, $(MAKECMDGOALS)): pre-update

