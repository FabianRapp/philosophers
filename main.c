/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/15 08:45:04 by frapp             #+#    #+#             */
/*   Updated: 2023/12/29 19:26:47 by fabi             ###   ########.fr       */
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
	- 1 800 200 200		does not work, litterly does nothing?
	- 5 800 200 200		works
	- 5 800 200 200 7	""
	- 4 410 200 200		works (does not die)
	- 4 310 200 100		works (should die)
*/

static inline void	intit_thread(t_philo *philo)
{
	philo->current_time = philo->total_start_t;
	philo->next_eat += philo->current_time;
	philo->death_time = philo->total_start_t + philo->starve_ti;
	my_sleep_until(philo->current_time, philo);
}

void	*main_loop(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	intit_thread(philo);
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
	return (arg);
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
	cleanup(&general);
	return (0);
}
