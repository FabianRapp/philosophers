/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:21:45 by frapp             #+#    #+#             */
/*   Updated: 2023/12/21 11:32:58 by frapp            ###   ########.fr       */
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
	{
		gen->eat_count = -1;
	}

	return (1);
}

int	fill_philo(t_general *general, int i)
{
	t_philo	*philo;

	philo = (general->philos + i);
	philo->index = i;
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
	if (i == general->count - 1)
		(general->philos)->right_fork = &(philo->main_fork);
	if (pthread_mutex_init(&philo->main_fork.mutex, NULL))
		return (0);
	if (pthread_mutex_init(&philo->main_fork.mutex_used, NULL))
		return (0);
	philo->next_eat = 0;
	//philo->eat_wait_time = philo->eat_ti >> 1;
	philo->eat_wait_time = (philo->eat_ti >> 4);
	if (!(general->count % 2))
	{
		philo->even = true;
		if (i % 2)
		{
			philo->next_eat += philo->eat_wait_time;
		}
		else
			philo->next_eat = 0;
	}
	else
	{
		philo->even = false;
		philo->eat_wait_time += philo->eat_ti;
		if (i % 3 && !(i % 2))
		{
			philo->next_eat +=  (philo->eat_ti) + philo->eat_wait_time;
		}
		else if (i % 2)
		{
			philo->next_eat +=  0;
		}
		philo->eat_wait_time += (philo->eat_ti >> 4);
	}
	return (1);
}

int	intit_threadding(t_general *general)
{
	int		i;
	t_philo	*philo;

	general->threads = malloc(sizeof(pthread_t) * (general->count + 1));
	if (!general->threads)
		return (0);
	general->total_start_t = my_gettime();
	i = 0;
	while (i < general->count)
	{
		philo = general->philos + i++;
		philo->total_start_t = general->total_start_t;
		if (pthread_create(general->threads + i, NULL, main_loop, philo))
			return (0);
	}
	return (1);
}

int	init_philos(t_general *general)
{
	int	i;

	general->philos = malloc(sizeof(t_philo) * (general->count + 1));
	if (!general->philos)
		return (0);
	general->exit = false;
	if (pthread_mutex_init(&general->mutex_exit, NULL))
		return (0);
	i = 0;
	while (i < general->count)
	{
		fill_philo(general, i);
		i++;
	}
	return (1);
}
