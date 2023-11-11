/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 21:21:07 by frapp             #+#    #+#             */
/*   Updated: 2023/11/10 05:14:07 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include "includes.h"
# include <stdbool.h>

# ifndef USED
#  define USED 1
# endif

# ifndef NUSED
#  define NUSED 0
# endif

typedef struct s_general t_general;
typedef struct s_philo	t_philo;
typedef struct s_fork	t_fork;

typedef struct s_fork
{
	pthread_mutex_t	mutex_status;
	bool			status;
	pthread_mutex_t	mutex;
} t_fork;

typedef struct s_philo
{
	int					index;
	unsigned			starve_ti;
	unsigned			eat_ti;
	unsigned			sleep_ti;
	unsigned			eat_count;
	bool				dead;
	long long			current_time;
	long long			death_time;
	t_fork				*left_fork;
	t_fork				*right_fork;	
	t_fork				right_f;
	pthread_t			*thread;
	bool				*exit;
	pthread_mutex_t		*exit_mutex;
	pthread_mutex_t		*status_mutex;
} t_philo;

typedef struct s_general
{
	bool				exit;
	pthread_mutex_t		exit_mutex;
	pthread_mutex_t		status_mutex;
	int					count;
	int					*thread_index;
	unsigned			starve_ti;
	unsigned			eat_ti;
	unsigned			sleep_ti;
	unsigned			eat_count;
	pthread_t			*threads;
	t_philo				*philos;
	
} t_general;


// utils
unsigned	ft_atoi(const char *str);
// exit
int		cleanup(t_general *general);
// mutex
bool	check_exit(t_philo *philo);

// philo stuff
bool	starvation(t_philo *philo);

// utils 1
long long	get_time(void);

#endif