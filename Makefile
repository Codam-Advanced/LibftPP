NAME 	:= program
LIBRARY := libftpp.a

SRC_DIR := src
OBJ_DIR := obj



# Find all subdirectories
SRC_DIRS := $(shell find $(SRC_DIR) -type d)

# Find all cpp files in those directories
SRC := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

HEADERS := $(wildcard includes/*.hpp)

OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

INC := $(addprefix -I, $(shell find includes -type d))
FLAGS	:= -Wall -Wextra -std=c++20 -MMD -MP -fsanitize=address

CC	  := /usr/bin/g++-12 # Compiler to use

AR    := ar  # Archiver to create static libraries

all: $(NAME)

seggies: fclean re

leaks: all
	@echo checking for leaks
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(NAME)

run: all
	@echo "Running the library..."
	./$(NAME)

$(NAME): $(LIBRARY)
	  ${CC} main.cpp $(LIBRARY) $(INC) -o $@ -fsanitize=address

$(LIBRARY): $(OBJ)
	  ${AR} -cr $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS) | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	${CC} $(FLAGS) $(INC) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME) $(LIBRARY)

re: fclean all

.PHONY: all run clean fclean re leaks

