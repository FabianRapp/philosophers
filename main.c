/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:04 by frapp             #+#    #+#             */
/*   Updated: 2023/12/29 09:49:58 by fabi             ###   ########.fr       */
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

bool	action(t_philo *philo)
{
	if (!eat(philo))
		return (0);
	if (philo->eat_count > 0)
		philo->eat_count--;
	if (!print_status(philo, "is sleeping"))
		return (false);
	philo->current_time = philo->sleep_ti + philo->current_time;
	if (!my_sleep_until(philo->current_time, philo))
		return (false);
	return (true);
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
	// WILL NEED MODIFACTION FOR UNEVEN COUNT
	// if (!print_status(philo, "is thinking"))
	// 	return (false);
	if (!philo->index % 2)
		my_sleep_until(philo->current_time, philo);
	else
	{
		my_sleep_until((philo->current_time + philo->eat_ti) >> 1, philo);
	}
	while (philo->eat_count)
	{
		if (!print_status(philo, "is thinking"))
			break ;
		if (!eat(philo))
			break ;
		if (philo->eat_count > 0)
			philo->eat_count--;
		if (!print_status(philo, "is sleeping"))
			break ;
		philo->current_time = philo->sleep_ti + philo->current_time;
		if (!my_sleep_until(philo->current_time, philo))
			break ;
	}
	return (NULL);
}

void	wait_threads(t_general *general)
{
	int	i;

	i = 0;
	while (i < general->count)
	{
		pthread_join(general->threads[i], NULL);
		//printf("thread %d joined\n", i);
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
