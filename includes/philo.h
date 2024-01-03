/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:07 by frapp             #+#    #+#             */
/*   Updated: 2024/01/03 10:48:49 by fabi             ###   ########.fr       */
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

typedef struct s_fork
{
	bool			used;
	pthread_mutex_t	mutex_used;
	pthread_mutex_t	mutex;
} t_fork;

typedef struct s_philo
{
	int16_t				index;
	int16_t				starve_ti;
	int16_t				eat_ti;
	int16_t				sleep_ti;
	int16_t				eat_count;
	int16_t				eat_wait_time;
	int64_t				death_time;
	int64_t				total_start_t;
	int64_t				current_time;
	int64_t				current_time_precise;
	bool				*exit;
	pthread_mutex_t		*mutex_exit;
	int64_t			next_eat;
	t_fork				*left_fork;
	t_fork				*right_fork;
	t_fork				main_fork;
}	t_philo;

typedef struct	s_general
{
	int16_t					count;
	int16_t					starve_ti;
	int16_t					eat_ti;
	int16_t					sleep_ti;
	int16_t					eat_count;
	int64_t					total_start_t;
	pthread_t				*threads;
	t_philo					*philos;
	bool					exit;
	pthread_mutex_t			mutex_exit;
}	t_general;


// main.c
void					*main_loop(void *arg);
void					wait_threads(t_general *general);
int						main(int ac, char *av[]);

// init.c
int						input(int ac, char *av[], t_general *gen);
int						fill_philo(t_general *general, int i);
int						intit_threadding(t_general *general);
int						init_philos(t_general *general);

// utils
int						ft_atoi(const char *str);

bool					pickup_left_fork(t_philo *philo);
bool					pickup_right_fork(t_philo *philo);
bool					drop_forks(t_philo *philo);
bool					check_exit(t_philo *philo);
bool					eat(t_philo *philo);
bool					print_status(t_philo *philo, char *status);

// time
int64_t					my_gettime(void);
bool					my_sleep_until(int64_t target_time, t_philo *philo);
void					my_sleep(int milliseconds);
int64_t					get_microseconds(void);

int						cleanup(t_general *general);

// debug
void					print_philo(t_philo *philo, bool timings, bool check_mutexes);

#endif
