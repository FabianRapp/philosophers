CC=cc
CFLAGS=
SOURCES= main.c utils/atoi.c utils/exit.c utils/mutex_utils.c utils/utils1.c
OBJECTS= $(SOURCES:.c=.o)
NAME=p

.PHONY: all clean fclean re clean2

all:$(NAME)

$(NAME): build clean2
	@echo "output:"
#@./$(NAME) 3 2 3 4

build: $(OBJECTS)
	@$(CC) $^ -o $(NAME)
	@echo "\033[32mBuild complete.\033[0m"

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $^

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