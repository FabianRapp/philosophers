/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/08 18:39:46 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdint.h>

# ifndef MICROSEC_PER_THREAD_INIT_TIME
#  define MICROSEC_PER_THREAD_INIT_TIME 2000
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

# ifndef SEC_TO_MICROSEC_FACTOR
#  define SEC_TO_MICROSEC_FACTOR 1000000
# endif

# ifndef CACHE_LINE_SIZE
#  define CACHE_LINE_SIZE 64
# endif

# ifndef THINKING_MSG
#  define THINKING_MSG "is thinking"
# endif

# ifndef PICK_UP_FORK_MSG
#  define PICK_UP_FORK_MSG "has taken a fork"
# endif

# ifndef EATING_MSG
#  define EATING_MSG "is eating"
# endif

# ifndef SLEEP_MSG
#  define SLEEP_MSG "is sleeping"
# endif

# ifndef SLEEP_TOLERANCE_SPEED
#  define SLEEP_TOLERANCE_SPEED 60
# endif

# ifndef SLEEP_TOLERANCE_ACC
#  define SLEEP_TOLERANCE_ACC 30
# endif

# ifndef SPEED_BOUNDRY
#  define SPEED_BOUNDRY 0
# endif

/*
	the x-value is the philo count (== thread count)
	the y-value thread execution time (in microseconds)

	these values are used to form a quadratic function,
	which is used to estimate the thread needs for all
	computations within one loop iteration

	this again is used to determine the maximum time
	a thread can sleep after the mandetory actions
	(to leave the compute power to other threads)
*/
# ifndef POINT1_X
#  define POINT1_X 4
# endif

# ifndef POINT1_Y
#  define POINT1_Y 2000 // changed from 3000
# endif

# ifndef POINT2_X
#  define POINT2_X 2046
# endif

# ifndef POINT2_Y
#  define POINT2_Y 170000
# endif

# ifndef POINT3_X
#  define POINT3_X 1024
# endif

# ifndef POINT3_Y
#  define POINT3_Y 47000
# endif


// used is used as a bool, int64_t for preticable layout
typedef struct s_fork
{
	int64_t			used; // 64
	pthread_mutex_t	mutex_used; // 128
	pthread_mutex_t	mutex; // 192
}	t_fork;

typedef struct s_philo
{
	// cache line 1 (0 - 64)
	pthread_mutex_t		*status;  //8
	int64_t				death_t; // 16
	bool				*exit;	// 24
	int64_t				current_t; // 32
	t_fork				*left_fork; // 40
	t_fork				*right_fork; // 48
	int64_t				eat_dur; // 56
	// cache line 2/3 (64 - 192)
	t_fork				main_fork; // 192
	int64_t				starve_dur; // 184
	 // 192
	int64_t				thinking_dur;
	int64_t				next_eat_t;
	int64_t				eat_count;
	int64_t				start_t;
	// cache line 4 (192 - 256)
	int64_t				sleep_dur;
	int64_t				index;
}	t_philo;

// exit and status have to be the first two values to ensure cache locality
typedef struct s_general
{
	bool					exit;
	pthread_mutex_t			status;
	t_philo					*philos;
	pthread_t				*threads;
	// count was uint16_t before
	int32_t					count;
	// were int32_t:
	int32_t					starve_dur;
	int32_t					eat_dur;
	int32_t					sleep_dur;
	int32_t					eat_count;
	// ^^were int32_t^^
	int64_t					start_t;
	t_philo					*ptr_to_free_philos;
	bool					even;
	bool					death_loop;
}	t_general;

// main.c
//void		*main_loop(void *arg);
void		*main_loop_fast(void *arg);
void		*main_loop_accurate(void *arg);
void		*main_loop_death(void *arg);
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
void		drop_forks(t_philo *restrict const philo);

//bool		eat(t_philo *restrict const philo);
bool		change_status(t_philo *restrict const philo, const char *restrict const status);
bool		check_exit(t_philo *restrict const philo);


void		drop_left_fork(t_philo *restrict const philo);
void		drop_right_fork(t_philo *restrict const philo);


bool	pickup_forks(t_philo *restrict const philo);
// time
bool		my_sleep_slow(const int64_t target_t,
				t_philo *restrict const philo);
void		my_sleep_fast(const int64_t target_t);
void		my_sleep_accurate(const int64_t target_t);
int64_t		get_microseconds(void);


int64_t		quadratic_function(int64_t x);


int			cleanup(t_general *const general);

// debug
void		print_philo_info(t_philo *restrict const philo,
				bool timings, bool check_mutexes);

#endif
