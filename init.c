/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:21:45 by frapp             #+#    #+#             */
/*   Updated: 2024/01/08 16:14:55 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// not performance critical
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


// not performance critical
static void	fill_odd_even(t_general *const general,
	t_philo *restrict const philo, int i)
{
	philo->next_eat_t = 0;
	if (general->even)
	{
		// if ((philo->starve_dur - philo->eat_dur - philo->sleep_dur) <= 0)
		// {
		// 	exit(0);
		// }
		// else
		// {
		// 	printf("%f\n", ((double)general->count) / ((philo->starve_dur - philo->eat_dur - philo->sleep_dur) / 1000));
		// 	exit(0);
		// 	philo->thinking_dur = (int64_t)((philo->starve_dur - philo->eat_dur - philo->sleep_dur) * 0.2);
		// }
		//int64_t		min_execution_time = linear_function_even(general->count);
		int64_t		min_execution_time = quadratic_function(general->count);
		philo->thinking_dur = ((philo->starve_dur - philo->eat_dur - philo->sleep_dur) - min_execution_time);
		static bool	once = true;
		if (once)
		{
			printf("min exection time: %lld\n", min_execution_time);
			printf("thinking_dur: %lld\n", philo->thinking_dur);
			usleep(2000000);
			//exit(0);
			once = false;
		}
		//philo->thinking_dur = 4800;
		if (i % 2)
			philo->next_eat_t = philo->eat_dur;
	}
	else
	{
		int64_t		min_execution_time = quadratic_function(general->count);
		//philo->thinking_dur = (philo->eat_dur * 2) + philo->sleep_dur;
		philo->thinking_dur = ((philo->starve_dur - (philo->eat_dur * 2) - philo->sleep_dur) - min_execution_time);
		//if (philo->thinking_dur <= philo->eat_dur)
		//	philo->thinking_dur = philo->eat_dur;
		static bool	once = true;
		if (once)
		{
			printf("min exection time: %lld\n", min_execution_time);
			printf("thinking_dur: %lld\n", philo->thinking_dur);
			usleep(2000000);
			//exit(0);
			once = false;
		}
		// philo->thinking_dur += 2000;
		// if (philo->thinking_dur <= 0)
		// 	philo->thinking_dur = 0;
		// if (philo->sleep_dur > philo->eat_dur)
		// {
		// 	philo->thinking_dur -= ((int) philo->eat_dur * 0.5);
		// }
		// else
		// {
		// 	philo->thinking_dur -= ((int) philo->sleep_dur * 0.5);
		// }
		if (!(i % 3))
			philo->next_eat_t = 0;
		else if (!((i + 2) % 3))
			philo->next_eat_t = philo->thinking_dur;
		else
			philo->next_eat_t = philo->thinking_dur + philo->sleep_dur;
			//philo->next_eat_t = philo->eat_dur * 2;
	}
}

// not performance critical
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
	else if (general->count == 1)
	{
		philo->right_fork = philo->left_fork;
		philo->left_fork->used = true;
	}
	fill_odd_even(general, philo, i);
	return (1);
}

// not performance critical
int	init_philos(t_general *const general)
{
	int64_t	i;

	general->even = true;
	if (general->count % 2)
		general->even = false;
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
	general->death_loop = false;
	if ((general->eat_dur + general->sleep_dur) > general->starve_dur)
		general->death_loop = true;
	else if (!general->even && ((int)(general->philos->starve_dur / 3)) <= general->eat_dur)
		general->death_loop = true;
	return (1);
}
