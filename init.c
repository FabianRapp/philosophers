/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/19 15:21:45 by frapp             #+#    #+#             */
/*   Updated: 2024/01/06 11:53:54 by fabi             ###   ########.fr       */
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

#define POINT1_X_EVEN 4
#define POINT1_Y_EVEN 3000
#define POINT2_X_EVEN 6000
#define POINT2_Y_EVEN 160000
#define POINT3_X_EVEN 256
#define POINT3_Y_EVEN 6200

int64_t	linear_function_even(int64_t x)
{
	double m = (double)(POINT2_Y_EVEN - POINT1_Y_EVEN) / (POINT2_X_EVEN - POINT1_X_EVEN);
	double c = POINT1_Y_EVEN - m * POINT1_X_EVEN;

	// Calculate y using the linear function y = mx + c
	return (int64_t)(m * x + c);
}


//philo->eat_wait_dur = (2 * philo->eat_dur) + philo->sleep_dur;
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
		// 	philo->eat_wait_dur = (int64_t)((philo->starve_dur - philo->eat_dur - philo->sleep_dur) * 0.2);
		// }
		//int64_t		min_execution_time = linear_function_even(general->count);
		int64_t		min_execution_time = quadratic_function_even(general->count);
		philo->eat_wait_dur = ((philo->starve_dur - philo->eat_dur - philo->sleep_dur) - min_execution_time);
		// //if (philo->eat_wait_dur <= 300)
		// {
		// 	printf("philo->eat_wait_dur %ld\n", philo->eat_wait_dur);
			if (philo->eat_wait_dur <= 0)
				philo->eat_wait_dur = 0;
		// 	//usleep(3000000);
		// }
		//philo->eat_wait_dur = 4800;
		// printf("%ld\n", philo->eat_wait_dur );
		// exit(0);
		// philo->eat_wait_dur = (int64_t)((philo->starve_dur - philo->eat_dur - philo->sleep_dur) * 0.6);

		//if (philo->starve_dur - philo->eat_dur - philo->sleep_dur - philo->eat_wait_dur < MIN_EAT_WAIT)
			//philo->eat_wait_dur = MIN_EAT_WAIT;
		//philo->eat_wait_dur = philo->starve_dur - philo->eat_dur - philo->sleep_dur - EXECUTION_TIME;
		//philo->eat_wait_dur += philo->eat_dur;// + 10000;//+ ((int)(philo->sleep_dur * 0.8));
		//philo->eat_wait_dur += philo->sleep_dur;
		if (i % 2)
			philo->next_eat_t = philo->eat_dur;
	}
	else
	{
		int64_t		min_execution_time = quadratic_function_even(general->count);
		//philo->eat_wait_dur = (philo->eat_dur * 2) + philo->sleep_dur;
		philo->eat_wait_dur = ((philo->starve_dur - philo->eat_dur - philo->sleep_dur) - min_execution_time);
		//if (philo->eat_wait_dur <= philo->eat_dur)
		//	philo->eat_wait_dur = philo->eat_dur;
		printf("%ld\n", philo->eat_wait_dur);
		philo->eat_wait_dur += 2000;
		if (philo->eat_wait_dur <= 0)
			philo->eat_wait_dur = 0;
		// if (philo->sleep_dur > philo->eat_dur)
		// {
		// 	philo->eat_wait_dur -= ((int) philo->eat_dur * 0.5);
		// }
		// else
		// {
		// 	philo->eat_wait_dur -= ((int) philo->sleep_dur * 0.5);
		// }
		if (!(i % 3))
			philo->next_eat_t = 0;
		else if (!((i + 2) % 3))
			philo->next_eat_t = philo->eat_wait_dur;
		else
			philo->next_eat_t = philo->eat_wait_dur + philo->sleep_dur;
			//philo->next_eat_t = philo->eat_dur * 2;
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
	else if (general->count == 1)
	{
		philo->right_fork = philo->left_fork;
		philo->left_fork->used = true;
	}
	if (general->even)
	{
		if ((philo->starve_dur / 2 - philo->eat_dur) > SPEED_BOUNDRY)
			philo->speed_mode = true;
		else
			philo->speed_mode = false;
	}
	else
	{
		//if ((philo->starve_dur / 3 - philo->eat_dur) > SPEED_BOUNDRY)
			philo->speed_mode = true;
		//else
		//	philo->speed_mode = false;
	}
	fill_odd_even(general, philo, i);
	return (1);
}

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
	else if (!general->even && ((general->eat_dur * 2) > general->starve_dur))
		general->death_loop = true;
	else if (!general->even && (general->philos->eat_wait_dur > general->starve_dur))
		general->death_loop = true;
	else if (!general->even && ((int)(general->philos->starve_dur / 3)) <= general->eat_dur)
		general->death_loop = true;
	return (1);
}
