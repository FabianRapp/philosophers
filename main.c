/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frapp <frapp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:04 by frapp             #+#    #+#             */
/*   Updated: 2023/12/21 11:41:10 by frapp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <philo.h>

/*
TODO:
	- handle philos death while waiting for forks
	- handle philos death in my_sleep_until
	- handle 1 philo only
	- handle early error returns and cleanup
Tests:
	- 1 800 200 200		works (should die		)
	- 5 800 200 200		does not works SOMETIMES(should not die)
	- 5 800 200 200 7	""
	- 4 410 200 200		works (does not die)
	- 4 310 200 100		works (should die)
*/

long long	action(t_philo *philo)
{
	long long	next_time;

	// if (check_exit(philo, "start action"))
	// 	return (0);
	printf("%lld %d is thinking\n", philo->current_time - philo->total_start_t, philo->index);
	if (!eat(philo))
		return (0);
	if (philo->eat_count > 0)
		philo->eat_count--;
	if (check_exit(philo))
		return (0);
	printf("%lld %d is sleeping\n", philo->current_time - philo->total_start_t, philo->index);
	next_time = philo->sleep_ti + philo->current_time;
	//if (!(philo->sleep_ti < MIN_SLEEP_T))
		my_sleep_until(next_time, philo);
	//else
	//	my_sleep_until(MIN_SLEEP_T + philo->current_time);
	return (next_time);
}

void	*main_loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	philo->current_time = philo->total_start_t;
	// if (philo->even)
	// {
	// 	if (philo->index % 2)
	// 		philo->next_eat = philo->current_time + (philo->eat_ti >> 1);
	// }
	// else
	philo->next_eat += philo->current_time;
	philo->death_time = philo->total_start_t + philo->starve_ti;
	while (philo->eat_count && !check_exit(philo))
	{
		philo->current_time = action(philo);
	}
	return (arg);
}

void	wait_threads(t_general *general)
{
	int	i;

	i = 0;
	while (i < general->count)
	{
		pthread_join(general->threads[i], NULL);
		printf("thread %d joined\n", i);
		i++;
	}
}

int	main(int ac, char *av[])
{
	t_general	general;

	if (!input(ac, av, &general))
		return (0);
	if (!init_philos(&general))
		return (0);
	if (!intit_threadding(&general))
		return (0);
	wait_threads(&general);
	return (0);
}
