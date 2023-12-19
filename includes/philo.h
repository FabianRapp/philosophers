/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:07 by frapp             #+#    #+#             */
/*   Updated: 2023/12/19 15:27:47 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H

# define PHILO_H
# include "includes.h"

typedef struct s_fork
{
	bool			used;
	pthread_mutex_t	mutex_used;
	pthread_mutex_t	mutex;
} t_fork;


typedef struct s_philo
{
	int					index;
	unsigned			starve_ti;
	unsigned			eat_ti;
	unsigned			sleep_ti;
	unsigned			eat_count;
	unsigned			death_time;
	unsigned long long	total_start_t;
	t_fork				*left_fork;
	t_fork				*right_fork;
	t_fork				main_fork;
	bool				*exit;
	pthread_mutex_t		mutex_exit;
}	t_philo;

typedef struct	s_general
{
	int					count;
	unsigned			starve_ti;
	unsigned			eat_ti;
	unsigned			sleep_ti;
	unsigned			eat_count;
	unsigned long long	total_start_t;
	pthread_t			*threads;
	t_philo				*philos;
	bool				exit;
	pthread_mutex_t		mutex_exit;
}	t_general;


// main.c
void				*main_loop(void *arg);
int					eat(t_philo *philo);
void				wait_threads(t_general *general);
int					main(int ac, char *av[]);

// init.c
int					input(int ac, char *av[], t_general *gen);
int					fill_philo(t_general *general, int i);
int					intit_threadding(t_general *general);
int					init_philos(t_general *general);

// utils
long long			get_time(void);
unsigned			ft_atoi(const char *str);
void				handle_only_1_philo(t_general *general);
void				my_sleep(unsigned time);
unsigned long long	my_gettime(unsigned long long start_time);
bool				pickup_left_fork(t_philo *philo);
bool				pickup_right_fork(t_philo *philo);
bool				drop_forks(t_philo *philo);
bool				check_exit(t_philo *philo, char *str);
bool				check_starvation(t_philo *philo, char *str);


#endif