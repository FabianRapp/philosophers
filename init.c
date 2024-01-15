/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:21:45 by frapp             #+#    #+#             */
/*   Updated: 2024/01/15 06:49:55 by frapp            ###   ########.fr       */
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
	if (ft_atoi(av[1]) <= 0)
		return (printf("philo count must be > 0\n"), 0);
	if (ft_atoi(av[2]) < 0)
		return (printf("time to die must be >= 0\n"), 0);
	if (ft_atoi(av[3]) < 0)
		return (printf("eat time must be >= 0\n"), 0);
	if (ft_atoi(av[4]) < 0)
		return (printf("eat time must be >= 0\n"), 0);
	gen->count = ft_atoi(av[1]);
	gen->starve_dur = ft_atoi(av[2]) * MILLISEC_TO_MICROSEC_FACTOR;
	gen->eat_dur = ft_atoi(av[3]) * MILLISEC_TO_MICROSEC_FACTOR;
	gen->sleep_dur = ft_atoi(av[4]) * MILLISEC_TO_MICROSEC_FACTOR;
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
	int64_t	min_execution_time;

	min_execution_time = calculate_iteration_time(general->count);
	philo->thinking_dur = (
				(philo->starve_dur - (philo->eat_dur) - philo->sleep_dur)
				- min_execution_time);
	philo->next_eat_t = 0;
	if ((i % 2) == 0)
		philo->next_eat_t = philo->eat_dur;
	if (general->even)
	{
		philo->surplus_starve_dur = philo->starve_dur - philo->eat_dur - philo->sleep_dur;
		// philo->thinking_dur = (
		// 		(philo->starve_dur - philo->eat_dur - philo->sleep_dur)
		// 		- min_execution_time);
		//(void)i;
	}
	else
	{
		philo->surplus_starve_dur = philo->starve_dur - philo->eat_dur * 2 - philo->sleep_dur;
		// philo->thinking_dur = (
		// 		(philo->starve_dur - (philo->eat_dur * 2) - philo->sleep_dur)
		// 		- min_execution_time);
		// if (!(i % 3))
		// 	philo->next_eat_t = 0;
		// else if (!((i + 2) % 3))
		// 	philo->next_eat_t = philo->thinking_dur;
		// else
		// 	philo->next_eat_t = philo->thinking_dur + philo->eat_dur;
	}
}

void	cpy_general_to_philo(t_general *general, t_philo *philo, int i)
{
	philo->death_loop = general->death_loop;
	philo->starve_dur = general->starve_dur;
	philo->sleep_dur = general->sleep_dur;
	philo->eat_count = general->eat_count;
	philo->left_fork = &(philo->main_fork);
	(philo->exit) = &(general->exit);
	(philo->buffer_mutex) = &(general->buffer_mutex);
	philo->main_fork.used = false;
	philo->output_buffer = &(general->cur_buffer);
	philo->output_size = &(general->output_size);
	philo->index = i;
	philo->debug = general->debug;
	if (i > 0)
		philo->right_fork = &((general->philos + i - 1)->main_fork);
}

// not performance critical
int	fill_philo(t_general *const general, int64_t i)
{
	t_philo	*philo;

	philo = (general->philos + i);
	philo->eat_dur = FAIL_INIT;
	philo->starve_dur = FAIL_INIT;
	philo->sleep_dur = FAIL_INIT;
	if (pthread_mutex_init(&philo->main_fork.mutex, NULL))
		return (cleanup (general));
	philo->eat_dur = general->eat_dur;
	if (pthread_mutex_init(&philo->main_fork.mutex_used, NULL))
		return (cleanup (general));
	philo->eat_dur = general->eat_dur;
	if (pthread_mutex_init(&philo->testing_exit_mutext, NULL))
		return (cleanup (general));
	cpy_general_to_philo(general, philo, i);
	if (i == general->count - 1 && general->count > 1)
		(general->philos)->right_fork = &(philo->main_fork);
	else if (general->count == 1)
		philo->left_fork->used = true;
	philo->testing_exit = false;
	fill_odd_even(general, philo, i);
	return (1);
}

// not performance critical
int	init_philos(t_general *const general)
{
	int64_t	i;

	if (pthread_mutex_init(&general->buffer_mutex, NULL))
		return (0);
	general->exit = false;
	general->cur_buffer = general->output_buffer_a;
	general->output_size = 0;
	general->even = true;
	if (general->count % 2)
		general->even = false;
	general->death_loop = false;
	if ((general->eat_dur * 2) > general->starve_dur
		|| (general->eat_dur + general->sleep_dur) > general->starve_dur
		|| general->count == 1
		|| (!general->even && (general->starve_dur / 3) <= general->eat_dur)
		)
	{
		general->death_loop = true;
	}
	general->philos = NULL;
	general->threads = malloc(sizeof(pthread_t) * (general->count + 1));
	general->philos = malloc(sizeof(t_philo) * (general->count + 2));
	if (!(general->philos) || ! (general->threads))
		return (cleanup(general));
	general->ptr_to_free_philos = general->philos;
	align_ptr((int8_t **)(&(general->philos)));
	i = 0;
	while (i < general->count)
		fill_philo(general, i++);
	return (1);
}
