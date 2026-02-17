CCPP = c++
CC = cc
FLAGS = -Wall -Wextra -Werror -MMD -g -fsanitize=address

SRC_DIR = .
OBJ_DIR = .obj

INC = includes
INCGLFW = ../deps/include/GLFW/

SRC_CPP = $(shell find $(SRC_DIR) -name "*.cpp")
SRC_C = $(shell find $(SRC_DIR) -name "*.c")
OBJ_CPP = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_CPP))
OBJ_C = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_C))
DEP = $(OBJ:.o=.d)

NAME = scop
LIBGLFW = ../deps/lib/libglfw3.a


$(NAME): all

all: $(OBJ_DIR) $(OBJ_CPP) $(OBJ_C)
	@$(CCPP) $(FLAGS) -I $(INC) -I $(INCGLFW) $(OBJ_C) $(OBJ_CPP) $(LIBGLFW) -o $(NAME)
	@echo "$(NAME) compiled"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CCPP) $(FLAGS) -I $(INC) -I $(INCGLFW) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(FLAGS) -I $(INC) -I $(INCGLFW) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

re:	fclean all

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Clear objects files"

fclean: clean
	@rm -rf $(NAME)
	@echo "Clear binary file"

test: all
	./$(NAME)

.PHONY: all re clean fclean

-include $(DEP)