CC=cc
CFLAGS=
INCLUDES=-I./includes
SOURCES= main.c utils/utils1.c utils/edgecases.c init.c
OBJECTS= $(SOURCES:.c=.o)
NAME=p

.PHONY: all clean fclean re clean2

all:$(NAME)

$(NAME): build clean2

build: $(OBJECTS)
	@$(CC) $^ -o $(NAME) $(INCLUDES)
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