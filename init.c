/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:21:45 by frapp             #+#    #+#             */
/*   Updated: 2023/12/29 19:29:34 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	gen->starve_ti = ft_atoi(av[2]);
	gen->eat_ti = ft_atoi(av[3]);
	gen->sleep_ti = ft_atoi(av[4]);
	if (ac == 6)
		gen->eat_count = ft_atoi(av[5]);
	else
		gen->eat_count = -1;
	return (1);
}

static void	fill_odd_even(t_general *general, t_philo *philo, int i)
{
	philo->next_eat = 0;
	if (!(general->count % 2))
	{
		philo->eat_wait_time = philo->eat_ti + philo->sleep_ti;
		if (i % 2)
			philo->next_eat += philo->eat_ti;
	}
	else
	{
		philo->eat_wait_time = philo->eat_ti + philo->sleep_ti;
		philo->eat_wait_time += (philo->eat_ti >> 1) + (philo->eat_ti >> 2) + (philo->eat_ti >> 3) + (philo->eat_ti >> 4);
		if (!(i % 3))
			philo->next_eat = 0;
		else if (!((i + 2) % 3))
			philo->next_eat =  philo->eat_ti + philo->sleep_ti;
		else
			philo->next_eat = philo->eat_wait_time;
	}
}

int	fill_philo(t_general *general, int i)
{
	t_philo	*philo;

	philo = (general->philos + i);
	philo->index = -1;
	philo->starve_ti = -1;
	if (pthread_mutex_init(&philo->main_fork.mutex, NULL))
		return (cleanup (general));
	philo->index = i;
	if (pthread_mutex_init(&philo->main_fork.mutex_used, NULL))
		return (cleanup (general));
	philo->starve_ti = general->starve_ti;
	philo->eat_ti = general->eat_ti;
	philo->sleep_ti = general->sleep_ti;
	philo->eat_count = general->eat_count;
	philo->left_fork = &(philo->main_fork);
	(philo->exit) = &(general->exit);
	(philo->mutex_exit) = &(general->mutex_exit);
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

	general->total_start_t = my_gettime() + general->count * 4;
	i = 0;
	while (i < general->count)
	{
		philo = general->philos + i;
		thread = general->threads + i;
		philo->total_start_t = general->total_start_t;
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
	int		i;
	t_fork	solo_fork;

	general->threads = NULL;
	general->philos = NULL;
	general->threads = malloc(sizeof(pthread_t) * (general->count + 1));
	general->philos = malloc(sizeof(t_philo) * (general->count + 1));
	if (!(general->philos) || ! (general->threads))
		return (cleanup(general));
	general->exit = false;
	if (pthread_mutex_init(&general->mutex_exit, NULL))
		return (0);
	i = 0;
	if (general->count == 1)
	{
		general->philos->right_fork = &solo_fork;
		pthread_mutex_init(&(solo_fork.mutex), NULL);
		solo_fork.used = true;
		pthread_mutex_init(&(solo_fork.mutex_used), NULL);
	}
	while (i < general->count)
	{
		fill_philo(general, i++);
	}
	return (1);
}
