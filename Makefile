# Project Settings
debug ?= 0
NAME := dman 
SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include
LIB_DIR := lib
TESTS_DIR := tests
BIN_DIR := bin

# Generate paths for all object files
OBJS := $(patsubst %.c,%.o, $(wildcard $(SRC_DIR)/*.c) $(wildcard $(LIB_DIR)/**/*.c))

# Compiler settings
CC := clang
LINTER := clang-tidy
FORMATTER := clang-format

# Compiler and Linker flags Settings:
# 	-std=gnu17: Use the GNU17 standard
# 	-D _GNU_SOURCE: Use GNU extensions
# 	-D __STDC_WANT_LIB_EXT1__: Use C11 extensions
# 	-Wall: Enable all warnings
# 	-Wextra: Enable extra warnings
# 	-pedantic: Enable pedantic warnings
# 	-lm: Link to libm
CFLAGS := -std=gnu17 -D _GNU_SOURCE -D __STDC_WANT_LIB_EXT1__ -Wall -Wextra -pedantic
LDFLAGS := -lm -lncurses -lcjson -lcurl

ifeq ($(debug), 1)
	CFLAGS := $(CFLAGS) -g -O0
else
	CFLAGS := $(CFLAGS) -Oz
endif

# Targets

# Build executable
$(NAME): format lint dir $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(BIN_DIR)/$@ $(patsubst %, build/%, $(OBJS))

# Build object files and third-party libraries
$(OBJS): dir
	@mkdir -p $(BUILD_DIR)/$(@D)
	@$(CC) $(CFLAGS) -o $(BUILD_DIR)/$@ -c $*.c

# Run CUnit tests
test: dir
	@$(CC) $(CFLAGS) -lcunit -o $(BIN_DIR)/$(NAME)_test $(TESTS_DIR)/*.c
	@$(BIN_DIR)/$(NAME)_test

# Run linter on source directories
lint:
	@$(LINTER) --config-file=.clang-tidy $(SRC_DIR)/* $(INCLUDE_DIR)/* $(TESTS_DIR)/* -- $(CFLAGS)

# Run formatter on source directories
format:
	@$(FORMATTER) -style=file -i $(SRC_DIR)/* $(INCLUDE_DIR)/* $(TESTS_DIR)/*

# Run valgrind memory checker on executable
check: $(NAME)
	@sudo valgrind -s --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$< --help
	@sudo valgrind -s --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$< --version
	@sudo valgrind -s --leak-check=full --show-leak-kinds=all $(BIN_DIR)/$< -v

# Setup dependencies for build and development
setup:
	# Update apt and upgrade packages
	@sudo dnf update
	@sudo dnf upgrade -y

	# Install Clang development tools
	@sudo dnf install -y clang

	# Install CUnit testing framework
	@sudo dnf install -y cunit

	# Cleanup
	@sudo dnf autoremove -y

# Setup build and bin directories
dir:
	@mkdir -p $(BUILD_DIR) $(BIN_DIR)

# Clean build and bin directories
clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)

# Run bear to generate compile_commands.json
bear:
	bear --exclude $(LIB_DIR) make $(NAME)

.PHONY: lint format check setup dir clean bear
