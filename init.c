/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:21:45 by frapp             #+#    #+#             */
/*   Updated: 2024/01/03 17:06:17 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static inline int64_t	get_microseconds_init(void)
{
	struct timeval s_time;
	int64_t time;

	gettimeofday(&s_time, NULL);
	time = (int64_t)s_time.tv_sec * 1000000;
	time += s_time.tv_usec;
	return (time);
}

int	input(int ac, char *av[], t_general *gen)
{
	if (ac !=5 && ac !=6)
	{
		printf("invalid input: <./philosophers> <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [<number_of_times_each_philosopher_must_eat>]\n");
		return (0);
	}
	if (!ft_atoi(av[1]))
	{
		printf("missing philo count\n");
		return (0);
	}
	gen->count = ft_atoi(av[1]);
	gen->starve_dur = ft_atoi(av[2]) * 1000;
	gen->eat_dur = ft_atoi(av[3]) * 1000;
	gen->sleep_dur = ft_atoi(av[4]) * 1000;
	if (ac == 6)
		gen->eat_count = ft_atoi(av[5]);
	else
		gen->eat_count = -1;
	return (1);
}

static void	fill_odd_even(t_general *general, t_philo *philo, int i)
{
	philo->next_eat_t = 0;
	if (!(general->count % 2))
	{
		philo->eat_wait_dur = philo->eat_dur + philo->sleep_dur;
		if (i % 2)
			philo->next_eat_t = philo->eat_dur;
	}
	else
	{
		philo->eat_wait_dur = philo->eat_dur + philo->sleep_dur;
		philo->eat_wait_dur += (int)(philo->eat_dur * 0.9);
		//philo->eat_wait_dur += (philo->eat_dur >> 1) + (philo->eat_dur >> 2) + (philo->eat_dur >> 3) + (philo->eat_dur >> 4);
		if (!(i % 3))
			philo->next_eat_t = 0;
		else if (!((i + 2) % 3))
			philo->next_eat_t =  philo->eat_dur + philo->sleep_dur;
		else
			philo->next_eat_t = philo->eat_wait_dur;
	}
}

int	fill_philo(t_general *general, COUNT_TYPE i)
{
	t_philo	*philo;

	philo = (general->philos + i);
	philo->eat_dur = MUTEX_FAIL_CHECK_VAL;
	philo->starve_dur= MUTEX_FAIL_CHECK_VAL;
	if (pthread_mutex_init(&philo->main_fork.mutex, NULL))
		return (cleanup (general));
	philo->eat_dur = general->eat_dur;
	if (pthread_mutex_init(&philo->main_fork.mutex_used, NULL))
		return (cleanup (general));
	philo->starve_dur = general->starve_dur;
	philo->index = i;
	philo->sleep_dur = general->sleep_dur;
	philo->eat_count = general->eat_count;
	philo->left_fork = &(philo->main_fork);
	(philo->exit) = &(general->exit);
	(philo->status) = &(general->status);
	philo->main_fork.used = false;
	if (i > 0)
		philo->right_fork = &((general->philos + i - 1)->main_fork);
	if (i == general->count - 1 && general->count > 1)
		(general->philos)->right_fork = &(philo->main_fork);
	fill_odd_even(general, philo, i);
	return (1);
}

int	intit_threadding(t_general *general)
{
	int			i;
	t_philo		*philo;
	pthread_t	*thread;

	general->start_t = get_microseconds_init();
	general->start_t += general->count * THREADDING_INIT_TIME_MICRO;
	i = 0;
	while (i < general->count)
	{
		philo = general->philos + i;
		thread = general->threads + i;
		philo->start_t = general->start_t;
		if (pthread_create(thread + 1, NULL, main_loop, philo))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

int	init_philos(t_general *general)
{
	COUNT_TYPE	i;
	t_fork		solo_fork;

	general->threads = NULL;
	general->philos = NULL;
	general->threads = malloc(sizeof(pthread_t) * (general->count + 1));
	general->philos = malloc(sizeof(t_philo) * (general->count + 1));
	if (!(general->philos) || ! (general->threads))
		return (cleanup(general));
	general->exit = false;
	if (pthread_mutex_init(&general->status, NULL))
		return (0);
	if (general->count == 1)
	{
		general->philos->right_fork = &solo_fork;
		pthread_mutex_init(&(solo_fork.mutex), NULL);
		solo_fork.used = true;
		pthread_mutex_init(&(solo_fork.mutex_used), NULL);
	}
	i = 0;
	while (i < general->count)
	{
		fill_philo(general, i++);
	}
	return (1);
}
