/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/03 16:26:52 by fabi             ###   ########.fr       */
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

#define	THREADDING_INIT_TIME_MICRO 4000
#define	MUTEX_FAIL_CHECK_VAL -1
#define	SHIFT_DIV_ESTIMATE 10
#define	MICROSEC_TO_MILLISEC 1000


// testing
//#define DATA_TYPE_INT64
#define DATA_TYPE_INT32
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

#define COUNT_TYPE uint16_t

typedef struct s_fork
{
	bool			used;
	pthread_mutex_t	mutex_used;
	pthread_mutex_t	mutex;
} t_fork;

typedef struct s_philo
{
	COUNT_TYPE			index;
	D_TYPE				starve_dur;
	D_TYPE				eat_dur;
	D_TYPE				sleep_dur;
	D_TYPE				eat_count;
	D_TYPE				eat_wait_dur;
	int64_t				death_t;
	int64_t				start_t;
	int64_t				current_t;
	bool				*exit;
	pthread_mutex_t		*status;
	int64_t				next_eat_t;
	t_fork				*left_fork;
	t_fork				*right_fork;
	t_fork				main_fork;
}	t_philo;

typedef struct	s_general
{
	COUNT_TYPE				count;
	D_TYPE					starve_dur;
	D_TYPE					eat_dur;
	D_TYPE					sleep_dur;
	D_TYPE					eat_count;
	int64_t					start_t;
	pthread_t				*threads;
	t_philo					*philos;
	bool					exit;
	pthread_mutex_t			status;
}	t_general;

// main.c
void					*main_loop(void *arg);
void					wait_threads(t_general *general);
int						main(int ac, char *av[]);

// init.c
int						input(int ac, char *av[], t_general *gen);
int						fill_philo(t_general *general, COUNT_TYPE i);
int						intit_threadding(t_general *general);
int						init_philos(t_general *general);

// utils
D_TYPE					ft_atoi(const char *str);

bool					pickup_left_fork(t_philo *philo);
bool					pickup_right_fork(t_philo *philo);
bool					drop_forks(t_philo *philo);
bool					check_exit(t_philo *philo);
bool					eat(t_philo *philo);
bool					print_status(t_philo *philo, char *status);

// time
//bool					my_sleep_until(int64_t target_t, t_philo *philo);
bool					my_sleep_until_small(int64_t target_t, t_philo *philo);
bool					my_sleep_until_large(int64_t target_t, t_philo *philo);

int						cleanup(t_general *general);

// debug
void					print_philo(t_philo *philo, bool timings, bool check_mutexes);

#endif
