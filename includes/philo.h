/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/05 15:02:35 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

// #define USLEEP_TIME_MINI 1

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>

# ifndef THREADING_INIT_TIME_MICRO
#  define THREADING_INIT_TIME_MICRO 6000
# endif

# ifndef FAIL_INIT
#  define FAIL_INIT -1
# endif

# ifndef SHIFT_DIV_ESTIMATE
#  define SHIFT_DIV_ESTIMATE 10
# endif

# ifndef MICROSEC_TO_MILLISEC_FACTOR
#  define MICROSEC_TO_MILLISEC_FACTOR 0.001
# endif

# ifndef CACHE_LINE_SIZE
#  define CACHE_LINE_SIZE 64
# endif

# ifndef SLEEP_TOLERANCE
#  define SLEEP_TOLERANCE 300
# endif

# ifndef HARDCODE_SLEEP
#  define HARDCODE_SLEEP 200
# endif

// used is used as a bool, int64_t for preticable layout
typedef struct s_fork
{
	pthread_mutex_t	mutex_used;
	int8_t			used;
	int8_t			my_padding1;
	int8_t			my_padding2;
	int8_t			my_padding3;
	int8_t			my_padding4;
	int8_t			my_padding5;
	int8_t			my_padding6;
	int8_t			my_padding7;
	int8_t			my_padding8;
	int8_t			my_padding9;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_philo
{
	// cache line 1 (0 - 64)
	pthread_mutex_t		*status;
	int64_t				death_t;
	bool				*exit;
	int64_t				current_t;
	int64_t				fast_eat_sleep;
	int64_t				fast_sleep_sleep;
	int64_t				my_padding3;
	int64_t				my_padding4;
	// cache line 2 (64 - 128)
	t_fork				*left_fork;
	t_fork				main_fork;
	// cache line 3 (middle main_fork - 192)
	t_fork				*right_fork; // 176
	int64_t				starve_dur; // 184
	int64_t				eat_dur; // 192
	// cache line 4 (192 - 256)
	int64_t				next_eat_t;
	int64_t				eat_wait_dur;
	int64_t				eat_count;
	int64_t				sleep_dur;
	int64_t				start_t;
	int64_t				index;
	int64_t				my_padding5;
	int64_t				my_padding6;
}	t_philo;

// exit and status have to be the first two values to ensure cache locality
typedef struct s_general
{
	bool					exit;
	pthread_mutex_t			status;
	t_philo					*philos;
	pthread_t				*threads;
	uint16_t				count;
	int32_t					starve_dur;
	int32_t					eat_dur;
	int32_t					sleep_dur;
	int32_t					eat_count;
	int64_t					start_t;
	t_philo					*ptr_to_free_philos;
}	t_general;

// main.c
void		*main_loop(void *arg);
void		wait_threads(t_general *const general);
//int		main(int ac, char *av[]);

// init.c
int			input(int ac, char *av[], t_general *const gen);
int			fill_philo(t_general *const general, int64_t i);
bool		intit_threading(t_general *const general);
int			init_philos(t_general *const general);

// utils
int64_t		ft_atoi(const char *str);
void		align_ptr(int8_t **ptr);

bool		pickup_left_fork(t_philo *restrict const philo);
bool		pickup_right_fork(t_philo *restrict const philo);
bool		drop_forks(t_philo *restrict const philo);
bool		check_exit(t_philo *restrict const philo);
bool		eat(t_philo *restrict const philo);
bool		change_status(t_philo *restrict const philo, char *const status);
void		drop_left_fork(t_philo *restrict const philo);
void		drop_right_fork(t_philo *restrict const philo);
bool		do_exit(t_philo *restrict const philo, const bool locked_mutex);

// time
bool		my_sleep_until(const int64_t target_t,
				t_philo *restrict const philo);
void		my_sleep_no_exit(const int64_t target_t);
int64_t		get_microseconds(void);

int			cleanup(t_general *const general);

// debug
void		print_philo_info(t_philo *restrict const philo,
				bool timings, bool check_mutexes);

#endif
