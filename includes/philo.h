/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/14 00:40:24 by frapp            ###   ########.fr       */
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
# include "types.h"



# ifndef MICROSEC_PER_THREAD_INIT_TIME
#  define MICROSEC_PER_THREAD_INIT_TIME 2000
# endif

# ifndef FAIL_INIT
#  define FAIL_INIT -1
# endif

# ifndef MAX_STR_NBRS
#  define MAX_STR_NBRS 15
# endif

# ifndef MICROSEC_TO_MILLISEC_FACTOR
#  define MICROSEC_TO_MILLISEC_FACTOR 0.001
# endif

# ifndef MILLISEC_TO_MICROSEC_FACTOR
#  define MILLISEC_TO_MICROSEC_FACTOR 1000
# endif

# ifndef SEC_TO_MICROSEC_FACTOR
#  define SEC_TO_MICROSEC_FACTOR 1000000
# endif

# ifndef CACHE_LINE_SIZE
#  define CACHE_LINE_SIZE 64
# endif

# ifndef THINKING_MSG
#  define THINKING_MSG " is thinking\n"
# endif

# ifndef PICK_UP_FORK_MSG
#  define PICK_UP_FORK_MSG " has taken a fork\n"
# endif

# ifndef EATING_MSG
#  define EATING_MSG " is eating\n"
# endif

# ifndef SLEEP_MSG
#  define SLEEP_MSG " is sleeping\n"
# endif

# ifndef SLEEP_TOLERANCE_FAST
#  define SLEEP_TOLERANCE_FAST 200
# endif

# ifndef THINK_TIME_BUFFER
#  define THINK_TIME_BUFFER 700
# endif

# ifndef SLEEP_TOLERANCE_ACC
#  define SLEEP_TOLERANCE_ACC 500
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
#  define POINT1_Y 7000// 4000?
# endif

# ifndef POINT2_X
#  define POINT2_X 2046
# endif

# ifndef POINT2_Y
#  define POINT2_Y 170000 // 170000
# endif

# ifndef POINT3_X
#  define POINT3_X 200
# endif

# ifndef POINT3_Y
#  define POINT3_Y 8000 // was 6000 unstable now...
# endif


// main.c
//void		*main_loop(void *arg);
void		*choose_loop(void *arg);
void		*main_loop_death(void *philos);

// my_print.c
int8_t		put_output_to_buffer(const int64_t cur_time, const int64_t philo_nb,
				char *restrict buffer, const char *restrict str);
void		*flush_output_loop(void *gen);

//int		main(int ac, char *av[]);

// init.c
int			input(int ac, char *av[], t_general *const gen);
int			fill_philo(t_general *const general, int64_t i);
int			init_philos(t_general *const general);

bool		intit_threading(t_general *const general);
bool		intit_thread(t_philo *restrict const philo);
void		wait_threads(t_general *const general, pthread_t *status_thread);

// utils
int64_t		ft_atoi(const char *str);
void		align_ptr(int8_t **ptr);
void		*ft_memcpy(void *dst, const void *src, size_t n);

bool		pickup_left_fork(t_philo *restrict const philo);
bool		pickup_right_fork(t_philo *restrict const philo);
void		drop_forks(t_philo *restrict const philo);

// sync_utils
bool		change_status(t_philo *restrict const philo,
				const char *restrict status);
bool		is_dead(t_philo *restrict const philo);

void		drop_left_fork(t_philo *restrict const philo);
void		drop_right_fork(t_philo *restrict const philo);
bool		pickup_forks(t_philo *restrict const philo);

// time
bool		my_sleep_slow(const int64_t target_t,
				t_philo *restrict const philo);
void		my_sleep_fast(const int64_t target_t);
void		my_sleep_accurate(const int64_t target_t);
void		my_sleep_think(const int64_t target_t);
int64_t		get_microseconds(void);

int64_t		calculate_iteration_time(int64_t x);

int			cleanup(t_general *const general);

// debug
void		print_philo_info(t_philo *restrict const philo,
				bool timings, bool check_mutexes);
void		debug_info_loop_conditoning(t_general *general);

#endif
