CC=cc
CFLAGS=-Wall -Wextra -Werror -O3 -march=native -ftree-vectorize -pthread -funroll-loops
INCLUDES=-I./includes
DEBUG= debug.c
SOURCES= $(DEBUG) main.c utils/utils1.c init.c utils/utils2.c forks.c
OBJECTS= $(SOURCES:.c=.o)
NAME=p
tool=drd

.PHONY: all clean fclean re clean2

all:$(NAME)

$(NAME): build clean2

build: $(OBJECTS)
	@$(CC) $(INCLUDES) $^ -o $(NAME) 
	@echo "\033[32mBuild complete.\033[0m"

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $^

clean:
	@rm -f $(OBJECTS)
	@echo "\033[33mObject files removed.\033[0m"

fclean: clean
	@rm -f $(NAME)
	@echo "\033[33mExecutable removed.\033[0m"

re: fclean all

clean2:
	@rm -f $(OBJECTS)
	@echo "\033[33mObject files removed.\033[0m"

run_valgrind:
	valgrind --tool=$(tool) ./$(NAME)