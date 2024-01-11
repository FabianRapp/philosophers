CC=cc
CFLAGS=-Wall -Wextra
FLAGS_SPEED=-Werror -O3 -march=native -ftree-vectorize -funroll-loops
PROFILE_FLAGS=
INCLUDES=-I./includes
SOURCES= main.c utils/utils1.c init.c utils/utils2.c forks.c utils/sync_utils.c utils/time.c threading.c \
 calculate_sleep_time.c debug.c my_print.c
OBJECTS= $(SOURCES:.c=.o)
NAME=p
NAME_PROFILE=$(NAME)_profile
tool=drd

.PHONY: all clean fclean re clean2

all:$(NAME)

$(NAME): build clean2

build: $(OBJECTS)
	@$(CC) $(INCLUDES) $^ -o $(NAME) $(CFLAGS) $(FLAGS_SPEED)
	@echo "\033[32mBuild complete.\033[0m"

nWWW:
	@$(CC) $(INCLUDES) $(SOURCES) $^ -o $(NAME) $(FLAGS_SPEED)
	@echo "\033[32mBuild complete.\033[0m"


fsanitize_address: fclean
	@$(CC) $(INCLUDES) $(CFLAGS) $(FLAGS_SPEED) -g $(SOURCES) -o $(NAME) -fsanitize=address
	@echo "\033[32mBuild complete.\033[0m"

fsanitize_thread: fclean
	@$(CC) $(INCLUDES) $(CFLAGS) $(FLAGS_SPEED) -g $(SOURCES) -o $(NAME) -fsanitize=thread
	@echo "\033[32mBuild complete.\033[0m"

prof_mac:
	clang -fprofile-instr-generate -fcoverage-mapping $(INCLUDES) $(SOURCES) -o $(NAME)_profile


profile: PROFILE_FLAGS=-pg -g
profile: fclean
	$(CC) $(INCLUDES) $(CFLAGS) $(FLAGS_SPEED) $(SOURCES) -o $(NAME_PROFILE) $(PROFILE_FLAGS)
	@echo "\033[32mBuild complete.\033[0m"

perf: fclean
	perf record @$(CC) $(CFLAGS) $(FLAGS_SPEED) $(INCLUDES) $(SOURCES) -o $(NAME)_perf

%.o: %.c
	@$(CC) $(CFLAGS) $(FLAGS_SPEED) $(PROFILE_FLAGS) $(INCLUDES) -o $@ -c $^

clean:
	@rm -f $(OBJECTS)
	@echo "\033[33mObject files removed.\033[0m"

fclean: clean
	@rm -f $(NAME) $(NAME_PROFILE) gmon.out analysis.txt perf.data a.out
	@echo "\033[33mExecutable removed.\033[0m"

re: fclean all

clean2:
	@rm -f $(OBJECTS)
	@echo "\033[33mObject files removed.\033[0m"

norm:
	norminette $(SOURCES) includes/philo.h

