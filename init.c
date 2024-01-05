/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:21:45 by frapp             #+#    #+#             */
/*   Updated: 2024/01/05 14:10:59 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	input(int ac, char *av[], t_general *const gen)
{
	if (ac != 5 && ac != 6)
	{
		printf("invalid input: <./philosophers> <number_of_philosophers> "
			"<time_to_die> <time_to_eat> <time_to_sleep> [<number_of_times_each"
			"_philosopher_must_eat>]\n");
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

//philo->eat_wait_dur = (2 * philo->eat_dur) + philo->sleep_dur;
static void	fill_odd_even(t_general *const general,
	t_philo *restrict const philo, int i)
{
	philo->next_eat_t = 0;
	if (!(general->count % 2))
	{
		philo->eat_wait_dur = philo->eat_dur + ((int)(philo->sleep_dur * 0.8));
		if (i % 2)
			philo->next_eat_t = philo->sleep_dur;
	}
	else
	{
		philo->eat_wait_dur = (philo->eat_dur * 2) + philo->sleep_dur;
		if (philo->sleep_dur > philo->eat_dur)
		{
			philo->eat_wait_dur -= ((int) philo->eat_dur * 0.5);
		}
		else
		{
			philo->eat_wait_dur -= ((int) philo->sleep_dur * 0.5);
		}
		if (!(i % 3))
			philo->next_eat_t = 0;
		else if (!((i + 2) % 3))
			philo->next_eat_t = philo->eat_dur;
		else
			philo->next_eat_t = philo->eat_dur * 2;
	}
}

int	fill_philo(t_general *const general, int64_t i)
{
	t_philo	*philo;

	philo = (general->philos + i);
	philo->eat_dur = FAIL_INIT;
	philo->starve_dur = FAIL_INIT;
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

int	init_philos(t_general *const general)
{
	int64_t	i;

	general->threads = NULL;
	general->philos = NULL;
	general->threads = malloc(sizeof(pthread_t) * (general->count + 1));
	general->philos = malloc(
			sizeof(t_philo) * (general->count + 1) + CACHE_LINE_SIZE);
	if (!(general->philos) || ! (general->threads))
		return (cleanup(general));
	general->ptr_to_free_philos = general->philos;
	align_ptr((int8_t **)(&(general->philos)));
	general->exit = false;
	if (pthread_mutex_init(&general->status, NULL))
		return (0);
	i = 0;
	while (i < general->count)
		fill_philo(general, i++);
	return (1);
}
