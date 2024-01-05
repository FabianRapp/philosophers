/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/05 01:15:37 by fabi             ###   ########.fr       */
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

#define	THREADING_INIT_TIME_MICRO 6000
#define	MUTEX_FAIL_CHECK_VAL -1
#define	SHIFT_DIV_ESTIMATE 10
#define	MICROSEC_TO_MILLISEC 1000
#define CACHE_LINE_SIZE 64


#define	SLEEP_TOLERANCE 300
#define	HARDCODE_SLEEP 200

#define PREFETCH_EXIT __asm__ volatile("PREFETCHT1 %0" : : "m" (philo->death_t))

// testing
//#define DATA_TYPE_INT64
#define DATA_TYPE_INT64
// #define DATA_TYPE_INT8

#ifdef DATA_TYPE_INT64
    #define D_TYPE int64_t
    #define DATA_TYPE_SPECIFIER "%ld"
#elif defined(DATA_TYPE_INT32)
    #define D_TYPE int32_t
    #define DATA_TYPE_SPECIFIER "%d"
#elif defined(DATA_TYPE_INT8)
    #define D_TYPE int8_t
    #define DATA_TYPE_SPECIFIER "%d"
#endif

#define COUNT_TYPE int64_t

/*currently:
	sizeof(pthread_mutex_t == 40
	sizeof(t_fork) == 88
	sizeof(t_philo) == 184
	sizeof(t_general) == 96
*/
typedef struct s_fork
{
	// t_philo.left_fork should be in the same cache line (8 byte)
	pthread_mutex_t	mutex_used; //40 bytes
	int8_t			used; // used as bool but set to 1 byte for alignment
	int8_t			my_padding1;
	int8_t			my_padding2;// force mutex to aligned with t_philo.right_fork
	int8_t			my_padding3;
	int8_t			my_padding4;
	int8_t			my_padding5;
	int8_t			my_padding6;
	int8_t			my_padding7;
	int8_t			my_padding8;
	int8_t			my_padding9;
	/* this had to be in the same cache line, but if possible the entire struct should be in 1 cach line
		pthread_mutex_t	mutex_used;
		bool			used;
	*/
	pthread_mutex_t	mutex; // 40 bytes
} t_fork;

typedef struct s_philo
{
	pthread_mutex_t		*status; // shared between thread and will point to status in a t_general object
	int64_t				death_t; // important to be with exit and status; part of tight inner loop
	bool				*exit; // shared between thread and will point to exit in a t_general object
	int64_t				current_t;
	/* this had to be in the same cache line
		int64_t				death_t;
		bool				*exit;
		pthread_mutex_t		*status;
		int64_t				current_t;
	*/
	int64_t				my_padding1;
	int64_t				my_padding2;
	int64_t				my_padding3;
	int64_t				my_padding4;

	//------------new cache line duo to diffrent execution part------------
	// left_fork should ideally be in the same cache line as main_fork.mutex_used and main_fork.used(which are in the first 48 bytes of main_fork)
	t_fork				*left_fork;
	t_fork				main_fork;
	/*------------new cache line within main_fork duo to size:
	first part of main_fork with left_forl second with right_fork (sizeof(main_fork) == 88)
	>>>>>main_fork should NOT be aligned by itself, it should be after left_fork and followed by right_fork
	*/
	// if right_fork can be in the same cache line as main_fork.mutex it would be ideal
	t_fork				*right_fork; // 176

	// // the rest is irrelevant

	// 184
	D_TYPE				starve_dur; // not very important to have good cache position, ideally with current_t, starve_dur, eat_wait_dur, eat_dur
	// 192
	D_TYPE				eat_dur; // not very important like starve_dur
	// 200
	int64_t				next_eat_t;// not very important like starve_dur
	//208
	D_TYPE				eat_wait_dur; // very unimportant to have good cache position, used with next_eat_t and current_t
	// 216
	D_TYPE				eat_count;// not very important, used right before status when used
	// 224
	D_TYPE				sleep_dur;// not very important like starve_dur
	// 232
	int64_t				start_t; // very unimportant, used with current_t
	// 240
	D_TYPE				index; // very unimportant to have good cache position
	int64_t				my_padding5; // 248
	int64_t				my_padding6; // 256 -> 4 cache lines exact
}	t_philo;
//38
typedef struct	s_general
{
	// exit and status have to be in the same cache line
	bool					exit; // accessed in the tight inner loop by threads, protected by status
	pthread_mutex_t			status; // protects exit in the tight inner loop during multi threading with 100+ threads

	t_philo					*philos; // idk yet
	pthread_t				*threads;// irrelevant cache position
	uint16_t				count;// irrelevant cache position
	int32_t					starve_dur;// irrelevant cache position
	int32_t					eat_dur;// irrelevant cache position
	int32_t					sleep_dur;// irrelevant cache position
	int32_t					eat_count;// irrelevant cache position
	int64_t					start_t;// irrelevant cache position
	t_philo					*ptr_to_free_philos;
}	t_general;

// main.c
void					*main_loop(void *arg);
void					wait_threads(t_general *const general);
//int						main(int ac, char *av[]);

// init.c
int						input(int ac, char *av[], t_general *const gen);
int						fill_philo(t_general *const general, COUNT_TYPE i);
int						intit_threading(t_general *const general);
int						init_philos(t_general *const general);

// utils
D_TYPE					ft_atoi(const char *str);
void					align_ptr(int8_t **ptr);

bool					pickup_left_fork(t_philo *restrict const philo);
bool					pickup_right_fork(t_philo *restrict const philo);
bool					drop_forks(t_philo *restrict const philo);
bool					check_exit(t_philo *restrict const philo);
bool					eat(t_philo *restrict const philo);
bool					print_status(t_philo *restrict const philo, char *const status);
void					drop_left_fork(t_philo *restrict const philo);
void					drop_right_fork(t_philo *restrict const philo);
bool					do_exit(t_philo *restrict const philo, const bool locked_mutex);

// time
//bool					my_sleep_until(int64_t target_t, t_philo *philo);
bool					my_sleep_until_small(const int64_t target_t, t_philo *restrict const philo);
void					my_sleep_eating(const int64_t target_t);
void					my_sleep_init(const int64_t target_t);

int						cleanup(t_general *const general);

// debug
void					print_philo(t_philo *restrict const philo, bool timings, bool check_mutexes);

#endif
