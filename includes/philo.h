/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:07 by frapp             #+#    #+#             */
/*   Updated: 2023/12/21 07:08:05 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#define USLEEP_TIME 30
//#define NEXT_EAT (philo->current_time + philo->eat_ti)
//#define NEXT_EAT (philo->starve_ti / 10)
//#define 
#define NEXT_EAT USLEEP_TIME + 1
//#define NEXT_EAT 10


# include "includes.h"

# ifndef MIN_SLEEP_T
#  define MIN_SLEEP_T 10
# endif

typedef struct s_fork
{
	bool			used;
	pthread_mutex_t	mutex_used;
	pthread_mutex_t	mutex;
} t_fork;


typedef struct s_philo
{
	uint8_t				index;
	uint16_t			starve_ti;
	uint16_t			eat_ti;
	uint16_t			sleep_ti;
	int16_t				eat_count;
	long long			death_time;
	long long			total_start_t;
	long long			current_time;
	bool				*exit;
	pthread_mutex_t		*mutex_exit;
	long long			next_eat;
	bool				even;
	t_fork				*left_fork;
	t_fork				*right_fork;
	t_fork				main_fork;
}	t_philo;

typedef struct	s_general
{
	int					count;
	int					starve_ti;
	int					eat_ti;
	int					sleep_ti;
	int					eat_count;
	long long			total_start_t;
	pthread_t			*threads;
	t_philo				*philos;
	bool				exit;
	pthread_mutex_t		mutex_exit;
}	t_general;


// main.c
void				*main_loop(void *arg);
long long			action(t_philo *philo);
void				wait_threads(t_general *general);
int					main(int ac, char *av[]);

// init.c
int					input(int ac, char *av[], t_general *gen);
int					fill_philo(t_general *general, int i);
int					intit_threadding(t_general *general);
int					init_philos(t_general *general);

// utils
int					ft_atoi(const char *str);
long long			my_gettime(void);
bool				pickup_left_fork(t_philo *philo);
bool				pickup_right_fork(t_philo *philo);
bool				drop_forks(t_philo *philo);
bool			check_exit(t_philo *philo, char *str);
//bool				check_starvation(t_philo *philo, char *str);
bool				eat(t_philo *philo);
void				my_sleep_until(long long target_time, t_philo *philo, char *str);


// debug
void					print_philo(t_philo *philo, bool timings, bool check_mutexes);

#endif