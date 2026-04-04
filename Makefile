# Compiler
CCPP = c++
CC = cc

# Flags
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -Wall -Wextra -Werror -MMD -g #-fsanitize=address

# Project Paths
SRC_DIR = srcs
OBJ_DIR = .obj
INC_DIR = includes

# Vendor Paths
GLAD_DIR = vendor/glad
GLFW_DIR = vendor/glfw
GLFW_BUILD_DIR = $(GLFW_DIR)/build
GLFW_LIB = $(GLFW_BUILD_DIR)/src/libglfw3.a

LIBS = $(GLFW_LIB) -lGL -ldl -lpthread -lX11 -lXrandr -lXinerama -lXcursor -lXi

# Include Paths
INCLUDES = -I $(INC_DIR) \
			-I $(GLAD_DIR)/include \
			-I $(GLFW_DIR)/include

SRC_CPP = $(shell find $(SRC_DIR) -name "*.cpp")
GLAD_C  = $(GLAD_DIR)/src/glad.c
OBJ_CPP = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_CPP))
OBJ_GLAD = $(OBJ_DIR)/glad.o
ALL_OBJS = $(OBJ_CPP) $(OBJ_GLAD)
DEP = $(ALL_OBJS:.o=.d)

NAME = scop

all: $(NAME)

$(NAME): $(OBJ_DIR) $(OBJ_CPP) $(OBJ_GLAD) $(GLFW_LIB)
	@$(CCPP) $(CPPFLAGS) $(INCLUDES) $(OBJ_GLAD) $(OBJ_CPP) $(LIBS) -o $(NAME)
	@echo "$(NAME) compiled"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CCPP) $(CPPFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_GLAD): $(GLAD_C)
	@mkdir -p $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

re:	fclean all

clean:
	@rm -rf $(OBJ_DIR)
	@echo "Clear objects files"

fclean: clean
	@rm -rf $(NAME)
	@echo "Clear binary file"

$(GLFW_LIB):
	@echo "Building GLFW"
	@mkdir -p $(GLFW_BUILD_DIR)
	@cd $(GLFW_BUILD_DIR) && cmake .. -DGLFW_BUILD_DOCS=OFF -DGLFW_BUILD_TESTS=OFF -DGLFW_BUILD_EXAMPLES=OFF
	@cd $(GLFW_BUILD_DIR) && make -j$(shell nproc)

test: all
	./$(NAME) ./assets/resources/teapot.obj

.PHONY: all re clean fclean

-include $(DEP)