/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:21:45 by frapp             #+#    #+#             */
/*   Updated: 2023/12/19 15:42:21 by frapp            ###   ########.fr       */
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
	gen->count = ft_atoi(av[1]);
	gen->starve_ti = ft_atoi(av[2]);
	gen->eat_ti = ft_atoi(av[3]);
	gen->sleep_ti = ft_atoi(av[4]);
	if (ac == 6)
		gen->eat_count = ft_atoi(av[5]);
	else
		gen->eat_count = -1;
	if (!gen->count)
	{
		printf("missing philo count\n");
		return (0);
	}
	return (1);
}

int	fill_philo(t_general *general, int i)
{
	(general->philos + i)->index = i;
	(general->philos + i)->starve_ti = general->starve_ti;
	(general->philos + i)->eat_ti = general->eat_ti;
	(general->philos + i)->sleep_ti = general->sleep_ti;
	(general->philos + i)->eat_count = general->eat_count;
	(general->philos + i)->left_fork = &((general->philos + i)->main_fork);
	((general->philos + i)->exit) = &(general->exit);
	if (i > 0)
		(general->philos + i)->right_fork = &((general->philos + i - 1)->main_fork);
	if (i == general->count - 1)
		(general->philos)->right_fork = &((general->philos + i)->main_fork);
	if (pthread_mutex_init(&(general->philos + i)->main_fork.mutex, NULL))
		return (0);
	return (1);
}

int	intit_threadding(t_general *general)
{
	int		i;

	i = -1;
	general->threads = malloc(sizeof(pthread_t) * (general->count + 1));
	if (!general->threads)
		return (0);
	general->total_start_t = my_gettime(0);
	printf("start time: %llu\n", general->total_start_t);
	while (++i < general->count)
	{
		(general->philos)[i].total_start_t = general->total_start_t;
		(general->philos)[i].death_time = general->total_start_t + general->starve_ti;
		if (pthread_create(general->threads + i, NULL, main_loop, general->philos + i))
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
	if (i == 1)
		return (handle_only_1_philo(general), 1);
	return (1);
}
