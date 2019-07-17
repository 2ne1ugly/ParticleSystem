CXXC=clang++
CXXFLAGS=-Werror -Wextra -Wall -g -Iinclude -Ilib/glew-2.1.0/include -Ilib/glfw-3.3/include -Ilib/glm-0.9.9.5 -std=c++14
LIB=-Llib/glew-2.1.0/lib -Llib/glfw-3.3/src -lglfw3 ./lib/glew-2.1.0/lib/libGLEW.a -Llib/glm-0.9.9.5/glm -lglm_static
FRAMEWORK=-framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo -framework OpenCL
SRC=$(shell find sources -type f -name '*.cpp')
NAME=ps
OBJ=$(SRC:sources/%.cpp=objects/%.o)

$(NAME): $(OBJ)
	$(CXXC) $(CXXFLAGS) $(LIB) $(FRAMEWORK) -o $(NAME) $(OBJ)
	
	@echo "$(NAME) build complete!"

all: $(NAME)

$(OBJ): objects/%.o : sources/%.cpp
	@mkdir -p $(shell dirname $@) 
	$(CXXC) $(CXXFLAGS) -o $@ -c $^
clean:
	@rm -rf objects/

fclean: clean
	@rm -f $(NAME)

re: fclean all
